# <u>Embedded Driver</u>
[back](../README.md)
### File: [embedded_driver.hpp](../embedded/inc/embedded_driver.hpp)

### Namespace : embtl

## basic_driver template class
```c++
template<typename DeviceIoRegion>
struct basic_driver;
```
- Template Parameter
  - DeviceIoRegion : basic_device_region template class should be used here.

### <u>Description</u>

This class is the base class for all drivers in a HAL library. This class holds the register map pointer for a device's
registers memory mapped in the system. The constructor allocates the device register map pointer through the new keyword,
if allocation returns a pointer to base address of the registers the driver is initialized without error. All registers 
are accessible to the inherited class through the reg_map pointer variable.

### <u>Constructors</u>
#### default
```c++
basic_driver() noexcept;
```

#### Copy (default)
```c++
basic_driver(const basic_driver&) noexcept = default;
```
#### Move (deleted)
```c++
basic_driver(basic_driver&&) noexcept = delete;
```

### <u>Methods</u>

#### has_error() 
```c++
[[nodiscard]] bool has_error() const noexcept;
```
- Returns
  - bool value : true := register map pointer is nullptr; false := register map pointer is allocated.

This method checks if the driver has an error. The driver is considered to have an error if the reg_map variable is 
nullptr and the method returns true, otherwise false.

### Members (protected)

- reg_map : pointer to DeviceIoRegion type.

## Example code

This example of how the basic_driver class could be implemented is a HAL library.

```c++
#include <concepts>
#include <embedded_register.hpp>
#include <embedded_allocator.hpp>
#include <embedded_region.hpp>

namespace hal {
    
    // 1. Define Register types for microcontroller.
    template<embtl::arch_type Mask = std::numeric_limits<embtl::arch_type>::max(),
             embtl::arch_type Reset = std::numeric_limits<embtl::arch_type>::min(),
             typename SideEffect = void>
    using mcu_reg_ro_t = embtl::basic_hardware_register<embtl::policy::basic_read_only<embtl::arch_type>, 
                                                        embtl::arch_type, Mask, Reset>;
    
    template<embtl::arch_type Mask = std::numeric_limits<embtl::arch_type>::max(),
             embtl::arch_type Reset = std::numeric_limits<embtl::arch_type>::min(),
             typename SideEffect = void>
    using mcu_reg_wo_t = embtl::basic_hardware_register<embtl::policy::basic_write_only<embtl::arch_type, Mask>, 
                                                        embtl::arch_type, Mask, Reset>;

    template<embtl::arch_type Mask = std::numeric_limits<embtl::arch_type>::max(),
             embtl::arch_type Reset = std::numeric_limits<embtl::arch_type>::min(),
             typename SideEffect = void>
    using mcu_reg_rw_t = embtl::basic_hardware_register<embtl::policy::basic_read_write<embtl::arch_type, Mask>, 
                                                        embtl::arch_type, Mask, Reset>;

    // 2. Define device register map, must not be final (using template defaults for simplicity)
    struct uart_register_map {
        public:
          mcu_reg_rw_t<> CTRL;
          mcu_reg_ro_t<> STAT;
          mcu_reg_wo_t<> TXD;
          mcu_reg_ro_t<> RXD;
          mcu_reg_rw_t<> BAUD;
    };
    static_assert(std::is_standard_layout_v<uart_register_map>);
    
    // 3. Define Device allocator class, for multiple uart devices
    template<std::size_t Number>
    requires (Number > 1)
    using uart_allocator = embtl::basic_hardware_allocator<Number, { 1, 0x4001'3800 }, { 2, 0x4000'4400 }, { 3, 0x4000'4800 }, { 4, 0x4000'4C00 }>;
        
    // 4. Define Device Region class type.
    template<std::size_t Number>
    using uart_registers_t = embtl::basic_device_region<uart_register_map, uart_allocator<Number>>;
    
    // 5. Define up driver template class.
    template<std::size_t Number>
    class uart_driver final : public embtl::basic_driver<uart_registers<Number>> { ... };
    
    // 6. (optional) create alias types for driver.
    using UART1_t = uart_driver<1>; 
    using UART2_t = uart_driver<2>; 
    using UART3_t = uart_driver<3>; 
    using UART4_t = uart_driver<4>;
```
