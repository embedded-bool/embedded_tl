# <u>Embedded Allocators</u>
[back](../README.md)
### File: [embedded_allocator.hpp](../embedded/inc/embedded_allocator.hpp)

## Namespace : embtl

## Description
<p>
This file contains classes to create custom allocators for embedded hardware.
There are 2 classes that are used a mmio_region_info, and basic_hardware_allocator template.
The mmio_region_info class is used to define the unit number and base address of the
hardware registers. The basic_hardware_allocator template class is used return a pointer
to the base address of the device hardware registers.
</p> 

## mmio_region_info class
### <u>Description</u>

This class is used to define individual devices unit number and base address for the hardware 
registers. The user can set just the base address or device and the base address. There is no
default constructor for this class. This class is primarily used to define single and multiple
device hardware register locations in from the custom allocator required for the 
[basic_device_region](embedded_region.md) class. 

### <u>Constructors</u>
- No default constructor.
- Initialize by base address. Device Number is set to 0 by default and base address is set by user.
  ```cpp
  mmio_region_info device_info { 0x4000'0000 };
  ```
- Initialize by Device Number and base address.
  ```cpp
  mmio_region_info device_info { 5, 0x4000'C000 };
  ```
  
### <u>Members (public)</u>
- std::size_t number := Device ID number.
  - ex: SPI1 = 1, I2C3 = 3, ... etc.
- address_t base_address := Registers Base Address
  - ex: SPI1_BASE = 0x4000'0000

## base_hardware_allocator template class.

### <u>Description</u>

This template class is used to allocate a pointer to device hardware based on the Number given
to the template parameter and the associated device list parameter(s). There must be at least
1 mmio_region_info in the device list or a compile error will be reported.

### <u>Template Parameters</u>

- Number : Device Number to allocate
- DeviceList ... (mmio_region_info) : List of Device numbers for specific hardware type.

### <u>Static Methods</u>

#### allocate
  ```cpp
    static void* allocate(std::size_t sz) noexcept;
  ```
  This static method returns a void pointer to the base address of device number indicated in
  Number template parameter, if the device number cannot be found in the DeviceList template 
  parameter then a nullptr is returned.

#### deallocate
  ```cpp
    static void deallocate(void* ptr) noexcept;
  ```
  Since the allocate method does not actually allocate system memory this an empty method.

### <u>Example</u>

This examples show how the basic_hardware_allocator template class can be used for multiple device allocation.

```cpp
    template<std::size_t Number>
    using spi_allocator = basic_hardware_allocator<Number, {1, 0x5000'0000 }, {2, 0x5000'1000 }, {3, 0x5000'2000 }>
    
    using spi1_alloc_t = spi_allocator<1>;
    using spi2_alloc_t = spi_allocator<2>;
    using spi3_alloc_t = spi_allocator<3>;
```
