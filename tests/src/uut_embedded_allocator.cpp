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
#include <array>
#include <uut_catch2.hpp>
#include <embedded_allocator.hpp>

/**
 * @brief Unit Test for find_base_address<>() template function.
 */
TEMPLATE_TEST_CASE_SIG("template find_base_address() function test.", "[embtl][function][template]",
                       ((embtl::memory_mapped_device_info ... DeviceList), DeviceList...),
                       ({0x4000'0000}),
                       ({1_uz,0x4000'0000},{2_uz,0x4000'1000},{3_uz,0x4000'2000},{4_uz,0x4000'3000}),
                       ({1_uz,0x4000'0000},{5_uz,0x4000'5000},{7_uz,0x4000'7000},{9_uz,0x4000'9000},{10_uz,0x4000'A000}),
                       ({1_uz,0x4000'1000},{2_uz,0x4000'2000},{3_uz,0x4000'3000})
){

  if constexpr (sizeof...(DeviceList) == 1){
    STATIC_REQUIRE_FALSE(embtl::find_base_address<DeviceList...>() == 0 );
    STATIC_REQUIRE(embtl::find_base_address<DeviceList...>() == 0x4000'0000 );
  }

  if constexpr (sizeof...(DeviceList) == 3){
    constexpr auto list = std::make_tuple(DeviceList...);

    STATIC_REQUIRE_FALSE(embtl::find_base_address<DeviceList...>(std::get<0>(list).number) == 0 );
    STATIC_REQUIRE(embtl::find_base_address<DeviceList...>(std::get<0>(list).number) == std::get<0>(list).base_address );

    STATIC_REQUIRE_FALSE(embtl::find_base_address<DeviceList...>(std::get<0>(list).number) == 0 );
    STATIC_REQUIRE(embtl::find_base_address<DeviceList...>(std::get<1>(list).number) == std::get<1>(list).base_address );

    STATIC_REQUIRE_FALSE(embtl::find_base_address<DeviceList...>(std::get<2>(list).number) == 0 );
    STATIC_REQUIRE(embtl::find_base_address<DeviceList...>(std::get<2>(list).number) == std::get<2>(list).base_address );
  }

  if constexpr (sizeof...(DeviceList) == 4){
    constexpr auto list = std::make_tuple(DeviceList...);

    STATIC_REQUIRE_FALSE(embtl::find_base_address<DeviceList...>(std::get<0>(list).number) == 0 );
    STATIC_REQUIRE(embtl::find_base_address<DeviceList...>(std::get<0>(list).number) == std::get<0>(list).base_address );

    STATIC_REQUIRE_FALSE(embtl::find_base_address<DeviceList...>(std::get<0>(list).number) == 0 );
    STATIC_REQUIRE(embtl::find_base_address<DeviceList...>(std::get<1>(list).number) == std::get<1>(list).base_address );

    STATIC_REQUIRE_FALSE(embtl::find_base_address<DeviceList...>(std::get<2>(list).number) == 0 );
    STATIC_REQUIRE(embtl::find_base_address<DeviceList...>(std::get<2>(list).number) == std::get<2>(list).base_address );

    STATIC_REQUIRE_FALSE(embtl::find_base_address<DeviceList...>(std::get<3>(list).number) == 0 );
    STATIC_REQUIRE(embtl::find_base_address<DeviceList...>(std::get<3>(list).number) == std::get<3>(list).base_address );
  }

  if constexpr (sizeof...(DeviceList) == 5){
    constexpr auto list = std::make_tuple(DeviceList...);

    STATIC_REQUIRE_FALSE(embtl::find_base_address<DeviceList...>(std::get<0>(list).number) == 0 );
    STATIC_REQUIRE(embtl::find_base_address<DeviceList...>(std::get<0>(list).number) == std::get<0>(list).base_address );

    STATIC_REQUIRE_FALSE(embtl::find_base_address<DeviceList...>(std::get<0>(list).number) == 0 );
    STATIC_REQUIRE(embtl::find_base_address<DeviceList...>(std::get<1>(list).number) == std::get<1>(list).base_address );

    STATIC_REQUIRE_FALSE(embtl::find_base_address<DeviceList...>(std::get<2>(list).number) == 0 );
    STATIC_REQUIRE(embtl::find_base_address<DeviceList...>(std::get<2>(list).number) == std::get<2>(list).base_address );

    STATIC_REQUIRE_FALSE(embtl::find_base_address<DeviceList...>(std::get<3>(list).number) == 0 );
    STATIC_REQUIRE(embtl::find_base_address<DeviceList...>(std::get<3>(list).number) == std::get<3>(list).base_address );

    STATIC_REQUIRE_FALSE(embtl::find_base_address<DeviceList...>(std::get<4>(list).number) == 0 );
    STATIC_REQUIRE(embtl::find_base_address<DeviceList...>(std::get<4>(list).number) == std::get<4>(list).base_address );
  }

}
/**
 * @brief Unit Test for basic_mmio_device_allocator<>() template function.
 */
TEMPLATE_TEST_CASE_SIG("template basic_mmio_device_allocator unit test","[embtl][function][template]",
                       ((embtl::memory_mapped_device_info ... DeviceList), DeviceList ...),
                       ({0x4000'0000}),
                       ({1_uz,0x5000'0000},{2_uz,0x5000'1000},{3_uz,0x5000'2000},{4_uz,0x5000'3000}),
                       ({1_uz,0x6000'0000},{5_uz,0x6000'5000},{7_uz,0x6000'7000},{9_uz,0x6000'9000},{10_uz,0x4000'A000}),
                       ({1_uz,0x7000'1000},{2_uz,0x7000'2000},{3_uz,0x7000'3000})
){
  constexpr std::array<embtl::memory_mapped_device_info, sizeof...(DeviceList)> device_list { DeviceList... };

  if constexpr (sizeof...(DeviceList) == 1){
    auto device_reg = embtl::basic_mmio_device_allocator<DeviceList...>();

    REQUIRE_FALSE(device_reg == nullptr);

    if constexpr(!embtl::host_allocation::value) {
      REQUIRE(device_reg == reinterpret_cast<void*>(device_list[0].base_address));
    }
  } else {
    for(auto& device : device_list) {
      auto device_reg = embtl::basic_mmio_device_allocator<DeviceList...>(device.number);

      CAPTURE(device.number, device.base_address, device_reg, sizeof...(DeviceList));
      REQUIRE_FALSE(device_reg == nullptr);

      if constexpr(!embtl::host_allocation::value) {
        REQUIRE(device_reg == reinterpret_cast<void*>(device.base_address));
      }
    }
  }
}
/**
 * @brief Unit for basic_mmio_device_list_allocator templated struct.
 */
TEMPLATE_TEST_CASE_SIG("template basic_mmio_device_list_allocator<> unit test","[embtl][template][struct]",
                       ((embtl::memory_mapped_device_info ... DeviceList), DeviceList ...),
                       ({1_uz,0x4000'0000},{2_uz,0x4000'1000}),
                       ({1_uz,0x5000'0000},{2_uz,0x5000'1000},{3_uz,0x5000'2000},{5_uz,0x5000'3000}),
                       ({1_uz,0x6000'0000},{2_uz,0x6000'5000},{3_uz,0x6000'7000},{4_uz,0x6000'9000},{5_uz,0x4000'A000}),
                       ({1_uz,0x7000'1000},{2_uz,0x7000'2000},{3_uz,0x7000'3000})
){
  using dev_alloc = embtl::basic_mmio_device_list_allocator<embtl::host_allocation, DeviceList...>;
  constexpr std::array<embtl::memory_mapped_device_info, sizeof...(DeviceList)> device_list { DeviceList... };

  for(auto& item : device_list){
    auto reg_ptr = dev_alloc::allocate(4_uz, item.number);
    REQUIRE_FALSE(reg_ptr == nullptr);

    if constexpr(!embtl::host_allocation::value){
      REQUIRE(reg_ptr == reinterpret_cast<void*>(item.base_address));
    } else {
      REQUIRE_FALSE(reg_ptr == reinterpret_cast<void*>(item.base_address));
    }
  }
}

/**
 * @brief Unit Test for basic_mmio_single_device_allocator<> template struct.
 */
TEMPLATE_TEST_CASE_SIG("template basic_mmio_single_device_allocator<> unit test","[embtl][template][struct]",
                       ((embtl::memory_mapped_device_info Device), Device),
                       ({0x4000'0000}),
                       ({0x5000'0000}),
                       ({0x6000'0000}),
                       ({0x7000'1000})
){

  SECTION("Hardware Allocation"){
    using dev_alloc = embtl::basic_mmio_single_device_allocator<Device, std::false_type>;
    auto reg_ptr = dev_alloc::allocate(4);
    REQUIRE_FALSE(reg_ptr == nullptr);

    REQUIRE(reg_ptr == reinterpret_cast<void*>(Device.base_address));
  }
  SECTION("Host Allocation"){
    using dev_alloc = embtl::basic_mmio_single_device_allocator<Device, std::true_type>;
    auto reg_ptr = dev_alloc::allocate(4);
    REQUIRE_FALSE(reg_ptr == nullptr);

    REQUIRE_FALSE(reg_ptr == reinterpret_cast<void*>(Device.base_address));
  }

}
