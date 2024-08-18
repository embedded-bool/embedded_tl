/**
 * @file embedded_return_type.cpp
 * @date 2024-08-15
 * @author Robert Morley
 *
 * @brief
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#include <uut_catch2.hpp>
#include <embedded_return_type.hpp>

TEMPLATE_TEST_CASE_SIG("return_value_status<> test","[embtl][value][status][return][template][basic_return_value_status]",
                       ((typename ValueType, ValueType Default), ValueType, Default),
                       (std::uint32_t, 0),
                       (std::int32_t, -98),
                       (std::uint16_t, 23),
                       (std::int16_t, -230)
){
  using embtl::STATUS;
  using rvs_t = embtl::basic_return_value_status<ValueType, Default>;

  SECTION("Compile Time"){
    SECTION("Constructors"){
      constexpr ValueType  c_value = 42;
      constexpr embtl::STATUS c_status = embtl::STATUS::INITIALIZED;

      SECTION("Default"){
        constexpr rvs_t rvs { };
        STATIC_REQUIRE(rvs.get_value() == Default);
        STATIC_REQUIRE(rvs.get_status() == embtl::STATUS::OK);
      }
      SECTION("Initialize by Value"){
        constexpr rvs_t rvs { c_value };
        STATIC_REQUIRE(rvs.get_value() == c_value);
        STATIC_REQUIRE(rvs.get_status() == embtl::STATUS::OK);
      }
      SECTION("Initialize by Status"){
        constexpr rvs_t rvs { c_status };
        STATIC_REQUIRE(rvs.get_value() == Default);
        STATIC_REQUIRE(rvs.get_status() == c_status);
      }
      SECTION("Initialize by Value and Status"){
        constexpr rvs_t rvs { c_value, c_status };
        STATIC_REQUIRE(rvs.get_value() == c_value);
        STATIC_REQUIRE(rvs.get_status() == c_status);
      }
    }
    SECTION("Methods"){
      constexpr ValueType c_value2 = 69;
      constexpr embtl::STATUS c_status2 = embtl::STATUS::INITIALIZED;

      constexpr rvs_t rvs { c_value2, c_status2 };

      SECTION("Get methods"){
        STATIC_REQUIRE(rvs.get_value() == c_value2);
        STATIC_REQUIRE(rvs.get_status() == c_status2);
      }
      SECTION("Has methods"){
        STATIC_REQUIRE_FALSE(rvs.has_error());
        STATIC_REQUIRE(rvs.has_value());
      }
    }
    SECTION("Operators"){
      constexpr ValueType c_value3 = 37;
      constexpr embtl::STATUS c_status3 = embtl::STATUS::ERROR;

      SECTION("Equality"){
        constexpr rvs_t rvs { c_value3, c_status3 };
        STATIC_REQUIRE(rvs == c_status3);
        STATIC_REQUIRE(rvs == c_value3);
      }
      SECTION("Inequality"){
        constexpr rvs_t rvs { c_value3, c_status3 };
        CAPTURE(c_value3, c_status3, Default);
        STATIC_REQUIRE(rvs != Default);
        STATIC_REQUIRE(rvs != STATUS::OK);
      }
    }
  }
  SECTION("Runtime"){
    SECTION("Constructors"){
      auto value = GENERATE(take(10, random(std::numeric_limits<ValueType>::min(), std::numeric_limits<ValueType>::max())));
      auto status = GENERATE(STATUS::OK, STATUS::ERROR, STATUS::INITIALIZED, STATUS::INVALID_PARAMETER);

      SECTION("Default"){
        rvs_t rvs { };
        REQUIRE(rvs.get_value() == Default);
        REQUIRE(rvs.get_status() == embtl::STATUS::OK);
      }
      SECTION("Initialize by Value"){
        rvs_t rvs { value };
        REQUIRE(rvs.get_value() == value);
        REQUIRE(rvs.get_status() == embtl::STATUS::OK);
      }
      SECTION("Initialize by Status"){
        rvs_t rvs { status };
        REQUIRE(rvs.get_value() == Default);
        REQUIRE(rvs.get_status() == status);
      }
      SECTION("Initialize by Value and Status"){
        rvs_t rvs { value, status };
        REQUIRE(rvs.get_value() == value);
        REQUIRE(rvs.get_status() == status);
      }
    }
    SECTION("Methods"){
      auto value = GENERATE(take(10, random(std::numeric_limits<ValueType>::min(), std::numeric_limits<ValueType>::max())));
      auto status = GENERATE(STATUS::OK, STATUS::ERROR, STATUS::INITIALIZED, STATUS::INVALID_PARAMETER);

      rvs_t rvs { value, status };

      SECTION("Get methods"){
        REQUIRE(rvs.get_value() == value);
        REQUIRE(rvs.get_status() == status);
      }
      SECTION("Has methods"){
        if(static_cast<std::underlying_type_t<STATUS>>(status) < 0){
          REQUIRE(rvs.has_error());
          REQUIRE_FALSE(rvs.has_value());
        }else {
          REQUIRE_FALSE(rvs.has_error());
          REQUIRE(rvs.has_value());
        }
      }
      SECTION("invoke methods"){
        auto ret_value = GENERATE(take(3, random(std::numeric_limits<ValueType>::min(), std::numeric_limits<ValueType>::max())));
        auto ret_status = GENERATE(STATUS::BUSY, STATUS::BUFFER_OVERFLOW, STATUS::NOT_AVAILABLE);

        SECTION("and_then"){
          auto result = rvs.and_then([&](ValueType val, STATUS stat){
            return rvs_t { ret_value, ret_status };
          });

          if(rvs.has_value()){
            REQUIRE(result.get_value() == ret_value);
            REQUIRE(result.get_status() == ret_status);
          } else {
            REQUIRE(result.get_value() == value);
            REQUIRE(result.get_status() == status);
          }
        }
        SECTION("or_else"){
          auto result = rvs.or_else([&](ValueType val, STATUS stat){
            return rvs_t { ret_value, ret_status };
          });

          if(rvs.has_error()){
            REQUIRE(result.get_value() == ret_value);
            REQUIRE(result.get_status() == ret_status);
          } else {
            REQUIRE(result.get_value() == value);
            REQUIRE(result.get_status() == status);
          }
        }
      }
    }
    SECTION("Operators"){
      auto value = GENERATE(take(4, random(std::numeric_limits<ValueType>::min(), std::numeric_limits<ValueType>::max())));
      auto status = GENERATE(STATUS::BUSY, /*STATUS::ERROR,*/ STATUS::INITIALIZED, STATUS::INVALID_PARAMETER);

      SECTION("Assign"){
        rvs_t rvs { };
        rvs = status;
        REQUIRE(rvs.get_status() == status);
        rvs = value;
        REQUIRE(rvs.get_value() == value);
      }
      SECTION("Equality"){
        rvs_t rvs { value, status };
        REQUIRE(rvs == status);
        REQUIRE(rvs == value);
      }
      SECTION("Inequality"){
        rvs_t rvs { value, status };
        CAPTURE(value, status, Default);
        REQUIRE(rvs != Default);
        REQUIRE(rvs != STATUS::OK);
      }
    }
  }
}