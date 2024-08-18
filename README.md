# Embedded Template Library (ETL)

## Brief

A basic template library to allow for easy development of Hardware
Abstraction Libraries (HAL) for embedded devices. Requires ___C++20___ support minimum.

## Description

This library contains basic types, templates, templated methods that can used help build a new HAL library 
for embedded devices that want to use the C++20 standard. Currently, the library is primarily used for building
hardware device drivers for the HAL library. Documentation for each library file is below. 

### Namespace
- embtl

## Header files
- [embedded_allocators.hpp](docs/embedded_allocator.md)
- [embedded_bits.hpp](docs/embedded_bits.md)
- [embedded_concepts.hpp](docs/embedded_concepts.md)
- [embedded_drivers.hpp](docs/embedded_driver.md)
- [embedded_policy.hpp](docs/embedded_policy.md)
- [embedded_region.hpp](docs/embedded_region.md)
- [embedded_register.hpp](docs/embedded_registers.md)
- [embedded_types.hpp](docs/embedded_types.md)
