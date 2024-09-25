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
     */
    template<typename Policy, embedded_base_type Base = arch_type,
            Base Mask = std::numeric_limits<Base>::max(),
            Base Reset = std::numeric_limits<Base>::min()>
    struct basic_hardware_register final {
      public:
        using value_type = Base;
        using access_policy = Policy;
        using access_side_effect = policy::policy_side_effect_t<access_policy>;

        // Compile time checks
        /**
         * @brief Get the write mask for the register.
         * @return Write mask for the register.
         */
        static consteval value_type WriteMask() { return Mask; }
        /**
         * @brief Gets the register value for the register.
         * @return Reset value for the register.
         */
        static consteval value_type ResetValue() { return Reset; }
        /**
         * @brief Check if access policy allows reads.
         * @return true := Reads allowed; false := Reads not allowed.
         */
        static consteval bool has_read_access() noexcept { return mmio_register_policy_read_only<access_policy>; }
        /**
         * @brief Check if access policy allows writes.
         * @return true := Writes allowed; false : Writes not allowed.
         */
        static consteval bool has_write_access() noexcept { return mmio_register_policy_write_only<access_policy>; }
        /**
         * @brief Checks if access policy allows reads and writes.
         * @return true := Reads and Writes allowed; false := Read and Write access not allowed.
         */
        static consteval bool has_read_write_access() noexcept { return mmio_register_policy_read_write<access_policy>; }
        /**
         * @brief Checks if the register is reserved.
         * @return true := register is reversed; false := the register is not reserved.
         */
        static consteval bool is_reserved() noexcept {
          return !(mmio_register_policy_read_write<access_policy> ||
                  mmio_register_policy_read_only<access_policy> ||
                  mmio_register_policy_write_only<access_policy>);
        }
        /**
         * @brief Checks if register has side effects
         * @return true := side effect is implemented; false := no side effect is implemented.
         */
        static consteval bool has_side_effect() noexcept {
          return mmio_side_effect_write_only<access_side_effect> ||
                 mmio_side_effect_read_only<access_side_effect>; }

        basic_hardware_register() noexcept = default;

        explicit basic_hardware_register(const value_type value) noexcept
        requires mmio_register_policy_write_only<access_policy> : reg(value){ }

        basic_hardware_register(const basic_hardware_register&) noexcept = default;
        basic_hardware_register& operator=(const basic_hardware_register&) noexcept = default;
        basic_hardware_register(basic_hardware_register&&) noexcept = delete;
        basic_hardware_register& operator=(basic_hardware_register&&) noexcept = delete;

#ifdef UNIT_TEST
        static auto get_register(const basic_hardware_register& hw_reg) noexcept { return hw_reg.reg; }
        static auto set_register(basic_hardware_register& hw_reg, const value_type val) noexcept { hw_reg.reg = val; }

        void set_register(const value_type& val) noexcept { reg = val; }
        auto get_register() noexcept { return reg; }
#endif
        /**
         * @brief Reset register method.
         * @note Only available if register has write-only or read/write policy.
         */
        void reset() noexcept
        requires mmio_register_policy_write_only<access_policy>{
          access_policy::write(reg, Reset);
        }
        /**
         * @brief Read register method (const).
         * @return Value of register.
         * @note Only available if register has read-only or read/write policy.
         */
        [[nodiscard]] value_type read() const noexcept
        requires mmio_register_policy_read_only<access_policy> {
          return access_policy::read(reg);
        }
        /**
         * @brief Read register method with side effect.
         * @return Value of register.
         * @note Only available if register has read-only or read/write policy.
         */
        [[nodiscard]] value_type read() noexcept
        requires mmio_register_policy_read_only<access_policy> {
          return access_policy::read(reg);
        }
        /**
         * @brief Get bit field method (const).
         * @param pos [in] Field start bit position.
         * @param size [in] Field size in bits.
         * @param shifted [in] true := only get field value; false := get value of the masked field.
         * @return Field value.
         * @note Only available if register has read-only or read/write policy.
         */
        [[nodiscard]] value_type get_field(std::size_t pos, std::size_t size = 1, bool shifted = true) const noexcept
        requires mmio_register_policy_read_only<access_policy> {
          return access_policy::get_field(reg, pos, size, shifted);
        }
        [[nodiscard]] value_type get_field(std::size_t pos, std::size_t size = 1, bool shifted = true) noexcept
        requires mmio_register_policy_read_only<access_policy> {
          return access_policy::get_field(reg, pos, size, shifted);
        }
        /**
         * @brief Get bit field method with side effect.
         * @param pos [in] Field start bit position.
         * @param size [in] Field size in bits.
         * @param shifted [in] true := only get field value; false := get value of the masked field.
         * @return Field value.
         * @note Only available if register has read-only or read/write policy.
         */
        [[nodiscard]] value_type get_field(std::size_t pos, std::size_t size = 1, bool shifted = true) noexcept
        requires (mmio_register_policy_read_only<access_policy> && mmio_side_effect_read_only<access_side_effect>) {
          return access_policy::get_field(reg, pos, size, shifted);
        }
        /**
         * @brief Write register method.
         * @param value [in] Value to be written to register.
         * @note Only available if register has write-only or read/write policy.
         */
        void write(const value_type value) noexcept
        requires mmio_register_policy_write_only<access_policy> {
          access_policy::write(reg, value);
        }
        /**
         * @brief Set bit field method.
         * @param pos [in] Starting bit position of the field.
         * @param size [in] Size of bit field.
         * @param value [in] Convertable to value to device_register_t.
         * @note Only available if register has write-only or read/write policy.
         */
        void set_field(std::size_t pos, std::size_t size = 1, const value_type value = 1, bool shifted = false) noexcept
        requires mmio_register_policy_read_write<access_policy> {
          auto reg_value = this->read();    // Read register value.

          reg_value &= ~make_mask<value_type>(pos, size);                                     // clear field.
          auto masked_value = value & make_mask<value_type>(shifted ? pos : 0, size);     // mask value to field size

          if(!shifted){
            masked_value <<= pos;                                                             // Shift if value not already shifted
          }

          reg_value |= masked_value;                                                          // insert field into reg value

          this->write(reg_value);                                                       // Write set field back to register.
        }
        /**
         * @brief Clear bit field method.
         * @param pos [in] Starting bit position of the field.
         * @param size [in] Size of the bit field. (default = 1)
         * @note Only available if register has write-only or read/write policy.
         */
        void clear_field(std::size_t pos, std::size_t size = 1) noexcept
        requires mmio_register_policy_read_write<access_policy> {
          auto value = this->read();
          value &= ~make_mask<value_type>(pos, size);    // clear field.
          this->write(value);
        }

        // Assignment operators

        /**
         * @brief Assignment operator.
         * @param rhs [in] Convertable register types value to be written.
         * @return Reference to this class.
         * @note Only available if register has write-only or read/write policy.
         */
        basic_hardware_register& operator =(const value_type rhs) noexcept {
          this->write(rhs);
          return *this;
        }
        /**
         * @brief AND assignment operator.
         * @param rhs [in] Convertable register types mask value.
         * @return Reference to this class.
         * @note Only available if register has read/write policy.
         */
        basic_hardware_register& operator &=(const value_type rhs) noexcept {
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
        basic_hardware_register& operator |=(const value_type rhs) noexcept {
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
        basic_hardware_register& operator ^=(const value_type rhs) noexcept {
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
         * @brief AND bit-wise operator with side effect.
         * @param rhs [in] Convertable register types value
         * @return Result of operation.
         * @note Only available if register has read-only or read/write policy.
         */
        value_type operator & (const value_type rhs) noexcept {
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
         * @brief OR bit-wise operator with side effect.
         * @param rhs [in] Convertable register types value.
         * @return Result of operation.
         * @note Only available if register has read-only or read/write policy.
         */
        value_type operator | (const value_type rhs) noexcept {
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
        /**
         * @brief XOR bit-wise operator with side effect
         * @param rhs [in] Convertable register types value.
         * @return Result of Operation.
         * @note Only available if register has read-only or read/write policy.
         */
        value_type operator ^ (const value_type rhs) noexcept {
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
         * @brief Equality operator with side effect.
         * @param rhs [in] Convertable register types value to compared.
         * @return true := rhs is equal to register value; false := rhs not equal to register value.
         * @note Only available if register has read-only or read/write policy.
         */
        bool operator == (const value_type rhs) noexcept {
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
         * @brief Inequality operator with side effect.
         * @param rhs [in] Convertable register types value to be compared.
         * @return true := rhs is not equal to register value; false := rhs is equal to register value.
         * @note Only available if register has read-only or read/write policy.
         */
        bool operator != (const value_type rhs) noexcept {
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
         * @brief Greater than operator with side effect.
         * @param rhs [in] Convertable register types value to be compared.
         * @return true := rhs is greater than to register value; false := rhs is not greater than to register value.
         * @note Only available if register has read-only or read/write policy.
         */
        bool operator > (const value_type rhs) noexcept {
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
         * @brief Greater than or equal to operator with side effect.
         * @param rhs [in] Convertable register types value to be compared.
         * @return true := rhs is greater than or equal to register value; false := rhs is not greater than or equal to register value.
         * @note Only available if register has read-only or read/write policy.
         */
        bool operator >= (const value_type rhs) noexcept {
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
         * @brief Less than operator with side effect.
         * @param rhs [in] Convertable register types value to be compared.
         * @return true := rhs is less than to register value; false := rhs is not less than to register value.
         * @note Only available if register has read-only or read/write policy.
         */
        bool operator < (const value_type rhs) noexcept {
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
        /**
         * @brief Less than or equal to operator with side effect.
         * @param rhs [in] Convertable register types value to be compared.
         * @return true := rhs is less than or equal to register value; false := rhs is not less than or equal to register value.
         * @note Only available if register has read-only or read/write policy.
         */
        bool operator <= (const value_type rhs) noexcept {
          return this->read() <= rhs;
        }
      private:
        volatile value_type reg;
    };

    template<typename RegType>
    using register_access_policy = typename RegType::access_policy;
    template<typename RegType>
    using register_side_effect = typename RegType::access_side_effect;
    template<typename RegType>
    using register_base_type = typename RegType::value_type;

}

#endif //EMBEDDED_TL_EMBEDDED_REGISTER_HPP
