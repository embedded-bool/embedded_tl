/**
 * @file embedded_policy.hpp
 * @date 2024-08-07
 * @author Robert Morley
 *
 * @brief
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#ifndef EMBEDDED_TL_EMBEDDED_POLICY_HPP
#define EMBEDDED_TL_EMBEDDED_POLICY_HPP

#include <embedded_types.hpp>
#include <embedded_bits.hpp>
#include <embedded_concepts.hpp>

namespace embtl::policy {

    /**
     * @brief Device Register policy : root (common)
     * @tparam Base Register type.
     */
    template<embedded_base_type Base>
    struct basic_reg_root {
      protected:
        using value_type = Base;
        inline static constexpr auto make_mask(const std::size_t pos, const std::size_t size) noexcept -> value_type  {
          return embtl::make_mask<value_type>(pos, size);
        }
    };
    /**
     * @brief Register policy : read-only template
     * @tparam Base Register type.
     * @tparam SideEffects Used for unit testing, simulates the effects of read access to the register.
     */
    template<embedded_base_type Base, typename SideEffects = void>
    struct basic_reg_read_only : public basic_reg_root<Base> {
      public:
        using value_type = Base;
        using side_effect = SideEffects;
        /**
         * @brief Read-only policy : Read method.
         * @param reg [in] Constant reference to register variable.
         * @return Value of reg parameter.
         */
        static value_type read(volatile value_type& reg) noexcept {
          // Read register before side effect.
          auto reg_value = reg;
          // Implement side effect if template parameter SideEffect meets requirements
          if constexpr (mmio_side_effect_read_only<side_effect>){
            side_effect::read(reg);
          }
          // Return read value.
          return reg_value;
        }
        /**
         * @brief Read-only policy : get bit field
         * @param reg [in] Constant reference to register.
         * @param pos [in] Starting bit position of the field.
         * @param size [in] Size in bits of the field.
         * @param raw_value [in] true := Returns only masked value of the field. false (default) := Return value of the field.
         * @return Field data.
         */
        static value_type get_field(volatile value_type& reg, std::size_t pos, std::size_t size, bool shifted = true) noexcept {
          // 1. Read register
          auto reg_value = read(reg);
          // 2. Mask field
          reg_value &= basic_reg_root<Base>::make_mask(pos, size);
          // 3. Shift field to bit zero if requested.
          if (shifted) {
            reg_value >>= pos;
          }
          // 4. Return field value.
          return reg_value;
        }
    };
    static_assert(mmio_register_policy_read_only<basic_reg_read_only<arch_type>>);
    /**
     * @brief Register policy : write-only template.
     * @tparam Base Register value type.
     * @tparam Mask Write bit mask for register.
     * @tparam SideEffect Used for unit testing, simulates the effects of write access to the register.
     */
    template<embedded_base_type Base, Base Mask = std::numeric_limits<arch_type>::max(), typename SideEffect = void>
    struct basic_reg_write_only : public basic_reg_root<Base> {
      public:
        using value_type = Base;
        using side_effect = SideEffect;
        /**
         * @brief Write-only policy : Write method.
         * @tparam Mask Write Mask value.
         * @param reg [in] Reference to device register.
         * @param val [in] Value to be written to register.
//         */
        static void write(volatile value_type& reg, const value_type value) noexcept {
          reg = value & Mask;

          if constexpr (mmio_side_effect_write_only<side_effect>){
            side_effect::write(reg, value);
          }
        }
    };
    static_assert(mmio_register_policy_write_only<basic_reg_write_only<arch_type>>);
    /**
     * @brief Register policy : read/write template
     * @tparam Base Register value type.
     * @tparam Mask Write bit mask for register.
     * @tparam SideEffect Used for unit testing, simulates the effects of read/write access to the register.
     */
    template<embedded_base_type Base, Base Mask = std::numeric_limits<Base>::max(), typename SideEffect = void>
    struct basic_reg_read_write : public basic_reg_read_only<Base, SideEffect>, basic_reg_write_only<Base, Mask, SideEffect> {
      public:
        using value_type = Base;
        using reg_ro = basic_reg_read_only<Base>;
        using reg_wo = basic_reg_write_only<Base, Mask>;
        using side_effect = SideEffect;
        /**
         * @brief Write-only policy : Set bit field method.
         * @tparam Mask Write mask for register.
         * @param reg [in] Reference to device register.
         * @param val [in] Value to be written to the field.
         * @param pos [in] Starting bit position of the field.
         * @param size [in] Size of the field in bits. (default = 1)
         */
        static void set_field(volatile value_type& reg,
                              const std::size_t pos, const std::size_t size,
                              const value_type value,
                              bool shifted = false) noexcept{
          // Read register
          auto reg_v =reg_ro::read(reg);
          // Clear field
          reg_v &= ~basic_reg_root<Base>::make_mask(pos, size);
          // Set field value
          auto field_mask = basic_reg_root<Base>::make_mask(shifted ? pos : 0, size);

          if(shifted){
            reg_v |= value & field_mask;
          } else {
            reg_v |= (value & field_mask) << pos;
          }
          // Write back to register.
          reg_wo::write(reg, reg_v);
        }
        /**
         * @brief Write-only policy : Clear bit field.
         * @tparam Mask Write mask for register.
         * @param reg [in] Reference to the device register.
         * @param pos [in] Starting position of the field.
         * @param size [in] Size of the field. (default = 1)
         */
        static void clear_field(volatile value_type& reg,
                                const std::size_t pos, const std::size_t size) noexcept {
          // Read Register
          auto reg_v = basic_reg_read_only<Base>::read(reg);
          // Clear field
          reg_v &= ~basic_reg_root<Base>::make_mask(pos, size);
          // Write Register
          reg_wo::write(reg, reg_v);
        }
    };
    static_assert(mmio_register_policy_write_only<basic_reg_read_write<arch_type>>);
    static_assert(mmio_register_policy_read_only<basic_reg_read_write<arch_type>>);
    static_assert(mmio_register_policy_read_write<basic_reg_read_write<arch_type>>);

    template<embedded_base_type>
    struct basic_reg_reserved { };

    template<typename Policy>
    using policy_side_effect_t = typename Policy::side_effect;
}

#endif //EMBEDDED_TL_EMBEDDED_POLICY_HPP
