/**
 * @file uut_embedded_policy.cpp
 * @date 2024-08-10
 * @author Robert Morley
 *
 * @brief Unit Test for Embedded Template Library header file "embedded_policy.hpp"
 *
 * @version v1.0.0
 * @copyright Copyright (c) 2024
 */
#include <uut_catch2.hpp>
#include <embedded_policy.hpp>

TEMPLATE_TEST_CASE("Register Access policy : Read-only","[embtl][template][policy][register][read-only][static]",
        (embtl::arch_type)
){
  using reg_ro_t = embtl::policy::basic_reg_read_only<TestType>;

  SECTION("Concept Tests"){
    STATIC_REQUIRE(embtl::mmio_register_policy_read_only<reg_ro_t>);
    STATIC_REQUIRE_FALSE(embtl::mmio_register_policy_write_only<reg_ro_t>);
    STATIC_REQUIRE_FALSE(embtl::mmio_register_policy_read_write<reg_ro_t>);
  }
  SECTION("Static Methods"){
    auto val = GENERATE(take(5, random(std::numeric_limits<TestType>::min(), std::numeric_limits<TestType>::max())));
    volatile TestType dev_reg { val };

    SECTION("read"){
      auto result = reg_ro_t::read(dev_reg);
      REQUIRE(result == val);
    }
    SECTION("get_field"){
      auto position = GENERATE(take(2, random(std::numeric_limits<TestType>::min(), std::numeric_limits<TestType>::digits - static_cast<TestType>(1))));
      auto size = GENERATE(take(2, random(static_cast<size_t>(0), static_cast<size_t>(8))));

      auto field_mask = embtl::make_mask<TestType>(position, size);
      TestType field = field_mask & val;

      SECTION("shifted") {
        auto result = reg_ro_t::get_field(dev_reg , position, size);
        REQUIRE(result == (field >> position));
      }
      SECTION("not-shifted"){
        auto result = reg_ro_t::get_field(dev_reg , position, size, false);
        REQUIRE(result == field);
      }
    }
  }
}

TEMPLATE_TEST_CASE_SIG("Register Access policy : Write-only","[embtl][template][policy][register][write-only][static]",
                      ((typename RegisterType, embtl::arch_type RegisterMask), RegisterType, RegisterMask),
                      (embtl::arch_type, std::numeric_limits<embtl::arch_type>::max()),
                      (embtl::arch_type, 0xFF00'0000),
                      (embtl::arch_type, 0x00FF'0000),
                      (embtl::arch_type, 0x0000'FF00),
                      (embtl::arch_type, 0x0000'00FF),
                      (embtl::arch_type, 0x03C0'7800)
){
  using reg_wo_t = embtl::policy::basic_reg_write_only<RegisterType, RegisterMask>;

  SECTION("Concept Tests"){
    STATIC_REQUIRE_FALSE(embtl::mmio_register_policy_read_only<reg_wo_t>);
    STATIC_REQUIRE(embtl::mmio_register_policy_write_only<reg_wo_t>);
    STATIC_REQUIRE_FALSE(embtl::mmio_register_policy_read_write<reg_wo_t>);
  }
  SECTION("Static Methods"){
    SECTION("write"){
      auto val = GENERATE(take(100, random(std::numeric_limits<RegisterType>::min(), std::numeric_limits<RegisterType>::max())));
      volatile RegisterType reg { 0 };

      reg_wo_t::write(reg, val);
      REQUIRE(reg == (val & RegisterMask));
    }
  }

}

TEMPLATE_TEST_CASE_SIG("Register Access policy : Read/Write","[embtl][template][policy][register][write-read][static]",
                       ((typename RegisterType, embtl::arch_type RegisterMask), RegisterType, RegisterMask),
                       (embtl::arch_type, std::numeric_limits<embtl::arch_type>::max()),
                       (embtl::arch_type, 0xFF00'0000),
                       (embtl::arch_type, 0x00FF'0000),
                       (embtl::arch_type, 0x0000'FF00),
                       (embtl::arch_type, 0x0000'00FF),
                       (embtl::arch_type, 0x03C0'7800)
){
  using reg_rw_t = embtl::policy::basic_reg_read_write<RegisterType, RegisterMask>;


  SECTION("Concept Tests"){
    STATIC_REQUIRE(embtl::mmio_register_policy_read_only<reg_rw_t>);
    STATIC_REQUIRE(embtl::mmio_register_policy_write_only<reg_rw_t>);
    STATIC_REQUIRE(embtl::mmio_register_policy_read_write<reg_rw_t>);
  }
  SECTION("Static Methods") {
    auto position = GENERATE(take(5, random(std::numeric_limits<std::size_t>::min(), std::numeric_limits<size_t>::digits - static_cast<std::size_t>(1))));
    auto gen_size = GENERATE(take(5, random(static_cast<std::size_t>(2), static_cast<std::size_t>(16))));
    std::size_t sz {gen_size };

    if(position + sz >= std::numeric_limits<std::size_t>::digits){
      sz = std::numeric_limits<std::size_t>::digits - position;
    }

    SECTION("Read Methods"){
      auto rd_val = GENERATE(take(5, random(std::numeric_limits<RegisterType>::min(), std::numeric_limits<RegisterType>::max())));
      volatile RegisterType rd_reg { rd_val };
      SECTION("read") {
        REQUIRE(reg_rw_t::read(rd_reg) == rd_reg);
      }
      SECTION("get_field") {
        auto mask = embtl::make_mask<RegisterType>(position, sz);
        auto field = (rd_reg & mask) >> position;
        REQUIRE(reg_rw_t::get_field(rd_reg, position, sz) == field);
      }
    }
    SECTION("Write Methods"){
      auto wr_value = GENERATE(take(5, random(std::numeric_limits<RegisterType>::min(), std::numeric_limits<RegisterType>::max())));
      volatile RegisterType wr_reg { };

      SECTION("write") {
        reg_rw_t::write(wr_reg, wr_value);
        REQUIRE(wr_reg == (wr_value & RegisterMask));
      }
    }
    SECTION("Read/Write Methods"){
      auto init_value = GENERATE(take(5, random(std::numeric_limits<RegisterType>::min(), std::numeric_limits<RegisterType>::max())));
      auto wr_value = GENERATE(take(5, random(std::numeric_limits<RegisterType>::min(), std::numeric_limits<RegisterType>::max())));

      SECTION("set_field") {
        volatile RegisterType rw_reg { init_value & RegisterMask};

        auto field_mask = embtl::make_mask<RegisterType>(position, sz);
        auto value_mask = embtl::make_mask<RegisterType>(0, sz);

        auto value_check { rw_reg & ~field_mask };
        value_check |= (wr_value & value_mask) << position;

        reg_rw_t::set_field(rw_reg, position, sz, wr_value );
        REQUIRE(rw_reg == (value_check & RegisterMask));
      }
      SECTION("clear_field") {
        volatile RegisterType rw_reg { init_value & RegisterMask};
        auto value_check { rw_reg & ~embtl::make_mask<RegisterType>(position, sz) };

        reg_rw_t::clear_field(rw_reg, position, sz);
        REQUIRE(rw_reg == value_check);
      }
    }
  }
}
