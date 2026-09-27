#pragma once
#ifdef __cplusplus
#include <cstdint>

#if !defined(_MSC_VER) || (!defined(_M_IX86) && !defined(_M_X64))
[[nodiscard]] constexpr std::uint64_t __emulu(std::uint32_t a,
                                              std::uint32_t b) noexcept {
	return static_cast<std::uint64_t>(a) * b;
}
#endif

#else
// shows as unused, but the macro uses the int types
#include <cstdint> // IWYU pragma: keep

#if !defined(_MSC_VER) || (!defined(_M_IX86) && !defined(_M_X64))
#define __emulu(a, b) ((uint64_t)(uint32_t)(a) * (uint32_t)(b))
#endif
#endif
