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

using reg_ro_t = embtl::basic_hardware_register<embtl::policy::basic_reg_read_only<embtl::arch_type>>;
using reg_wo_t = embtl::basic_hardware_register<embtl::policy::basic_reg_write_only<embtl::arch_type>>;
using reg_rw_t = embtl::basic_hardware_register<embtl::policy::basic_reg_read_write<embtl::arch_type>>;

struct uart_registers_t {
  public:
    reg_rw_t  CTRL;
    reg_ro_t  STAT;
    reg_wo_t  TX;
    reg_ro_t  RX;
};

struct gpio_registers_t {
    reg_rw_t MODE;
    reg_rw_t OTYPE;
    reg_rw_t OSPEED;
    reg_rw_t PUPD;
    reg_ro_t IDR;
    reg_wo_t ODR;
    reg_rw_t BSR;
};

struct syscfg_register_t {
    reg_rw_t CR1;
    reg_rw_t CR2;
    reg_rw_t CR3;
    reg_rw_t CR4;
    reg_ro_t SR1;
    reg_ro_t SR2;
    reg_ro_t SR3;
    reg_ro_t SR4;
};

/**
 * @brief Unit Test for basic_mmio_device_registers<> template struct.
 */
TEMPLATE_TEST_CASE_SIG("basic_mmio_device_registers<> template numeric test","[embtl][template][multiple][device_registers]",
                       ((embtl::memory_mapped_device_info ... DeviceList), DeviceList...),
                       ({1_uz,0x4000'0000},{2_uz, 0x4000'1000},{3_uz, 0x4000'2000}),
                       ({1_uz,0x4000'0000},{2_uz, 0x4000'1000},{3_uz, 0x4000'2000},{4_uz, 0x4000'3000}),
                       ({'A', 0x4000'0000},{'B', 0x4100'0000}, {'C', 0x4000'0000})
){
  using uart_reg_allocator_t = embtl::basic_mmio_device_list_allocator<std::false_type, DeviceList...>;
  using uart_device_registers_t = embtl::basic_mmio_device_registers<uart_registers_t, uart_reg_allocator_t>;

  SECTION("Allocation Test"){
    constexpr std::array<embtl::memory_mapped_device_info, sizeof...(DeviceList)> device_list { DeviceList ...};

    for(auto& item : device_list){
      auto* device = new (item.number) uart_device_registers_t;

      REQUIRE_FALSE(device == nullptr);

      REQUIRE(device == reinterpret_cast<decltype(device)>(item.base_address));

      delete device;
    }
  }
}

TEMPLATE_TEST_CASE_SIG("basic_mmio_device_registers<> template single test","[embtl][template][single][device_registers]",
                       ((embtl::memory_mapped_device_info Device), Device),
                       ({0x4000'0000}),
                       ({0x4000'1000}),
                       ({0x4000'2000}),
                       ({0x4000'3000}),
                       ({0x4000'4000}),
                       ({0x4000'5000}),
                       ({0x4000'6000}),
                       ({0x4000'7000}),
                       ({0x4001'0000})
){
  using syscfg_allocator_t = embtl::basic_mmio_single_device_allocator<Device>;
  using syscfg_registers_t = embtl::basic_mmio_device_registers<syscfg_register_t, syscfg_allocator_t>;

  SECTION("Allocation Test"){
    auto* device = new syscfg_registers_t;

    REQUIRE_FALSE(device == nullptr);
    REQUIRE(device == reinterpret_cast<decltype(device)>(Device.base_address));

    delete device;
  }

}
