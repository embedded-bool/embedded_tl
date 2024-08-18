# <u>Embedded Region</u>
[back](../README.md)
### File: [embedded_region.hpp](../embedded/inc/embedded_region.hpp)

### Namespace : embtl

## <u>Description</u>

This file defines the device register memory region of the microcontrollers for memory mapped devices. The template
class basic_device_region is used to define that memory region.

## basic_device_region
```c++
template<typename RegisterMap, mmio_allocator DeviceAllocator>
requires (std::is_class_v<RegisterMap> && std::is_standard_layout_v<RegisterMap>)
struct basic_device_region final : public RegisterMap { ... };
```
### <u>Description</u>

This class is used to define a memory mapped device in the microcontroller. There are two template parameters, RegisterMap
and DeviceAllocator. The RegisterMap parameter is a class that is a standard layout that maps the memory structure of the
device registers. The DeviceAllocator parameter is used to allocate the register map to the correct address location to 
allow the driver to access them.

### <u>Template parameters</u>   
- RegisterMap : Device register map structure, must a class type and standard layout.
- DeviceAllocator : Memory mapped allocator for device.

### <u>Constructors</u>
#### Default
```c++
basic_device_region() noexcept = default;
```
#### Copy (default)
```c++
basic_device_region(const basic_device_region&) = default;
```
#### Move (deleted)
```c++
basic_device_region(basic_device_region&&) = delete;
```
### New and Delete operators
#### new operator
```c++
void* operator new(std::size_t sz) noexcept;
```

This operator new overrides the standard new call. This method will return a pointer to base address of the device register, 
which is given by the DeviceAlloc::allocate method.

#### delete operator
```c++
void operator delete (void* ptr) noexcept;
```

This operator delete overrides the standard delete call. This method essentially an empty method since no heap memory 
allocation was preformed.

## <u>Example</u>

The example below is used to demonstrate how driver development can be done using the basic_device_region template class.

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
    using usart_registers_t = embtl::basic_device_region<uart_register_map, uart_allocator<Number>>;
}

```
