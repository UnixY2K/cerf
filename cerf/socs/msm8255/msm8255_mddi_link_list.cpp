#include "msm8255_mddi_link_list.h"

#include "msm8255_mddi_client.h"

#include "../../boards/board_context.h"
#include "../../core/cerf_emulator.h"
#include "../../core/fatal.h"
#include "../../cpu/emulated_memory.h"

#include <cstdint>

namespace {

/* Linux arch/arm/mach-msm video-msm mddihosti.h:
   mddi_host_llist_struct. */
constexpr uint32_t kItemFlagsOff       =  0u;
constexpr uint32_t kItemHeaderCountOff =  2u;
constexpr uint32_t kItemDataCountOff   =  4u;
constexpr uint32_t kItemDataPtrOff     =  6u;
constexpr uint32_t kItemNextPtrOff     = 10u;
constexpr uint32_t kItemPacketOff      = 16u;

/* Linux arch/arm/mach-msm video-msm mddihosti.h:
   mddi_register_access_packet_type. */
constexpr uint32_t kPktTypeOff    =  2u;
constexpr uint32_t kPktRwInfoOff  =  6u;
constexpr uint32_t kPktAddressOff =  8u;

constexpr uint32_t kTypeRegisterAccess = 146u;

constexpr uint32_t kRegisterAccessHeaderBytes = 14u;

/* Linux arch/arm/mach-msm video-msm mddihosti.h: read_write_info carries the
   count of 32-bit register data items in bits 13:0, and the direction in bits
   15:14, where 00 writes registers and 10 reads them. */
constexpr uint32_t kRwCountMask = 0x3FFFu;
constexpr uint32_t kRwOpShift   = 14u;
constexpr uint32_t kRwOpWrite   = 0u;
constexpr uint32_t kRwOpRead    = 2u;

constexpr uint32_t kOneDataItem   = 1u;
constexpr uint32_t kDataItemBytes = 4u;

/* Linux arch/arm/mach-msm video-msm mddihosti.c: appending to the chain clears
   the end flag of the item that was last, so bit 0 marks the final item. */
constexpr uint32_t kFlagEndOfList = 0x0001u;

constexpr uint32_t kFlagsAccepted = kFlagEndOfList | 0x0010u;

constexpr uint32_t kMaxChainItems = 25u;

}

bool Msm8255MddiLinkList::ShouldRegister() {
    auto* bd = emu_.TryGet<BoardContext>();
    return bd && bd->GetSoc() == SocFamily::MSM8255;
}

uint32_t Msm8255MddiLinkList::ReadUnalignedWord(uint32_t pa) {
    auto& mem = emu_.Get<EmulatedMemory>();
    return mem.ReadHalf(pa) |
           (static_cast<uint32_t>(mem.ReadHalf(pa + 2u)) << 16);
}

void Msm8255MddiLinkList::Execute(uint32_t head_pa,
                                  Msm8255MddiLinkListHost& host) {
    uint32_t item  = head_pa;
    uint32_t walked = 0u;
    while (item != 0u) {
        if (walked == kMaxChainItems) {
            emu_.Get<Fatal>().Die(
                "msm8255 mddi link list: the chain at 0x%08X still runs after "
                "%u items", head_pa, walked);
        }
        ExecuteItem(item, host);
        item = ReadUnalignedWord(item + kItemNextPtrOff);
        ++walked;
    }
}

void Msm8255MddiLinkList::ExecuteItem(uint32_t item_pa,
                                      Msm8255MddiLinkListHost& host) {
    auto& mem = emu_.Get<EmulatedMemory>();

    const uint32_t flags = mem.ReadHalf(item_pa + kItemFlagsOff);
    if ((flags & ~kFlagsAccepted) != 0u) {
        emu_.Get<Fatal>().Die(
            "msm8255 mddi link list: the item at 0x%08X carries link "
            "controller flags 0x%04X", item_pa, flags);
    }

    const uint32_t packet_pa = item_pa + kItemPacketOff;
    const uint32_t type      = mem.ReadHalf(packet_pa + kPktTypeOff);
    if (type != kTypeRegisterAccess) {
        emu_.Get<Fatal>().Die(
            "msm8255 mddi link list: the item at 0x%08X sends packet type %u",
            item_pa, type);
    }

    const uint32_t header_bytes = mem.ReadHalf(item_pa + kItemHeaderCountOff);
    if (header_bytes != kRegisterAccessHeaderBytes) {
        emu_.Get<Fatal>().Die(
            "msm8255 mddi link list: the item at 0x%08X sends a %u-byte packet "
            "header", item_pa, header_bytes);
    }

    ExecuteRegisterAccess(item_pa, packet_pa, host);
}

void Msm8255MddiLinkList::ExecuteRegisterAccess(
    uint32_t item_pa, uint32_t packet_pa, Msm8255MddiLinkListHost& host) {
    auto& mem = emu_.Get<EmulatedMemory>();

    const uint32_t rw_info = mem.ReadHalf(packet_pa + kPktRwInfoOff);
    const uint32_t count   = rw_info & kRwCountMask;
    if (count != kOneDataItem) {
        emu_.Get<Fatal>().Die(
            "msm8255 mddi link list: the register access at 0x%08X transfers "
            "%u data items", item_pa, count);
    }

    const uint32_t address = ReadUnalignedWord(packet_pa + kPktAddressOff);
    auto&          client  = emu_.Get<Msm8255MddiClient>();

    const uint32_t op = rw_info >> kRwOpShift;
    if (op == kRwOpRead) {
        host.QueueRegisterReadResponse(address, client.ReadRegister(address));
        return;
    }
    if (op != kRwOpWrite) {
        emu_.Get<Fatal>().Die(
            "msm8255 mddi link list: the register access at 0x%08X carries "
            "read_write_info 0x%04X", item_pa, rw_info);
    }

    const uint32_t data_bytes = mem.ReadHalf(item_pa + kItemDataCountOff);
    if (data_bytes != kDataItemBytes) {
        emu_.Get<Fatal>().Die(
            "msm8255 mddi link list: the register write at 0x%08X sends %u "
            "data bytes", item_pa, data_bytes);
    }

    const uint32_t data_pa = ReadUnalignedWord(item_pa + kItemDataPtrOff);
    client.WriteRegister(address, ReadUnalignedWord(data_pa));
}

REGISTER_SERVICE(Msm8255MddiLinkList);
