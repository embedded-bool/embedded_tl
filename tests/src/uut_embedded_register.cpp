/**
 * @file uut_embedded_register.cpp
 * @date 2024-08-11
 * @author Robert Morley
 *
 * @brief Unit Test : Embedded Template Library := basic_register_t<> template test.
 *
 * @version v1.0.0
 * @copyright Copyright (c) 2024
 */
#include <uut_catch2.hpp>
#include <embedded_register.hpp>

using register_type = embtl::arch_type;
constexpr auto DEFAULT_MASK { std::numeric_limits<register_type>::max() };
constexpr auto DEFAULT_RESET { std::numeric_limits<register_type>::min() };

TEMPLATE_TEST_CASE_SIG("Embedded Register Template test",
                       "[embtl][register][template]",
                       ((typename Policy, typename BaseType, register_type Mask, register_type Reset, typename SideEffect), Policy, BaseType, Mask, Reset, SideEffect),
                       (embtl::policy::basic_reg_read_only<register_type>, register_type, DEFAULT_MASK, DEFAULT_RESET, void),
                       (embtl::policy::basic_reg_write_only<register_type, DEFAULT_MASK>, register_type, DEFAULT_MASK, DEFAULT_RESET, void),
                       (embtl::policy::basic_reg_read_write<register_type, DEFAULT_MASK>, register_type, DEFAULT_MASK, DEFAULT_RESET, void)
){
  using reg_t = embtl::basic_register_t<Policy, BaseType, Mask, Reset>;

  SECTION("Compile Time Tests"){
    if constexpr (std::is_same_v<SideEffect, void>){
      STATIC_REQUIRE_FALSE(reg_t::has_side_effect());
    }
    // Read-only
    if constexpr (std::is_same_v<Policy, embtl::policy::basic_reg_read_only<BaseType>>){
      STATIC_REQUIRE(reg_t::is_read_only());
      STATIC_REQUIRE_FALSE(reg_t::is_write_only());
      STATIC_REQUIRE_FALSE(reg_t::is_read_write());
    }
    // Write-only check
    if constexpr (std::is_same_v<Policy, embtl::policy::basic_reg_write_only<BaseType, Mask>>){
      STATIC_REQUIRE_FALSE(reg_t::is_read_only());
      STATIC_REQUIRE(reg_t::is_write_only());
      STATIC_REQUIRE_FALSE(reg_t::is_read_write());
    }
    // Read/Write check
    if constexpr (std::is_same_v<Policy, embtl::policy::basic_reg_read_write<BaseType, Mask>>){
      STATIC_REQUIRE(reg_t::is_read_only());
      STATIC_REQUIRE(reg_t::is_write_only());
      STATIC_REQUIRE(reg_t::is_read_write());
    }
  }
  SECTION("Methods"){
    SECTION("Read-only methods"){
      if constexpr (embtl::mmio_register_policy_read_only<Policy>){
        auto rd_value = GENERATE(take(10, random(std::numeric_limits<BaseType>::min(), std::numeric_limits<BaseType>::max())));
        reg_t uut_reg { };
        reg_t::set_register(uut_reg, rd_value);

        SECTION("read method"){
          auto rd_check = uut_reg.read();
          REQUIRE(rd_check == (rd_value & Mask));
        }
        SECTION("get_field method"){
          auto position = GENERATE(take(5,random(std::numeric_limits<std::size_t>::min(), std::numeric_limits<BaseType>::digits - static_cast<std::size_t>(1))));
          auto gen_size = GENERATE(take(5,random(static_cast<std::size_t>(1), static_cast<std::size_t>(16))));

          std::size_t sz = gen_size;
          if((position + gen_size) > std::numeric_limits<BaseType>::digits){
            sz = std::numeric_limits<BaseType>::digits - position;
          }

          auto field_check = reg_t::get_register(uut_reg) & embtl::make_mask<BaseType>(position, sz);
          field_check >>= position;

          auto field = uut_reg.get_field(position, sz);
          CAPTURE(reg_t::get_register(uut_reg), position, gen_size, sz, field_check, field, std::numeric_limits<BaseType>::digits);
          REQUIRE(field == field_check);
        }
      }
    }
    SECTION("Write-only methods"){
      if constexpr (embtl::mmio_register_policy_write_only<Policy>){
        auto wr_value = GENERATE(take(10, random(std::numeric_limits<BaseType>::min(), std::numeric_limits<BaseType>::max())));
        reg_t uut_reg { wr_value };

        SECTION("reset method"){
          uut_reg.reset();
          REQUIRE(reg_t::get_register(uut_reg) == (Reset & Mask));
        }
        SECTION("write method"){
          uut_reg.write(wr_value);
          REQUIRE(reg_t::get_register(uut_reg) == (wr_value & Mask));
        }
      }
    }
    SECTION("Read/Write methods"){
      if constexpr (embtl::mmio_register_policy_read_write<Policy>){
        auto rw_value = GENERATE(take(10, random(std::numeric_limits<BaseType>::min(), std::numeric_limits<BaseType>::max())));
        auto field_value = GENERATE(take(10, random(std::numeric_limits<BaseType>::min(), std::numeric_limits<BaseType>::max())));
        auto position = GENERATE(take(5,random(std::numeric_limits<std::size_t>::min(), std::numeric_limits<BaseType>::digits - static_cast<std::size_t>(1))));
        auto gen_size = GENERATE(take(5,random(static_cast<std::size_t>(1), static_cast<std::size_t>(16))));

        reg_t uut_reg { rw_value };
        std::size_t sz = gen_size;

        if((position + gen_size) > std::numeric_limits<BaseType>::digits){
          sz = std::numeric_limits<BaseType>::digits - position;
        }

        SECTION("set_field method"){
          auto init_reg_value = reg_t::get_register(uut_reg);
          auto field = field_value & embtl::make_mask<BaseType>(0, sz);
          auto value_check = init_reg_value & ~embtl::make_mask<BaseType>(position, sz);
          value_check |= field << position;

          uut_reg.set_field(position, sz, field_value);
          CAPTURE(init_reg_value, position, gen_size, sz, field_value, field, value_check, reg_t::get_register(uut_reg), std::numeric_limits<BaseType>::digits);
          REQUIRE(reg_t::get_register(uut_reg) == value_check);
        }
        SECTION("clear_field method"){
          auto value_check = reg_t::get_register(uut_reg) & ~embtl::make_mask<BaseType>(position, sz);

          uut_reg.clear_field(position, sz);
          REQUIRE(reg_t::get_register(uut_reg) == value_check);
        }
      }
    }
  }
  SECTION("Operator Overloads"){
    SECTION("Read-only operators"){
      if constexpr (embtl::mmio_register_policy_read_only<Policy>){
        auto init_value = GENERATE(take(10, random(std::numeric_limits<BaseType>::min(), std::numeric_limits<BaseType>::max())));
        auto op_value = GENERATE(take(10, random(std::numeric_limits<BaseType>::min(), std::numeric_limits<BaseType>::max())));
        reg_t uut_reg { };
        reg_t::set_register(uut_reg, init_value);

        SECTION("Operators"){
          SECTION("AND"){
            auto result = uut_reg & op_value;
            REQUIRE(result == (init_value & op_value));
          }
          SECTION("OR"){
            auto result = uut_reg | op_value;
            REQUIRE(result == (init_value | op_value));
          }
          SECTION("XOR"){
            auto result = uut_reg ^ op_value;
            REQUIRE(result == (init_value ^ op_value));
          }
        }
        SECTION("Logical Comparisons"){
          auto reg_value = init_value & Mask;
          SECTION("Equal to"){
            if(reg_value == op_value){
              REQUIRE(uut_reg == op_value);
            } else {
              REQUIRE_FALSE(uut_reg == op_value);
            }
          }
          SECTION("Not Equal to"){
            if(reg_value != op_value){
              REQUIRE(uut_reg != op_value);
            } else {
              REQUIRE_FALSE(uut_reg != op_value);
            }
          }
          SECTION("Less than"){
            if(reg_value < op_value){
              REQUIRE(uut_reg < op_value);
            } else {
              REQUIRE_FALSE(uut_reg < op_value);
            }
          }
          SECTION("Less than or equal to"){
            if(reg_value <= op_value){
              REQUIRE(uut_reg <= op_value);
            } else {
              REQUIRE_FALSE(uut_reg <= op_value);
            }
          }
          SECTION("Greater than"){
            if(reg_value > op_value){
              REQUIRE(uut_reg > op_value);
            } else {
              REQUIRE_FALSE(uut_reg > op_value);
            }
          }
          SECTION("Greater than or equal to"){
            if(reg_value >= op_value){
              REQUIRE(uut_reg >= op_value);
            } else {
              REQUIRE_FALSE(uut_reg >= op_value);
            }
          }
        }
      }
    }
    SECTION("Write-only operators"){
      if constexpr (embtl::mmio_register_policy_write_only<Policy>){
        SECTION("Assignment Operator"){
          auto init_value = GENERATE(take(10, random(std::numeric_limits<BaseType>::min(), std::numeric_limits<BaseType>::max())));
          auto op_value = GENERATE(take(10, random(std::numeric_limits<BaseType>::min(), std::numeric_limits<BaseType>::max())));

          reg_t uut_reg { init_value };
          uut_reg = op_value;
          REQUIRE(reg_t::get_register(uut_reg) == (op_value & Mask));
        }
      }
    }
    SECTION("Read/Write operators"){
      if constexpr (embtl::mmio_register_policy_read_write<Policy>){
        auto init_value = GENERATE(take(10, random(std::numeric_limits<BaseType>::min(), std::numeric_limits<BaseType>::max())));
        auto op_value = GENERATE(take(10, random(std::numeric_limits<BaseType>::min(), std::numeric_limits<BaseType>::max())));

        reg_t uut_reg { init_value };

        SECTION("AND Assignment Operator"){
          auto check = reg_t::get_register(uut_reg);
          check &= op_value & Mask;
          uut_reg &= op_value;

          REQUIRE(reg_t::get_register(uut_reg) == check);
        }
        SECTION("OR Assignment Operator"){
          auto check = reg_t::get_register(uut_reg);
          check |= op_value & Mask;
          uut_reg |= op_value;

          REQUIRE(reg_t::get_register(uut_reg) == check);
        }
        SECTION("XOR Assignment Operator"){
          auto check = reg_t::get_register(uut_reg);
          check ^= op_value & Mask;
          uut_reg ^= op_value;

          REQUIRE(reg_t::get_register(uut_reg) == check);
        }
      }
    }
  }
}