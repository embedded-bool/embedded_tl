/**
 * @file embedded_utilities.hpp
 * @date 2024-09-03
 * @author Robert Morley
 *
 * @brief Embedded Template Utilities header file.
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#ifndef EMBEDDED_TEMPLATE_LIBRARY_EMBEDDED_UTILITIES_HPP
#define EMBEDDED_TEMPLATE_LIBRARY_EMBEDDED_UTILITIES_HPP

#include <embedded_types.hpp>

namespace embtl {
    /**
     * @brief Converts upper case ASCII char to lower case.
     * @param c [in] ASCII character ['a'-'z'] or ['A' or 'Z']
     * @return Lower case value of the ASCII character
     * @note If character is given is not an alpha character the result is unknown.
     */
    constexpr char to_lower(const char c) noexcept {
      constexpr char upper_diff = 'a' - 'A';
      if(c >= 'A' && c <= 'Z'){
        return static_cast<char>(c - upper_diff);
      } else {
        return c;
      }
    }
    /**
     * @brief Converts alpha character to an index number
     * @param c ASCII character, ['a'-'z'] or ['A' or 'Z']
     * @return
     */
    constexpr std::size_t char_to_index(const char c) noexcept {
      auto x = to_lower(c);
      return static_cast<std::size_t>(x - 'a');
    }
}

#endif //EMBEDDED_TEMPLATE_LIBRARY_EMBEDDED_UTILITIES_HPP
