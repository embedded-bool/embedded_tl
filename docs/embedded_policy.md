# <u>Embedded Policy</u>
[back](../README.md)
### File: [embedded_policy.hpp](../embedded/inc/embedded_policy.hpp)

## Namespace : embtl::policy

## Description

This file contains classes that represent how registers are accessed by the drivers. The goal
of these classes is help the developer(s) spot access errors at compile time to register that 
can only be accessed in a specific way. For example a read-only register being written to by
the driver. Instead of having to debug this simple error at runtime it can be done at compile 
time. All policy classes only have static methods. 

## basic_reg_root class.
```c++
template<embedded_base_type Base>
struct basic_reg_root { ... };    
```
### <u>Description</u>

This class has no public methods, just one protected method that is inherited by all other policy
classes.

### <u>Template Parameters</u>
- Base : used to device the register type, 8bit, 16bit, 32bit, etc.

### <u>Static Methods (protected) </u>
#### make_mask()
```cpp
inline static constexpr auto make_mask(const std::size_t pos, const std::size_t size) noexcept;
```
- Parameters
  - std::size_t pos := Sets the starting bit position of the mask.
  - std::size_t size := Sets the size in bits of the mask.
- Returns: Bit mask based on the parameters given. Return type is Base template parameter type.

Used to generate a bit field mask based on the pos and size parameters. The pos parameter is used to
set the starting bit position of the mask and the size parameter is used to set the number of bits in
the field. The return type is the Base template parameter type.

## basic_reg_read_only class.

```c++
template<embedded_base_type Base>
struct basic_reg_read_only : public basic_reg_root<Base> { ... };
```

### <u>Description</u>
The read only policy class is used for set the access policy for in the [basic_hardware_register](embedded_registers.md) 
template class. If the register is accessed with a method other than a read a compile time error will occur. This 
class will allow developers to restrict access to specific registers that only allow read access.

### <u>Template Parameters</u>
- Base : used to set the device register type; 8bit, 16bit, 32bit, etc.

### <u>Static Methods (public) </u>

#### read()
```cpp
static value_type read(const volatile value_type& reg) noexcept;
```
- Parameters
  - reg : Constant reference to volatile register that is being read.
- Returns
  - Value stored in the register.

This is a basic read function for the hardware register. The data return is the value
stored in the register.

#### get_field()
```cpp
static value_type get_field(const volatile value_type& reg, std::size_t pos, std::size_t size, bool shifted = true) noexcept;
```
- Parameters
  - reg : Constant reference to volatile register that being read.
  - pos : Starting bit position of the field being read.
  - size : Size of the field in bits.
  - shifted : 
    - true := return value where field is shift to bit position 0.
    - false := return only the masked value of field not shifted.
- Returns
  - value of the field, shift or un-shifted. 

This method gets the requested field of the in the register. The parameters of the field are given in the 
pos and size parameters. The pos parameter is the starting bit position of the field, the size parameter
is the size of the field in bits. The value returned is effected by the shift parameter, if you only want 
the masked value of the field then set the shifted parameter to false. If you want the field value shifted 
to bit position 0 then set the shifted parameter to true, this is the default value.

## basic_reg_write_only class.
```c++
template<embedded_base_type Base, Base Mask = std::numeric_limits<arch_type>::max()>
struct basic_reg_write_only : public basic_reg_root<Base> { ... }; 
```

### <u>Description</u>
The write only policy class is used for set the access policy for in the [basic_hardware_register](embedded_registers.md)
template class. If the register is accessed with a method other than write a compile time error will occur. This
class will allow developers to restrict access to specific registers that only allow writes access.

### <u>Template Parameters</u>
- Base : used to device the register type, 8bit, 16bit, 32bit, etc.
- Mask : Write mask to prevent write operations from changing bits that are reserved. The default is all bits are masked.

### <u>Types</u>
- value_type : Is the Base template parameter type.

### <u>Static Methods (public) </u>
#### write()
```cpp
static void write(volatile value_type& reg, const value_type value) noexcept
```
- Parameters
  - reg : Reference to volatile reg to be written to.
  - value : The value of to be written to the register

This method will write a value to the register. The value is masked with the Mask template
parameter before being written to the register.

## basic_reg_read_write class.
```c++
template<embedded_base_type Base, Base Mask = std::numeric_limits<Base>::max()>
struct basic_reg_read_write : public basic_reg_read_only<Base>, basic_reg_write_only<Base, Mask> { ... };
```
### <u>Description</u>
The read/write policy class is used for set the access policy for in the [basic_hardware_register](embedded_registers.md)
template class. This policy allows full access to the register. The class itself inherits from the basic_reg_read_only and
basic_reg_write_only policy classes. The two methods that in this class require both read and write access to the register.

### <u>Template Parameters</u>
- Base : used to device the register type, 8bit, 16bit, 32bit, etc.
- Mask : Write mask to prevent write operations from changing bits that are reserved. The default is all bits are masked.

### <u>Types</u>
- value_type : Is the Base template parameter type.

### <u>Static Methods (public) </u>

#### set_field()
```cpp
static void set_field(volatile value_type& reg, const std::size_t pos, const std::size_t size, const value_type value, bool shifted = false) noexcept;
```
- Parameters
  - reg : Volatile reference to hardware register.
  - pos : Starting bit position of the field.
  - size : Size of the field in bits.
  - value : Value to set in the field.
  - shifted : 
    - true : the value parameter is already shifted to the starting bit position.
    - false : the value parameter needs to shifted to the starting bit position.

This method sets a value in a bit field defined by the parameters pos and size. The pos parameter sets the starting bit position
of the field and the size parameter sets the size of the bit field in bits. The shift parameter is used to indicate if the value 
parameter is already masked and shifted to the proper field position and size. If the shift parameter is true then the value is
ready to writen to register without masking and shifting the value. If the shift parameter is false (default) then the value needs
to masked and shifted to the parameters given. This method requires both read and write access to the register.

#### clear_field()
```cpp
static void clear_field(volatile value_type& reg, const std::size_t pos, const std::size_t size) noexcept;
```
- Parameters
  - reg : Volatile reference to hardware register.
  - pos : Staring bit position of the field.
  - size : Size of the field in bits.

This method is used to clear a bit field. The parameters used are pos and size. The pos parameter is the starting bit position
of the field and the size parameter is the size of the in bits. This method requires both read and write access.

## basic_reg_reserved class.
```c++
template<embedded_base_type>
struct basic_reg_reserved { };
```

### <u>Description</u>
The reserved access policy class is used for set the access policy for in the [basic_hardware_register](embedded_registers.md)
template class. This policy does not permit access to the register for any access type, however, register is still preserved in
memory space.
