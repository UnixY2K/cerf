#include "../board_context.h"

#include "siemens_mp377_id.h"
#include "../../core/cerf_emulator.h"

namespace {

class SiemensMp377Context : public BoardContext {
public:
    using BoardContext::BoardContext;

    std::string_view GetBoardId() const override { return BoardId::SiemensMp377; }
};

}  /* namespace */

REGISTER_SERVICE_AS(SiemensMp377Context, BoardContext);
