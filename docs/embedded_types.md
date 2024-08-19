# <u>Embedded Types</u>
[back](../README.md)
### File: [embedded_allocator.hpp](../embedded/inc/embedded_allocator.hpp)

### Namespace : embtl

## Types

- ___arch_type___ : Architecture type : defines the max bit width of the target architecture.
  - The architecture type can be defined use the following defines 
    - EMBEDDED_SYSTEM_8BIT : defines 8bit architecture type.
    - EMBEDDED_SYSTEM_16BIT : defines 16bit architecture type.
    - EMBEDDED_SYSTEM_64BIT : defines 64bit architecture type.
    - If none of thee above are defined the default is a 32bit architecture type.

- ___address_t___ : defines the address variable type of the system.
 
- ___STATUS___ (enum) : This enum is used to give the status of a function from a driver.

- ___IO_STATE___ (enum) : Used to represent the state of an IO pin (LOW, HIGH).

- __IO_DIRECTION__ (enum) : Used to represent the direction of an IO pin (INPUT, OUTPUT).

- __SIGNAL_EDGE__ (enum) : Used to represent the signal edge for detection (FALLING, RISING).


    