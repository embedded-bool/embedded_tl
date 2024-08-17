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
     */
    template<embedded_base_type Base>
    struct basic_reg_read_only : public basic_reg_root<Base> {
      public:
        using value_type = Base;
        /**
         * @brief Read-only policy : Read method.
         * @param reg [in] Constant reference to register variable.
         * @return Value of reg parameter.
         */
        static value_type read(const volatile value_type& reg) noexcept {
          return reg;
        }
        /**
         * @brief Read-only policy : get bit field
         * @param reg [in] Constant reference to register.
         * @param pos [in] Starting bit position of the field.
         * @param size [in] Size in bits of the field.
         * @param raw_value [in] true := Returns only masked value of the field. false (default) := Return value of the field.
         * @return Field data.
         */
        static value_type get_field(const volatile value_type& reg, std::size_t pos, std::size_t size, bool shifted = true) noexcept {
          // 1. Read register
          auto reg_v = read(reg);
          // 2. Mask field
          reg_v &= basic_reg_root<Base>::make_mask(pos, size);
          // 3. Shift field to bit zero if requested.
          if (shifted) {
            reg_v >>= pos;
          }
          // 4. Return field value.
          return reg_v;
        }
    };
    static_assert(mmio_register_policy_read_only<basic_reg_read_only<arch_type>>);

    /**
     * @brief Register policy : write-only template.
     * @tparam Base Register type.
     */
    template<embedded_base_type Base, Base Mask = std::numeric_limits<arch_type>::max()>
    struct basic_reg_write_only : public basic_reg_root<Base> {
      public:
        using value_type = Base;
        /**
         * @brief Write-only policy : Write method.
         * @tparam Mask Write Mask value.
         * @param reg [in] Reference to device register.
         * @param val [in] Value to be written to register.
//         */
        static void write(volatile value_type& reg, const value_type value) noexcept {
          reg = value & Mask;
        }
    };
    static_assert(mmio_register_policy_write_only<basic_reg_write_only<arch_type>>);

    /**
     * @brief Register policy : read/write template
     * @tparam Base Register type.
     */
    template<embedded_base_type Base, Base Mask = std::numeric_limits<Base>::max()>
    struct basic_reg_read_write : public basic_reg_read_only<Base>, basic_reg_write_only<Base, Mask> {
      public:
        using value_type = Base;
        using reg_ro = basic_reg_read_only<Base>;
        using reg_wo = basic_reg_write_only<Base, Mask>;
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
}

#endif //EMBEDDED_TL_EMBEDDED_POLICY_HPP
