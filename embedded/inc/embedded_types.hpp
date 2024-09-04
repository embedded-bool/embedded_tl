/**
 * @file embedded_types.hpp
 * @date 2024-08-07
 * @author Robert Morley
 *
 * @brief 
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#ifndef EMBEDDED_TL_EMBEDDED_TYPES_HPP
#define EMBEDDED_TL_EMBEDDED_TYPES_HPP

#include <cstdint>
#include <limits>
#include <concepts>
#include <array>

namespace embtl {

#ifdef UNIT_TEST
    constexpr bool REGISTER_SIMULATION{true};
#else
    constexpr bool REGISTER_SIMULATION { false };
#endif

#if defined EMBEDDED_SYSTEM_8BIT
    using arch_type = std::uint8_t;
#elif defined EMBEDDED_SYSTEM_16BIT
    using arch_type = std::uint16_t;
#elif defined EMBEDDED_SYSTEM_64BIT
    using arch_type = std::uint64_t;
#else
    using arch_type = std::uint32_t;
#endif

    using address_t = arch_type;
    /**
     * @brief Return status of device or method.
     */
    enum class STATUS : std::int32_t {
        ERROR = std::numeric_limits<std::int32_t>::min(),
        NOT_IMPLEMENTED,
        NOT_AVAILABLE,
        BUFFER_OVERFLOW,
        INVALID_PARAMETER,
        OUT_OF_RANGE,
        UNINITIALIZED,
        OK = 0,
        BUSY,
        INITIALIZED
    };

    enum IO_STATE : arch_type {
        LOW = 0,
        HIGH
    };

    enum IO_DIRECTION : arch_type {
        INPUT = 0,
        OUTPUT
    };

    enum class SIGNAL_EDGES : arch_type {
        FALLING = 0,
        RISING
    };
}

#ifndef __cpp_size_t_suffix

constexpr std::size_t operator ""_uz (unsigned long long value) {
  return static_cast<std::size_t>(value);
}

#endif

#endif //EMBEDDED_TL_EMBEDDED_TYPES_HPP
