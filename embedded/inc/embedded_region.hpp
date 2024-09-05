/**
 * @file embedded_region.hpp
 * @date 2024-08-07
 * @author Robert Morley
 *
 * @brief Embedded Template Library : Device Register Region Template class header file.
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#ifndef EMBEDDED_TL_EMBEDDED_REGION_HPP
#define EMBEDDED_TL_EMBEDDED_REGION_HPP

#include <embedded_concepts.hpp>
#include <embedded_register.hpp>

namespace embtl {

    /**
     * @brief Templated Memory Map IO device registers class.
     * @tparam RegisterMap Device register map class.
     * @tparam Alloc Device register allocator type.
     *
     * @details
     * <p>
     * This template class takes two template parameters, first is the class type that has a standard layout and represents
     * the registers of the device. The second template parameter is the allocator class that returns a pointer to the devices
     * base register. Once the parameters are set the user can use this class in the device driver class to control the device.
     * There are 3 new operators methods in this class. Each new operator method requires a specific allocator to be valid as
     * specified below:
     * </p>
     *
     * <ul>
     *  <li>
     *      <b>void operator new(std::size_t);</b>
     *      <p>
     *      Requires Alloc template parameter to be a class with static method allocate(std::size_t), otherwise the methods is not
     *      considered.
     *      </p>
     *  </li>
     *  <li>
     *      <b>void operator new(std::size_t, const std::size_t);</b>
     *      <p>
     *      Requires Alloc template parameter to be a class with static method allocate(std::size_t, std::size_t), otherwise the methods is not
     *      considered.
     *      </p>
     *  </li>
     *  <li>
     *      <b>void operator new(std::size_t, const char); </b>
     *      <p>
     *      Requires Alloc template parameter to be a class with static method allocate(std::size_t,char), otherwise the methods is not
     *      considered.
     *      </p>
     *  </li>
     * </ul>
     *
     * @example
     * UART device driver setup
     *
     * @code{.cpp}
     *  #include <embedded_allocator.hpp>
     *  #include <embedded_register.hpp>
     *
     *  // 1. define devices register.
     *  // - reg_ro, reg_wo, reg_rw are aliases for basic_hardware_register<> template that are read-only, write-only, and read/write
     *  struct uart_register_map {
     *      public:
     *          reg_rw  CTRL;
     *          reg_ro  STAT;
     *          reg_wo  TXD;
     *          reg_ro  RXD;
     *  };
     *  // suggested checks for the register map.
     *  static_assert(std::is_standard_layout<register_map>;
     *  static_assert(offsetof(register_map, RXD) == 0x10);
     *
     *  // 2. define device allocator.
     *  using uart_allocator_t = basic_mmio_device_list_allocator<std::size_t,{1_uz, 0x5000'0000},{2_uz, 0x5000'1000},{3_uz, 0x5000'2000}>;
     *
     *  // 3. define uart registers type.
     *  using uart_registers_t = basic_device_registers<uart_register_map, uart_allocator_t>;
     *
     * @endcode
     */
    template<typename RegisterMap, typename Alloc>
    requires (std::is_class_v<RegisterMap> && std::is_standard_layout_v<RegisterMap>)
    struct basic_mmio_device_registers final : public RegisterMap {
      public:
        basic_mmio_device_registers() = default;

        basic_mmio_device_registers(const basic_mmio_device_registers&) noexcept = default;
        auto operator=(const basic_mmio_device_registers&) noexcept -> basic_mmio_device_registers& = default;
        basic_mmio_device_registers(basic_mmio_device_registers&&) noexcept = default;
        auto operator=(basic_mmio_device_registers&&) noexcept -> basic_mmio_device_registers& = default;

        auto operator new(std::size_t sz) noexcept -> void*
        requires mmio_single_allocator<Alloc> {
          return Alloc::allocate(sz);
        }

        auto operator new(std::size_t sz, const std::size_t n) noexcept -> void*
        requires mmio_multi_allocator_numeric<Alloc> {
          return Alloc::allocate(sz, n);
        }

        void operator delete(void*) noexcept { }
    };

    /**
     * @brief Device Register Region Template class.
     * @tparam RegisterMap Register Map class, represents register map of device.
     * @tparam DeviceAllocator Device Allocator class.
     */
    template<typename RegisterMap, mmio_allocator DeviceAllocator>
    requires (std::is_class_v<RegisterMap> && std::is_standard_layout_v<RegisterMap>)
    struct basic_device_region final : public RegisterMap{
      public:
        basic_device_region() noexcept = default;

        basic_device_region(const basic_device_region&) = default;
        basic_device_region& operator=(const basic_device_region&) = default;
        basic_device_region(basic_device_region&&) = delete;
        basic_device_region& operator=(basic_device_region&&) = delete;

        /**
         * @brief New operator for single mmio allocators.
         * @param sz [in] Size of the object in bytes.
         * @return pointer to mmio base register address.
         * @note Only available if Allocator is a single device allocator.
         */
        void* operator new(std::size_t sz) noexcept {
          return DeviceAllocator::allocate(sz);
        }
        /**
         * @brief Operator delete for single device allocators.
         * @param obj [in] Pointer to object to be deleted.
         * @note Only available if Allocator is a single device allocator.
         */
        void operator delete (void* ptr) noexcept{
          DeviceAllocator::deallocate(ptr);
        }
    };

}


#endif //EMBEDDED_TL_EMBEDDED_REGION_HPP
