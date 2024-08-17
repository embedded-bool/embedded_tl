/**
 * @file uut_embedded_allocator.cpp
 * @date 2024-08-12
 * @author Robert Morley
 *
 * @brief Unit Test : Embedded Template Library := Template Multiple/Single device allocator classes.
 *
 * @version v1.0.0
 * @copyright Copyright (c) 2024
 */
#include <uut_catch2.hpp>
#include <embedded_allocator.hpp>

TEMPLATE_TEST_CASE_SIG("MMIO Hardware Allocator template test : single",
                       "[embtl][template][single][allocate][basic_hardware_allocator]",
                       ((embtl::mmio_region_info DeviceInfo), DeviceInfo),
                       ({ 1, 0x4000'0000 }),
                       ({ 2, 0x4000'0400 }),
                       ({ 3, 0x4000'0800 }),
                       ({ 4, 0x4000'0C00 }),
                       ({ 5, 0x4000'1000 })
){
  using alloc_t = embtl::basic_hardware_allocator<DeviceInfo.number, DeviceInfo>;

  SECTION("Device Address allocator"){
    REQUIRE(alloc_t::allocate(8) != nullptr);
  }
}

TEMPLATE_TEST_CASE_SIG("MMIO Hardware Allocator template test : multiple",
                       "[embtl][template][multiple][allocate][basic_hardware_allocator]",
                       ((std::size_t Number, embtl::mmio_region_info ... DeviceList), Number, DeviceList ...),
                       (1, {1, 0xE000'0000}, { 2, 0xD000'0000 }, { 3, 0xED00'0000 }),
                       (2, {1, 0xE000'0000}, { 2, 0xD000'0000 }, { 3, 0xED00'0000 }),
                       (3, {1, 0xE000'0000}, { 2, 0xD000'0000 }, { 3, 0xED00'0000 }),
                       (5, {1, 0xE000'0000}, { 2, 0xD000'0000 }, { 3, 0xED00'0000 })
){
    using alloc_t = embtl::basic_hardware_allocator<Number, DeviceList ...>;

    SECTION("Device Address Allocator"){
      auto ptr = alloc_t::allocate(5);
      if constexpr(Number == 5){
        REQUIRE(ptr == nullptr);
      } else {
        REQUIRE(ptr != nullptr);
      }
    }
}
