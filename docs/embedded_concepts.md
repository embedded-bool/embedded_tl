# <u>Embedded Concepts</u>
[back](../README.md)
### File: [embedded_concepts.hpp](../embedded/inc/embedded_concepts.hpp)

### Namespace : embtl

## <u>Description</u>

The concepts defined in this file are used to insure specific methods are part of the type being passed to the 
template class.

## <u>Concepts</u>

- ### embedded_base_type
```c++
template<typename T>
concept embedded_base_type = ... ;
```

This concept is used ensure that only valid types are passed to the register 
policies and register template classes. Must be integral type, unsigned and 
not boolean.

- ### mmio_register_policy_read_only
```c++
template<typename T>
concept mmio_register_policy_read_only = ... ;
```
This concept is used to determine if a type is a read only policy. 
Requires the following public static methods:
```c++
arch_type T::read(const volatile arch_type&) noexcept;
arch_type T::get_field(const volatile arch_type&, std::size_t, std::size_t, bool) noexcept;
```

- ### mmio_register_policy_write_only
```c++
template<typename T>
concept mmio_register_policy_write_only = ... ;
```

This concept is used to determine if a type is a write only policy.
Requires the following public static method:
```c++
void T::write(const volatile arch_type&, arch_type) noexcept;
```

- ### mmio_register_policy_read_write
```c++
template<typename T>
concept mmio_register_policy_read_write = ... ;
```
This concept is used to determine if a type is a read/write policy. The type
requires the same static methods in the read-only and write-only policy concepts
and the following public static methods:
```c++
void T::set_field(volatile arch_type&, std::size_t, std::size_t, arch_type, bool) noexcept;
void T::clear_field(volatile arch_type&, std::size_t, std::size_t) noexcept;
```

- ### mmio_side_effect_read_only
```c++
template<typename T>
concept mmio_side_effect_read_only = ... ;
```

This concept is used to determine if the side effect type has the following public static methods
for read only access:
```c++
void T::read(volatile arch_type&) noexcept;
```

- ### mmio_side_effect_write_only
```c++
template<typename T>
concept mmio_side_effect_write_only = ... ;
```

This concept is used to determine if the side effect type has the following public static methods
for write only access:
```c++
void T::write(volatile arch_type&, arch_type) noexcept;
```

- ### mmio_side_effect_read_write
```c++
template<typename T>
concept mmio_side_effect_read_write = ... ;
```
This concept is used to determine if the side effect type has the same methods as the read-only, write-only concepts 
as well as the following public static methods for read/write access:
```c++
void T::set_field(volatile arch_type&, std::size_t, std::size_t, arch_type, bool) noexcept;
void T::clear_field(volatile arch_type&, std::size_t, std::size_t) noexcept;
```

- ### mmio_allocator
```c++
template<typename T>
concept mmio_allocator = ... ;
```

The memory mapped io (mmio) allocator concept is used to insure the following static methods to meet the allocator
requirements for mmio register regions:
```c++
void* T::allocate(std::size_t) noexcept;
void T::deallocate(void*) noexcept;
```

- ### io_pin_input
```c++
template<typename T>
concept io_pin_input = ... ;
```
Some device drivers can require input pins as part of their functionality. This concept set methods need to define
IO input interface class. IO_Class is just a place hold for the IO class name.

```c++
IO_STATE read() noexcept;
IO_Class& operator==(IO_STATE);
IO_Class& operator!=(IO_STATE);
```

- ### io_pin_input
```c++
template<typename T>
concept io_pin_output = ... ;
```
Some device drivers can require output pins as part of their functionality. This concept set methods need to define
IO output interface class. IO_Class is just a place hold for the IO class name.

```c++
void write(IO_STATE) noexcept;
IO_Class& operator=(IO_STATE) noexcept;
void toggle() noexcept;
IO_Class& operator~() noexcept;
```

- ### io_pin_bidir
```c++
template<typename T>
concept io_pin_bidir = ... ;
```
Some device drivers can require bi-direction pins as part of their functionality. Both the io_pin_input and io_pin_output
methods must be part of this type provide as well as the following method for an IO bidirectional interface class. IO_Class 
is just a place hold for the IO class name.

```c++
void direction(IO_DIRECTION) noexcept;
```

