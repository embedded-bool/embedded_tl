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

    template<typename T, typename S>
    concept rvs_type = requires (T, S) {
      (std::is_enum_v<S> && std::is_signed_v<std::underlying_type_t<S>>);   // Status must be enum and underlying type is signed type.
//    (std::is_integral_v<T>);                                              // Value type must be integral type.
    };


    template<typename ValueType, ValueType Default>
    requires std::is_trivial_v<ValueType>
    struct basic_return_value_status final {
      public:
        using value_type = ValueType;
        using status_type = embtl::STATUS;

        basic_return_value_status() noexcept : basic_return_value_status(Default, default_status) {}
        explicit basic_return_value_status(const value_type val) noexcept : basic_return_value_status(val, default_status){}
        explicit basic_return_value_status(status_type stat) noexcept : basic_return_value_status(Default, stat) {}
        basic_return_value_status(const value_type val, status_type stat) noexcept : value(val), status(stat) {}

        basic_return_value_status(const basic_return_value_status&) noexcept = default;
        basic_return_value_status& operator=(const basic_return_value_status&) noexcept = default;
        basic_return_value_status(basic_return_value_status&&) noexcept = default;
        basic_return_value_status& operator=(basic_return_value_status&&) noexcept = default;

        [[nodiscard]] value_type get_value() const noexcept { return value; }
        [[nodiscard]] status_type get_status() const noexcept { return status; }

        basic_return_value_status& operator=(const value_type val) noexcept {
          value = val;
          return *this;
        }

        basic_return_value_status& operator=(status_type stat) noexcept {
          status = stat;
          return *this;
        }

        explicit operator bool() noexcept {
          return static_cast<std::int32_t>(status) >= 0;
        }

        auto and_then(auto&& func) noexcept {

        }

        auto or_else(auto&& func) noexcept {

        }

      private:
        static constexpr embtl::STATUS default_status { STATUS::OK };

        value_type value;
        status_type status;
    };
//
//    template<typename VALUE_T, typename STATUS_T, VALUE_T DEF_VALUE, STATUS_T DEF_STATUS>
//    requires rvs_type<VALUE_T, STATUS_T>
//    struct return_value_status final {
//      public:
//        using value_type = VALUE_T;
//        using status_type = STATUS_T;
//        /**
//         * @brief Initialize all constructor, all variables are initialized by given parameters.
//         * @param val [in] Initial value number.
//         * @param stat [in] Initial status value.
//         */
//        constexpr return_value_status(const value_type val, const status_type stat) noexcept: m_value(val), m_status(stat) {}
//        /**
//         * @brief Default construct, value and status are set to default values given in template parameters.
//         */
//        constexpr return_value_status() noexcept: return_value_status(DEF_VALUE, DEF_STATUS) {}
//        /**
//         * @brief Initialize value_type, status_type is given default value given by template parameters.
//         * @param val
//         */
//        constexpr explicit return_value_status(const value_type val) noexcept: return_value_status(val, DEF_STATUS) {}
//        /**
//         * @brief Initialize status_type, value_type is given default value in template parameter.
//         * @param stat
//         */
//        constexpr explicit return_value_status(const status_type stat) noexcept: return_value_status(DEF_VALUE, stat) {}
//
//        constexpr return_value_status(const return_value_status &) noexcept = default;
//        constexpr return_value_status &operator=(const return_value_status &) noexcept = default;
//        constexpr return_value_status(return_value_status &&) noexcept = default;
//        constexpr return_value_status& operator=(return_value_status &&) noexcept = default;
//        /**
//         * @brief Assignment operator for value_type variable.
//         * @param val [in] New value_type to be assigned.
//         * @return Reference to this class.
//         */
//        constexpr return_value_status& operator=(const value_type val) noexcept {
//          m_value = val;
//          return *this;
//        }
//        /**
//         * @brief Assignment operator for status_type variable.
//         * @param stat [in] New status_type value to be assigned.
//         * @return Reference to this class.
//         */
//        constexpr return_value_status& operator=(const status_type stat) noexcept {
//          m_status = stat;
//          return *this;
//        }
//        /**
//         * @brief Invokes func parameter if current status has no error.
//         *
//         * @param func [in] rvalue reference to invoked function.
//         *
//         * @return Return Value Status class
//         * @note Expected function format parameter := [](value_type val, status_type stat) -> return_value_status { }
//         */
//        constexpr auto and_then(auto&& func) const noexcept -> return_value_status {
//          using U = std::remove_cvref_t<std::invoke_result_t<decltype(func), value_type, status_type>>;
//          static_assert(std::is_same_v<return_value_status, U>);
//
//          if(!has_error()){ // No Error
//            return std::invoke(std::forward(func), m_value, m_status);
//          } else { // has error
//            return { m_value, m_status };
//          }
//        }
//        /**
//         * @brief Transforms current return_value_status into another return_value_status when there is no status error.
//         *
//         * @tparam U Value type of new return_value_status<>
//         * @tparam V Status type of new return_value_status<>
//         * @tparam DEF_U Default value of new return_value_status<>
//         * @tparam DEF_V Default status of new return_value_status<>
//         *
//         * @param func [in] rvalue reference to invoked function.
//         *
//         * @return Transformed return_value_status<> data.
//         * @note Expected function format parameter := [](value_type val, status_type stat) -> return_value_status { }
//         */
//        template<typename U, typename V, U DEF_U, V DEF_V>
//        constexpr auto transform(auto&& func) -> return_value_status<U, V, DEF_U, DEF_V>{
//          using RTYPE = std::remove_cvref_t<std::invoke_result_t<decltype(func), value_type, status_type>>;
//          static_assert(std::is_same_v<return_value_status<U, V, DEF_U, DEF_V>, RTYPE>);
//
//          if(!has_error()){
//            return std::invoke(std::forward(func), m_value, m_status);
//          } else {
//            return { m_value, m_status };
//          }
//        }
//        /**
//         * @brief Invokes func parameter if current status has an error.
//         *
//         * @param func [in] rvalue reference to invoked function.
//         *
//         * @return Return Value Status class
//         * @note Expected function format parameter := [](value_type val, status_type stat) -> return_value_status { }
//         */
//        constexpr auto or_else(auto&& func) const noexcept -> return_value_status {
//          using U = std::remove_cvref_t<std::invoke_result_t<decltype(func), value_type, status_type>>;
//          static_assert(std::is_same_v<return_value_status, U>);
//
//          if(has_error()){
//            return std::invoke(std::forward(func), m_value, m_status);
//          } else { // has error
//            return { m_value, m_status };
//          }
//        }
//        /**
//         * @brief Indicates if the there is an error in the returned data.
//         * @return true := Return_Value_Status has error.
//         * @return false := Return_Value_Status has no error.
//         */
//        [[nodiscard]] constexpr bool has_error() const noexcept {
//          using U = std::underlying_type_t<status_type>;
//          return static_cast<U>(m_status) < 0;
//        }
//        /**
//         * @breif Bool operator, returns true if no error.
//         * @return true = No error, false = error.
//         */
//        explicit constexpr operator bool() const noexcept {
//          return !has_error();
//        }
//        /**
//         * @brief Comparison Equality operator for value_type.
//         * @param val [in] value_type value to compare against stored value.
//         * @return true := val equals stored value.
//         * @return false := val not equal stored value.
//         */
//        constexpr bool operator == (const value_type val) const noexcept {
//          return val == m_value;
//        }
//        /**
//         * @brief Comparison Equality operator for status_value.
//         * @param stat [in] status_type stat to compare against stored status.
//         * @return true := stat equals stored status.
//         * @return false := stat not equal to stored status.
//         */
//        constexpr bool operator == (const status_type stat) const noexcept {
//          return stat == m_status;
//        }
//        /**
//         * @brief Comparison Inequality operator for value_type.
//         * @param val [in] value_type value to compare against stored value.
//         * @return true := val not equal to stored value.
//         * @return false := val is equal to stored value.
//         */
//        constexpr bool operator != (const value_type val) const noexcept {
//          return m_value != val;
//        }
//        /**
//         * @brief Comparison Inequality operator for status_value.
//         * @param stat [in] status_type stat to compare against stored status.
//         * @return true := stat is not equal to stored status.
//         * @return false := stat is equal to stored status.
//         */
//        constexpr bool operator != (const status_type stat) const noexcept {
//          return m_status != stat;
//        }
//        /**
//         * @brief Gets returned value
//         * @return Returned value.
//         */
//        [[nodiscard]] constexpr value_type value() const noexcept {
//          return m_value;
//        }
//        /**
//         * @brief Gets Returned status
//         * @return Return status.
//         */
//        [[nodiscard]] constexpr status_type status() const noexcept {
//          return m_status;
//        }
//      private:
//        value_type m_value;
//        status_type m_status;
//    };

}

#endif //EMBEDDED_TEMPLATE_LIBRARY_EMBEDDED_RETURN_TYPE_HPP
