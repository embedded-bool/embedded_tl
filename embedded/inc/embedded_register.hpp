/**
 * @file embedded_register.hpp
 * @date 2024-08-07
 * @author Robert Morley
 *
 * @brief Embedded Template Library : Device Register Template header file.
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#ifndef EMBEDDED_TL_EMBEDDED_REGISTER_HPP
#define EMBEDDED_TL_EMBEDDED_REGISTER_HPP

#include <embedded_types.hpp>
#include <embedded_policy.hpp>
#include <embedded_concepts.hpp>

namespace embtl {

    /**
     * @brief Device Register template.
     * @tparam Policy Access policy for the register ex: Read-only, Write-Only, Read/Write, or Reserved.
     * @tparam Base Register variable type.
     * @tparam Mask Register Write mask.
     * @tparam Reset Register reset value.
     * @tparam SideEffect Used for simulation the effects of read or write access. ( default = void; for no side effects or not simulating.)
     */
    template<typename Policy, embedded_base_type Base = arch_type,
            Base Mask = std::numeric_limits<Base>::max(),
            Base Reset = std::numeric_limits<Base>::min(),
            typename SideEffect = void>
    struct basic_register_t final {
      public:
        using value_type = Base;

        // Compile time checks
        static consteval bool is_read_only() noexcept { return mmio_register_policy_read_only<Policy>; }
        static consteval bool is_write_only() noexcept { return mmio_register_policy_write_only<Policy>; }
        static consteval bool is_read_write() noexcept { return mmio_register_policy_read_write<Policy>; }
        static consteval bool has_side_effect() noexcept { return mmio_side_effect_write_only<SideEffect> || mmio_side_effect_read_only<SideEffect>; }

        basic_register_t() noexcept = default;

        explicit basic_register_t(const value_type value) noexcept
        requires mmio_register_policy_write_only<Policy> : reg(value){ }

        basic_register_t(const basic_register_t&) noexcept = default;
        basic_register_t& operator=(const basic_register_t&) noexcept = default;
        basic_register_t(basic_register_t&&) noexcept = delete;
        basic_register_t& operator=(basic_register_t&&) noexcept = delete;

#ifdef UNIT_TEST
        static auto get_register(const basic_register_t& hw_reg) noexcept { return hw_reg.reg; }
        static auto set_register(basic_register_t& hw_reg, const value_type val) noexcept { hw_reg.reg = val & Mask; }
#endif
        /**
         * @brief Reset register method.
         * @note Only available if register has write-only or read/write policy.
         */
        void reset() noexcept
        requires mmio_register_policy_write_only<Policy>{
          Policy::write(reg, Reset);
          if constexpr (mmio_side_effect_write_only<SideEffect>) {
            SideEffect::write(reg, Reset);
          }
        }
        /**
         * @brief Read register method.
         * @return Value of register.
         * @note Only available if register has read-only or read/write policy.
         */
        [[nodiscard]] value_type read() const noexcept
        requires mmio_register_policy_read_only<Policy> {
          auto val = Policy::read(reg);
          // Compile Side effect if valid type.
          if constexpr (mmio_side_effect_read_only<SideEffect>){
            SideEffect::read(reg);
          }

          return val;
        }
        /**
         * @brief Get bit field method.
         * @param pos [in] Field start bit position.
         * @param size [in] Field size in bits.
         * @param shifted [in] true := only get field value; false := get value of the masked field.
         * @return Field value.
         * @note Only available if register has read-only or read/write policy.
         */
        [[nodiscard]] value_type get_field(std::size_t pos, std::size_t size = 1, bool shifted = true) const noexcept
        requires mmio_register_policy_read_only<Policy> {
          auto val =  Policy::get_field(reg, pos, size, shifted);

          if constexpr (mmio_side_effect_read_only<SideEffect>){
            SideEffect::read(reg);
          }

          return val;
        }
        /**
         * @brief Write register method.
         * @param value [in] Value to be written to register.
         * @note Only available if register has write-only or read/write policy.
         */
        void write(const value_type value) noexcept
        requires mmio_register_policy_write_only<Policy> {
          Policy::write(reg, value);

          if constexpr (mmio_side_effect_write_only<SideEffect>){
            SideEffect::write(reg, value);
          }
        }
        /**
         * @brief Set bit field method.
         * @param pos [in] Starting bit position of the field.
         * @param size [in] Size of bit field.
         * @param value [in] Convertable to value to device_register_t.
         * @note Only available if register has write-only or read/write policy.
         */
        void set_field(std::size_t pos, std::size_t size = 1, const value_type value = 1, bool shifted = false) noexcept
        requires mmio_register_policy_read_write<Policy> {
          Policy::set_field(reg, pos, size, value, shifted);

          if constexpr (mmio_side_effect_read_write<SideEffect>){
            SideEffect::set_field(reg, pos, size, value, shifted);
          }
        }
        /**
         * @brief Clear bit field method.
         * @param pos [in] Starting bit position of the field.
         * @param size [in] Size of the bit field. (default = 1)
         * @note Only available if register has write-only or read/write policy.
         */
        void clear_field(std::size_t pos, std::size_t size = 1) noexcept
        requires mmio_register_policy_read_write<Policy> {
          Policy::clear_field(reg, pos, size);

          if constexpr (mmio_side_effect_read_write<SideEffect>){
            SideEffect::clear_field(reg, pos, size);
          }
        }

        // Assignment operators

        /**
         * @brief Assignment operator.
         * @param rhs [in] Convertable register types value to be written.
         * @return Reference to this class.
         * @note Only available if register has write-only or read/write policy.
         */
        basic_register_t& operator =(const value_type rhs) noexcept {
          this->write(rhs);
          return *this;
        }
        /**
         * @brief AND assignment operator.
         * @param rhs [in] Convertable register types mask value.
         * @return Reference to this class.
         * @note Only available if register has read/write policy.
         */
        basic_register_t& operator &=(const value_type rhs) noexcept {
          auto reg_v = this->read();
          reg_v &= rhs;
          this->write(reg_v);
          return *this;
        }
        /**
         * @brief OR assignment operator.
         * @param rhs [in] Convertable register types value
         * @return Reference to this class.
         * @note Only available if register has read/write policy.
         */
        basic_register_t& operator |=(const value_type rhs) noexcept {
          auto reg_v = this->read();
          reg_v |= rhs;
          this->write(reg_v);
          return *this;
        }
        /**
         * @brief XOR Assignment operator.
         * @param rhs [in] Convertable register types value
         * @return Reference to this class.
         * @note Only available if register has read/write policy.
         */
        basic_register_t& operator ^=(const value_type rhs) noexcept {
          auto reg_v = this->read();
          reg_v ^= rhs;
          this->write(reg_v);
          return *this;
        }

        // Bitwise Operators

        /**
         * @brief AND bit-wise operator.
         * @param rhs [in] Convertable register types value
         * @return Result of operation.
         * @note Only available if register has read-only or read/write policy.
         */
        value_type operator & (const value_type rhs) const noexcept {
          return this->read() & rhs;
        }
        /**
         * @brief OR bit-wise operator.
         * @param rhs [in] Convertable register types value.
         * @return Result of operation.
         * @note Only available if register has read-only or read/write policy.
         */
        value_type operator | (const value_type rhs) const noexcept {
          return this->read() | rhs;
        }
        /**
         * @brief XOR bit-wise operator.
         * @param rhs [in] Convertable register types value.
         * @return Result of Operation.
         * @note Only available if register has read-only or read/write policy.
         */
        value_type operator ^ (const value_type rhs) const noexcept {
          return this->read() ^ rhs;
        }

        // Comparison operators

        /**
         * @brief Equality operator.
         * @param rhs [in] Convertable register types value to compared.
         * @return true := rhs is equal to register value; false := rhs not equal to register value.
         * @note Only available if register has read-only or read/write policy.
         */
        bool operator == (const value_type rhs) const noexcept {
          return this->read() == rhs;
        }
        /**
         * @brief Inequality operator.
         * @param rhs [in] Convertable register types value to be compared.
         * @return true := rhs is not equal to register value; false := rhs is equal to register value.
         * @note Only available if register has read-only or read/write policy.
         */
        bool operator != (const value_type rhs) const noexcept {
          return this->read() != rhs;
        }
        /**
         * @brief Greater than operator.
         * @param rhs [in] Convertable register types value to be compared.
         * @return true := rhs is greater than to register value; false := rhs is not greater than to register value.
         * @note Only available if register has read-only or read/write policy.
         */
        bool operator > (const value_type rhs) const noexcept {
          return this->read() > rhs;
        }
        /**
         * @brief Greater than or equal to operator.
         * @param rhs [in] Convertable register types value to be compared.
         * @return true := rhs is greater than or equal to register value; false := rhs is not greater than or equal to register value.
         * @note Only available if register has read-only or read/write policy.
         */
        bool operator >= (const value_type rhs) const noexcept {
          return this->read() >= rhs;
        }
        /**
         * @brief Less than operator.
         * @param rhs [in] Convertable register types value to be compared.
         * @return true := rhs is less than to register value; false := rhs is not less than to register value.
         * @note Only available if register has read-only or read/write policy.
         */
        bool operator < (const value_type rhs) const noexcept {
          return this->read() < rhs;
        }
        /**
         * @brief Less than or equal to operator.
         * @param rhs [in] Convertable register types value to be compared.
         * @return true := rhs is less than or equal to register value; false := rhs is not less than or equal to register value.
         * @note Only available if register has read-only or read/write policy.
         */
        bool operator <= (const value_type rhs) const noexcept {
          return this->read() <= rhs;
        }
      private:
        volatile value_type reg;

    };

}

#endif //EMBEDDED_TL_EMBEDDED_REGISTER_HPP
