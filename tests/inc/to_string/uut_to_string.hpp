/**
 * @file uut_to_string.hpp
 * @date 2024-10-05
 * @author Robert Morley
 *
 * @brief
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#ifndef EMBEDDED_TL_UUT_TO_STRING_HPP
#define EMBEDDED_TL_UUT_TO_STRING_HPP

#include <iostream>
#include <embedded_types.hpp>

namespace embtl {
    std::ostream& operator<<(std::ostream&, const STATUS&) noexcept;
}

#endif //EMBEDDED_TL_UUT_TO_STRING_HPP
