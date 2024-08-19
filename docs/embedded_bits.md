# <u>Embedded Bits</u>
[back](../README.md)
### File: [embedded_bits.hpp](../embedded/inc/embedded_bits.hpp)

### Namespace : embtl

## <u>Description</u>
This file is for bit manipulation 

### make_mask()
```c++
template<embedded_base_type T>
constexpr auto make_mask(const std::size_t pos, const std::size_t size) noexcept -> T { ... }
```
- Template parameters
  - T : Value type to be returned, must be integral type, unsigned and not boolean.
- Parameters
  - pos : Starting bit position of the mask.
  - size : Size in bits of the mask.
- Returns
  - generate mask value for T type.

Constexpr function that generates a continuous bit field mask based on the function parameters given. The two parameters
needed are pos and size, pos is the starting bit position of the mask and size set the bit size of the mask. 

### mask_parameters
```c++
struct mask_parameters final;
```

#### Description

#### Constructors
- Default constructor deleted.
```c++
constexpr mask_parameters() = delete;
```
- Initializer constructor (single value)
```c++
constexpr mask_parameters(const std::size_t pos) noexcept;
```

Used for single bit masks, pos sets the position of mask and size is set to 1 by default.

- Initializer constructor (all variables)
```c++
constexpr mask_parameters(const std::size_t pos, const std::size_t sz) noexcept; 
```

Used for multi bit masks, pos sets the start position of the mask and sz sets the size of the mask in bits.

#### Members (public)
- position [std::size_t] : Starting bit position of the mask.
- size [std::size_t] : Size of the mask in bits. 

### static_mask
```c++
template<embedded_base_type T, mask_parameters ... PosSize>
constexpr auto static_mask = ... ;
```
- Template parameters
  - T : Mask variable type, must be integral type, unsigned and not boolean.
  - PosSize : Mask Parameter list for mask generation

Constexpr template variable that generates a continuous or discontinuous mask for variable type T based on PosSize list.

### mask_u64b
```c++
template<mask_parameters ... PosSize>
constexpr auto mask_u64b = ...;
```
- Template Parameters
  - PosSize : Mask Parameter list for mask generation.

Constexpr template variable that generates a continuous or discontinuous mask for variable of unsigned 64bit based on PosSize list.

### mask_u32b
```c++
template<mask_parameters ... PosSize>
constexpr auto mask_u32b = ...;
```
- Template Parameters
    - PosSize : Mask Parameter list for mask generation.

Constexpr template variable that generates a continuous or discontinuous mask for variable of unsigned 32bit based on PosSize list.

### mask_u16b
```c++
template<mask_parameters ... PosSize>
constexpr auto mask_u16b = ...;
```
- Template Parameters
    - PosSize : Mask Parameter list for mask generation.

Constexpr template variable that generates a continuous or discontinuous mask for variable of unsigned 16bit based on PosSize list.

### mask_u8b
```c++
template<mask_parameters ... PosSize>
constexpr auto mask_u64b = ...;
```
- Template Parameters
    - PosSize : Mask Parameter list for mask generation.

Constexpr template variable that generates a continuous or discontinuous mask for variable of unsigned 8bit based on PosSize list.
