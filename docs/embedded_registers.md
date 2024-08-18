# <u>Embedded Registers</u>
[back](../README.md)
### File: [embedded_register.hpp](../embedded/inc/embedded_register.hpp)

### Namespace : embtl

## Description

This file defines a basic hardware registers template class for general register operations. Concepts are used in conjunction
with policy classes to determine if access is valid or not at compile time. The concepts used are defined in 
[embedded_concepts.hpp](../embedded/inc/embedded_concepts.hpp), information about these concepts can be found in the 
[embedded_concepts](embedded_concepts.md) document file.

## basic_hardware_register
```c++
template<typename Policy, embedded_base_type Base = arch_type,
         Base Mask = std::numeric_limits<Base>::max(),
         Base Reset = std::numeric_limits<Base>::min(),
         typename SideEffect = void>
struct basic_hardware_register final { ... };
```

### <u>Description</u>

This class is used to define device registers that are internal to the microcontroller system. The template parameters are
used to set the access policy, write mask, and reset value of the hardware register. In addition, for test and simulation
the SideEffect template parameter can be used emulate additional actions the register takes during read or write accesses. 
If the register side effects are not needed for the type can be set to void. This is done by default.

### <u>Template Parameters</u>
- Policy : Register access type, read-only, write-only, read/write, and reserved. Policy must follow policy concepts.
- Base : Register base type, 8bit, 16bit, 32bit, or 64bit, default is arch_type.
- Mask : Write bit mask, used to determine which bytes on a register write are stored. Default is all bits masked.
- Reset : Reset value of the register. Only used on write-only and read/write access policies. Default is 0.
- SideEffect : Use for unit testing and simulation. Can be used define behavior of the register during read and write access. 
  Default is void, which signifies no side effect needed.

### <u>Types</u>
- value_type : Base template parameter type.

### <u>Constructors</u>

#### Default
```c++
basic_hardware_register() noexcept;
```

Default constructor used by all policies, no initialization of the register value.

#### Initializer
```c++
explicit basic_hardware_register(const value_type value) noexcept
```

Initialization constructor used to set the value of the register. This constructor can only be used for write-only and
read/write access policies.

#### Copy
```c++
basic_hardware_register(const basic_hardware_register&) noexcept = default;
```

Default copy used for this class.

#### Move
```c++
basic_hardware_register(basic_hardware_register&&) noexcept = delete;
```
Move construct is deleted.

### <u>static consteval methods</u>

#### has_read_access()
```c++
static consteval bool has_read_access() noexcept;
```

Indicates if the register has read access, if true user can read from register, otherwise user cannot use 
read methods.

#### has_write_access()
```c++
static consteval bool has_write_access() noexcept;
```

Indicates if the register has write access, if true user can write to register, otherwise user cannot use
write methods.

#### has_read_write_access()
```c++
static consteval bool has_read_write_access() noexcept;
```

Indicates if the register has read/write access, if true user can read/write from/to register, otherwise user cannot use
read/write methods.

#### is_reserved()
```c++
static consteval bool is_reserved() noexcept;
```

Indicates if the register is reserved, if true user cannot access register with any method, otherwise user check use 
other methods to check access policy.

#### has_side_effect()
```c++
static consteval bool has_side_effect() noexcept;
```

### <u>Methods (public)</u>

#### reset()
```c++
void reset() noexcept;
```
- Parameters
  - NONE
- Returns
  - void

This method requires a write-only or read/write policy to be used. This method sets the writes the reset 
value in the Reset template parameter.

#### read()
```c++
[[nodiscard]] value_type read() const noexcept;
```
- Parameters
  - NONE
- Returns
  - value of the register.

This method requires a read-only or read/write policy to be used. This method register the value of
the register and returns the value of register.

#### get_field()
```c++
[[nodiscard]] value_type get_field(std::size_t pos, std::size_t size = 1, bool shifted = true) const noexcept;
```
- Parameters
  - pos : Starting bit position of the field.
  - size : Size of the field in bits.
  - shifted : 
    - true (default) : Field value returned is shifted to bit position 0.
    - false : Field value is not shifted to bit position 0.
- Returns
  - Value of the field. 

This method requires a read-only or read/write policy to be used. This method reads the register
value, extracts the field value and returns field. The pos and size parameters are used to set the
size and position of the field. The shift parameter is to tell the method format the field is 
returned. If the shift parameter is true, then the value returned is the field shifted to bit 
position 0. If the shift parameter is false, then the value returned is the masked field and not
shifted.

#### write()
```c++
void write(const value_type value) noexcept;
```
- Parameters
  - value : value to be written to the register.
- Returns
  - void.

This method requires write-only or read/write policies to be used. This method writes the value
parameter to register after the value is masked using the Mask template value. This is to prevent 
unwanted data write to reserved bits.

#### set_field()
```c++
void set_field(std::size_t pos, std::size_t size = 1, const value_type value = 1, bool shifted = false) noexcept;
```
- Parameters
  - pos : Starting bit position of field.
  - size : Size of the field in bits, default = 1.
  - value : Value to be written to field, default = 1.
  - shifted : 
    - true : Value parameter is already shifted to field position and masked.
    - false (default) : Value parameter needs to masked and shifted to field position. 
- Returns
  - void

This method requires read/write policy access. The method is used to set the value of a field in 
register. The field parameters are the size and pos, the pos parameter is used to set the starting
bit position of the field and size parameter is used to set the field size in bits. The value 
parameter is used to set the value of the field, the default is 1. If the value parameter is already
shifted into place for the field, shifted should be true. If the value needs to be shifted into place
set shifted to false, this is done by default.

#### clear_field()
```c++
void clear_field(std::size_t pos, std::size_t size = 1) noexcept;
```
- Parameters
  - pos : Starting bit position of field.
  - size : Size of the field in bits, default = 1.
- Returns 
  - void

This method requires read/write policy access. This method is used to clear a bit field in the register.
The pos and size parameters define the bit field, pos is the starting bit position of the field and size
is the bit field size in bits. The result of the operation is the field value is set to 0.

### <u>Assignment operators</u>

#### Copy assignment (default)
```c++
basic_hardware_register& operator=(const basic_hardware_register&) noexcept = default;
```
#### Move assignment (deleted)
```c++
basic_hardware_register& operator=(basic_hardware_register&&) noexcept = delete;
```
#### Value assignment
```c++
basic_hardware_register& operator =(const value_type rhs) noexcept;
```

This operator requires write-only or read/write policy. Same functionality as the write method.

#### Value and assignment
```c++
basic_hardware_register& operator &=(const value_type rhs) noexcept;
```

This operator requires read/write policy. This operator reads the register performs an AND operation using
the rhs parameter with value of the register and then writes the result back to the register.

#### Value or assignment
```c++
basic_hardware_register& operator |=(const value_type rhs) noexcept;
```

This operator requires read/write policy. This operator reads the register performs an OR operation using
the rhs parameter with value of the register and then writes the result back to the register.

#### Value xor assignment
```c++
basic_hardware_register& operator ^=(const value_type rhs) noexcept;
```

This operator requires read/write policy. This operator reads the register performs an XOR operation using
the rhs parameter with and then writes the result back to the register.

### <u>Bit-wise operators</u>

#### AND operator
```c++
value_type operator & (const value_type rhs) const noexcept;
```

This operator requires read-only or read/write policy. This operator reads the register performs an AND operation using
the rhs parameter and returns the result of the operation.

#### OR operator
```c++
value_type operator | (const value_type rhs) const noexcept;
```

This operator requires read-only or read/write policy. This operator reads the register performs an OR operation using
the rhs parameter and returns the result of the operation.

#### XOR operator
```c++
value_type operator ^ (const value_type rhs) const noexcept;
```

This operator requires read-only or read/write policy. This operator reads the register performs an XOR operation using
the rhs parameter and returns the result of the operation.

### <u>Comparison operators</u>

#### Equality comparison
```c++
bool operator == (const value_type rhs) const noexcept;
```

Compares the register value to parameter rhs, if the values are equal the return value is true, otherwise false.

#### Inequality comparison
```c++
bool operator != (const value_type rhs) const noexcept;
```

Compares the register value to parameter rhs, if the values are not equal the return value is true, otherwise false.

#### Greater than comparison
```c++
bool operator > (const value_type rhs) const noexcept;
```

Compares the register value to parameter rhs, if the register value is greater than rhs the return value is true, otherwise false.

#### Greater than or equal too comparison
```c++
bool operator >= (const value_type rhs) const noexcept;
```

Compares the register value to parameter rhs, if the register value is greater than or equal too rhs the return value is true, otherwise false.

#### Less than comparison
```c++
bool operator < (const value_type rhs) const noexcept;
```

Compares the register value to parameter rhs, if the register value is less than rhs the return value is true, otherwise false.

#### Less than or equal comparison
```c++
bool operator <= (const value_type rhs) const noexcept;
```

Compares the register value to parameter rhs, if the register value is less than or equal too rhs the return value is true, otherwise false.
