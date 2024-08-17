/**
 * @file uut_embedded_region.cpp
 * @date 2024-08-12
 * @author Robert Morley
 *
 * @brief
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#include <uut_catch2.hpp>
#include <embedded_allocator.hpp>
#include <embedded_register.hpp>
#include <embedded_region.hpp>

struct RegisterTestMap {
  public:
    embtl::basic_register_t<embtl::policy::basic_reg_read_write<embtl::arch_type>> CTRL;
    embtl::basic_register_t<embtl::policy::basic_reg_read_only<embtl::arch_type>>  STAT;
    embtl::basic_register_t<embtl::policy::basic_reg_write_only<embtl::arch_type>> TX;
    embtl::basic_register_t<embtl::policy::basic_reg_read_only<embtl::arch_type>>  RX;
};

struct TestAllocator {
  public:
    static void* allocate(std::size_t sz){
      return std::malloc(sz);
    }

    static void deallocate(void* ptr) {
      std::free(ptr);
    }
};

TEMPLATE_TEST_CASE_SIG("basic_device_region<> template test","[embtl][template][new][delete][basic_device_region]",
                       ((std::size_t Number, embtl::mmio_region_info ... DeviceInfoList), Number, DeviceInfoList...),
                       ( 1, { 1, 0x4000'0000 } ),
                       ( 3, { 1, 0x4000'0000 }, { 2, 0x4000'0400 }, { 3, 0x4000'0800}, { 4, 0x4000'1000}, { 5, 0x4000'1400} ),
                       ( 4, { 1, 0x4000'0000 }, { 2, 0x4000'0400 }, { 3, 0x4000'0800}, { 4, 0x4000'1000}, { 5, 0x4000'1400}  ),
                       ( 5, { 1, 0x4000'0000 }, { 2, 0x4000'0400 }, { 3, 0x4000'0800}, { 4, 0x4000'1000}, { 5, 0x4000'1400}  ),
                       ( 6, { 1, 0x4000'0000 }, { 2, 0x4000'0400 }, { 3, 0x4000'0800}, { 4, 0x4000'1000}, { 5, 0x4000'1400}  )
){

  SECTION("Allocate then Deallocate"){
    using register_map_t = embtl::basic_device_region<RegisterTestMap, embtl::basic_hardware_allocator<Number, DeviceInfoList...>>;
    auto ptr = new register_map_t;

    if constexpr(Number != 6){
      REQUIRE(ptr != nullptr);
    } else {
      REQUIRE_FALSE(ptr != nullptr);
    }

    delete ptr;
  }
  SECTION("Register Access test"){
    using register_map_t = embtl::basic_device_region<RegisterTestMap, TestAllocator>;

    auto wr_value = GENERATE(take(5, random(std::numeric_limits<embtl::arch_type>::min(), std::numeric_limits<embtl::arch_type>::max())));
    auto map = new register_map_t;

    REQUIRE(map != nullptr);

    map->CTRL = wr_value;
    REQUIRE(map->CTRL.read() == wr_value);

    delete map;
  }
}