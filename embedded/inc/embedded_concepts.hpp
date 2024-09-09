/**
 * @file embedded_concepts.hpp
 * @date 2024-08-07
 * @author Robert Morley
 *
 * @brief 
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#ifndef EMBEDDED_TL_EMBEDDED_CONCEPTS_HPP
#define EMBEDDED_TL_EMBEDDED_CONCEPTS_HPP

#include <embedded_types.hpp>

namespace embtl {

    template<typename T>
    concept embedded_base_type = std::is_integral_v<T> &&
                                 std::is_unsigned_v<T> &&
                                 !std::is_same_v<T, bool>;

    template<typename T>
    concept mmio_register_policy_read_only = requires (
            const volatile arch_type& reg, std::size_t pos, std::size_t sz, bool shifted) {
      { T::read(reg) } -> std::same_as<arch_type>;
      { T::get_field(reg, pos, sz, shifted) } -> std::same_as<arch_type>;
    };

    template<typename T>
    concept mmio_register_policy_write_only = requires (volatile arch_type& reg, const arch_type value) {
      { T::write(reg, value) } -> std::same_as<void>;
    };

    template<typename T>
    concept mmio_register_policy_read_write = mmio_register_policy_read_only<T> && mmio_register_policy_write_only<T> &&
            requires (volatile arch_type& reg, std::size_t pos, std::size_t sz, arch_type value, bool masked) {
              { T::set_field(reg, pos, sz, value, masked) } -> std::same_as<void>;
              { T::clear_field(reg, pos, sz) } -> std::same_as<void>;
            };

    template<typename T>
    concept mmio_side_effect_read_only = requires (volatile arch_type& reg) {
      { T::read(reg) } -> std::same_as<void>;
    };

    template<typename T>
    concept mmio_side_effect_write_only = requires (volatile arch_type& reg, const arch_type& lvalue, arch_type&& rvalue) {
      { T::write(reg, lvalue) } -> std::same_as<void>;
      { T::write(reg, rvalue) } -> std::same_as<void>;
    };

    template<typename T>
    concept mmio_side_effect_read_write = mmio_side_effect_read_only<T> && mmio_side_effect_write_only<T> &&
            requires (volatile arch_type& reg, arch_type value, size_t pos, std::size_t size, bool masked) {
              { T::set_field(reg, pos, size, value, masked) } -> std::same_as<void>;
              { T::clear_field(reg, pos, size) } -> std::same_as<void>;
            };

    /**
     * @brief Memory Mapped IO (MMIO) Single Device concept.
     * @tparam T Type to be checked.
     */
    template<typename T>
    concept mmio_allocator = requires (std::size_t sz /*, void* obj */) {
      { T::allocate(sz) } -> std::same_as<void*>;
    };

    template<typename T>
    concept mmio_single_allocator = requires (std::size_t sz) {
      { T::allocate(sz) } -> std::same_as<void*>;
    };

    template<typename T>
    concept mmio_multi_allocator_alpha = requires (std::size_t sz, char n) {
      { T::allocate(sz,n) } -> std::same_as<void*>;
    };

    template<typename T>
    concept mmio_multi_allocator_numeric = requires (std::size_t sz, std::size_t n) {
      { T::allocate(sz,n) } -> std::same_as<void*>;
    };

    template<typename T>
    concept mmio_multi_allocator_alpha_numeric = mmio_multi_allocator_alpha<T> || mmio_multi_allocator_numeric<T>;

    template<typename T>
    concept io_pin_input = requires (T a, IO_STATE state) {
      { a.read() } -> std::same_as<IO_STATE>;
      { a == state };
      { a != state };
    };

    template<typename T>
    concept io_pin_output = requires (T a, IO_STATE state) {
      { a.write(state) } -> std::same_as<void>;
      { a = state };
      { a.toggle() } -> std::same_as<void>;
      { ~a };
    };

    template<typename T>
    concept io_pin_bidir = io_pin_input<T> && io_pin_output<T> &&
            requires (T a, IO_DIRECTION dir) {
              { a.direction(dir) } -> std::same_as<void>;
            };

}

#endif //EMBEDDED_TL_EMBEDDED_CONCEPTS_HPP
