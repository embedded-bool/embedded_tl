/**
 * @file embedded_actions.hpp
 * @date 2024-10-05
 * @author Robert Morley
 *
 * @brief
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#ifndef EMBEDDED_TL_EMBEDDED_ACTIONS_HPP
#define EMBEDDED_TL_EMBEDDED_ACTIONS_HPP

#include <embedded_types.hpp>
#include <embedded_return_type.hpp>

namespace embtl {



    /**
     * @brief Used to check if the type has a constructor initializer that takes STATUS as a parameter.
     * @tparam T Type parameter.
     *
     * @note Unit Tested
     */
    template<typename T>
    concept status_initializer = !std::is_same_v<T, void> && requires (STATUS stat) {
      T{stat};
    };

    /**
     * @brief Used to check if the type is STATUS.
     * @tparam T Type parameter.
     */
    template<typename T>
    concept is_status = std::is_same_v<STATUS, T>;

    /**
     * @brief Used to check if the return type of the lambda for is a valid return type.
     * @tparam T Type parameter.
     *
     * @note Unit Tested
     */
    template<typename T>
    concept action_return_type = is_status<T> || status_initializer<T> || std::is_same_v<T,void>;

    /**
     * @brief Used to check if the lambda given to status_action_if method will return a value type.
     * @tparam T Type parameter.
     *
     * @note Unit Tested
     */
    template<typename T>
    concept action_can_return = (is_status<T> || status_initializer<T>) && !std::is_same_v<T,void>;

    /**
     * @brief The templated method is used to execute a lambda if the cond parameter is true.
     *
     * @param cond This the condition parameter and must be true for the lambda to be executed.
     * @param action Lambda function to be executed if the cond parameter is true.
     * @return Returns an error condition if cond parameter is false. Otherwise the return value of the lambda is returned.
     *
     * @details
     * This templated method is used for driver actions that requires a STATUS or status type like basic_return_value_status
     * template class as a return type. For STATUS only returns if the cond parameter is false a STATUS::ERROR will be
     * returned. For return types that have a constructor initializer that takes STATUS as a parameter will return that
     * type with a STATUS::ERROR. If the condition is true, and the lambda function returns a void type then an STATUS::OK
     * is returned after the lambda is finished executing. If the lambda's return type is not void than the resulting
     * return type is returned by the function.
     *
     * @example
     * Driver implementation examples.
     *
     * @code{.cpp}
     *
     * // Return type example for STATUS only.
     * STATUS enable() noexcept {
     *    return status_action_if(!has_error(), [&]()->void {
     *        // Place driver code here.
     *        // return type is void so the return will be either STATUS::OK or STATUS::ERROR.
     *    });
     * }
     *
     * // Return type example with a constructor initializer that takes STATUS.
     * base_rvs_t get_count() noexcept {
     *    return status_action_if(!has_error(), [&]() -> base_rvs_t {
     *        // Place driver coder here
     *        return base_rvs_t { ... };
     *    });
     * }
     *
     * @endcode
     *
     * @note Unit Tested
     */
    auto status_action_if(bool cond, auto&& action, auto&& ... args) noexcept
    requires action_return_type<decltype(action(args...))> {
      using return_type = decltype(action(args...));

      if(!cond){
        // If return_type has a STATUS constructor initializer.
        if constexpr(status_initializer<return_type>) {
          return return_type { STATUS::ERROR };
        } else {
          // All other types
          return STATUS::ERROR;
        }
      } else {
        if constexpr (action_can_return<return_type>){
          return action(std::forward(args...));
        } else {
          action(std::forward(args...));
          return STATUS::OK;
        }
      }
    }

    auto status_action_if(bool cond, auto&& action, auto& ... args) noexcept
    requires action_return_type<decltype(action(args...))> {
      using return_type = decltype(action(args...));

      if(!cond){
        // If return_type has a STATUS constructor initializer.
        if constexpr(status_initializer<return_type>) {
          return return_type { STATUS::ERROR };
        } else {
          // All other types
          return STATUS::ERROR;
        }
      } else {
        if constexpr (action_can_return<return_type>){
          return action(args...);
        } else {
          action(args...);
          return STATUS::OK;
        }
      }
    }

    auto status_action_if(bool cond, auto&& action) noexcept
    requires action_return_type<decltype(action())> {
      using return_type = decltype(action());

      if(!cond){
        // If return_type has a STATUS constructor initializer.
        if constexpr(status_initializer<return_type>) {
          return return_type { STATUS::ERROR };
        } else {
          // All other types
          return STATUS::ERROR;
        }
      } else {
        if constexpr (action_can_return<return_type>){
          return action();
        } else {
          action();
          return STATUS::OK;
        }
      }
    }

}


#endif //EMBEDDED_TL_EMBEDDED_ACTIONS_HPP
