/**
 * @file embedded_return_type.hpp
 * @date 2024-08-15
 * @author Robert Morley
 *
 * @brief
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#ifndef EMBEDDED_TEMPLATE_LIBRARY_EMBEDDED_RETURN_TYPE_HPP
#define EMBEDDED_TEMPLATE_LIBRARY_EMBEDDED_RETURN_TYPE_HPP

#include <concepts>
#include <algorithm>
#include <functional>
#include <embedded_types.hpp>

namespace embtl {

    template<typename ValueType, ValueType Default>
    requires std::is_trivial_v<ValueType>
    struct basic_return_value_status final {
      public:
        using value_type = ValueType;
        using status_type = embtl::STATUS;

        constexpr basic_return_value_status() noexcept : basic_return_value_status(Default, default_status) {}
        explicit constexpr basic_return_value_status(const value_type val) noexcept : basic_return_value_status(val, default_status){}
        explicit constexpr basic_return_value_status(status_type stat) noexcept : basic_return_value_status(Default, stat) {}
        constexpr basic_return_value_status(const value_type val, status_type stat) noexcept : value(val), status(stat) {}

        constexpr basic_return_value_status(const basic_return_value_status&) noexcept = default;
        constexpr basic_return_value_status& operator=(const basic_return_value_status&) noexcept = default;
        constexpr basic_return_value_status(basic_return_value_status&&) noexcept = default;
        constexpr basic_return_value_status& operator=(basic_return_value_status&&) noexcept = default;

        [[nodiscard]] constexpr value_type get_value() const noexcept { return value; }
        [[nodiscard]] constexpr status_type get_status() const noexcept { return status; }

        constexpr basic_return_value_status& operator=(const value_type val) noexcept {
          value = val;
          return *this;
        }

        constexpr basic_return_value_status& operator=(status_type stat) noexcept {
          status = stat;
          return *this;
        }

        constexpr explicit operator bool() noexcept {
          return !has_error();
        }

        constexpr basic_return_value_status and_then(auto&& func) noexcept {
          using U = std::remove_cvref_t<std::invoke_result_t<decltype(func), value_type, status_type>>;
          static_assert(std::is_same_v<basic_return_value_status, U>);

          if(has_value()){ // No Error
            return std::invoke(func, value, status);
          } else { // has error
            return { value, status };
          }
        }

        constexpr basic_return_value_status or_else(auto&& func) noexcept {
          using U = std::remove_cvref_t<std::invoke_result_t<decltype(func), value_type, status_type>>;
          static_assert(std::is_same_v<basic_return_value_status, U>);

          if(has_error()){
            return std::invoke(func, value, status);
          } else { // has error
            return { value, status };
          }
        }

        [[nodiscard]] constexpr bool has_error() const noexcept {
          return static_cast<std::underlying_type_t<embtl::STATUS>>(status) < 0;
        }

        [[nodiscard]] constexpr bool has_value() const noexcept{
          return !has_error();
        }

        constexpr bool operator==(const value_type val) const noexcept {
          return value == val;
        }

        constexpr bool operator==(status_type stat) const noexcept {
          return status == stat;
        }

        constexpr bool operator !=(const value_type val) const noexcept {
          return value != val;
        }

        constexpr bool operator !=(status_type stat) const noexcept {
          return status != stat;
        }

      private:
        static constexpr embtl::STATUS default_status { STATUS::OK };

        value_type value;
        status_type status;
    };
}

#endif //EMBEDDED_TEMPLATE_LIBRARY_EMBEDDED_RETURN_TYPE_HPP
