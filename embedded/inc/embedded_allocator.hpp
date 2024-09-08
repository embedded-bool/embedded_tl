/**
 * @file embedded_allocator.hpp
 * @date 2024-08-07
 * @author Robert Morley
 *
 * @brief Embedded Template Library : Device allocator classes template header file.
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#ifndef EMBEDDED_TL_EMBEDDED_ALLOCATOR_HPP
#define EMBEDDED_TL_EMBEDDED_ALLOCATOR_HPP

#include <algorithm>

#include <embedded_types.hpp>
#include <embedded_concepts.hpp>
#include <embedded_utilities.hpp>

namespace embtl {
    /**
     * @brief Memory Mapped IO (MMIO) Device Information.
     * @details
     * This class is used to create a single or list of devices for the hardware allocator function. Information stored
     * in this class is the Device index number and base address of the registers for the device.
     */
    struct memory_mapped_device_info final {
      public:
        constexpr memory_mapped_device_info() noexcept = delete;
        /**
         * @brief Single memory mapped device constructor
         * @param base [in] Base Address.
         * @details
         * This constructor is used when there are not list of memory mapped devices to allocate. The number variable
         * is automatically set to 0 and only the base_address is set by the user.
         */
        constexpr memory_mapped_device_info(const address_t base) noexcept : memory_mapped_device_info(0_uz, base) { }
        /**
         * @brief Construct list initializer for multiple numerically indexed memory mapped devices.
         * @param n [in] Device index number.
         * @param base [in] Memory mapped device base address associated with
         * @details
         * This constructor is used for numerically indexed memory mapped devices. The n parameter is the device number
         * and the base parameter is the base address of the indexed device.
         * @note The literal operator '_uz' need to be used for the n parameter so not confused the compiler with the
         * alpha character based device index constructor.
         */
        constexpr memory_mapped_device_info(const std::size_t n, const address_t base) noexcept : number(n), base_address(base) { }
        /**
         * @brief Construct list initializer for muliple alpha characater indexed memory mapped devices.
         * @param n [in] Character index := ['a'-'z'] or ['A' or 'Z']
         * @param base [in] Memory mapped device base address
         * @details
         * This constructor is used for alpha character indexed memory mapped devices. The n parameter is a alpha character,
         * 'a' to 'z' or 'A' to 'Z', and the base parameter sets the base address of the memory mapped device.
         */
        constexpr memory_mapped_device_info(const char n, const address_t base) noexcept : number(char_to_index(n)), base_address(base) { }

        constexpr memory_mapped_device_info(const memory_mapped_device_info&) noexcept = default;
        constexpr memory_mapped_device_info& operator=(const memory_mapped_device_info&) noexcept = default;
        constexpr memory_mapped_device_info(memory_mapped_device_info&&) noexcept = default;
        constexpr memory_mapped_device_info& operator=(memory_mapped_device_info&&) noexcept = default;

        std::size_t number;       /**< Memory mapped device index number. */
        address_t base_address;   /**< Memory mapped device base address. */
    };

    /**
     * @brief Finds base address in list of Device memory configurations.
     * @tparam Device First Device configuration.
     * @tparam DeviceList Device configuration List.
     * @param n [in] Device number to search for.
     * @return If device number found, its base address, else 0.
     *
     * @note Unit Tested.
     */
    template<memory_mapped_device_info Device, memory_mapped_device_info ... DeviceList>
    consteval address_t find_base_address(const std::size_t n = 0) noexcept {
      if (n == Device.number){
        return Device.base_address;
      } else {
        if constexpr (sizeof...(DeviceList) > 0){
          return find_base_address<DeviceList...>(n);
        } else {
          return 0;
        }
      }
    }

    /**
     * @brief Templated Memory Mapped IO (MMIO) device allocator for number indexed devices.
     * @tparam Device First Device info to be checked against parameter n.
     * @tparam DeviceList List of Device info to checked against parameter n.
     * @param n [in] Device index request for device register map pointer.
     * @return If sucessuful a pointer to MMIO device register map base address. If the number is not
     * found a nullptr is returned.
     *
     * @example
     * - Device registers new operator overload.
     * @code{.cpp}
     *
     * class device_registers final {
     *
     *  ...
     *
     *  void* operator new(std::size_t, const std::size_t n) noexcept {
     *      return basic_mmio_device_allocator<{1_uz, 0x4000'0000},{2_uz, 0x4000'0100},{3_uz, 0x4000'0200}>(n);
     *  }
     *
     *  ...
     *
     * };
     * @endcode
     *
     * @note Unit Tested.
     */
    template<memory_mapped_device_info Device, memory_mapped_device_info ... DeviceList>
    void* basic_mmio_device_allocator(const std::size_t n = 0) noexcept {
      if(n == Device.number){
        return reinterpret_cast<void*>(Device.base_address);
      } else {
        if constexpr(sizeof...(DeviceList) > 0){
          return basic_mmio_device_allocator<DeviceList...>(n);
        } else {
          return nullptr;
        }
      }
    }

    /**
     * @brief Memory Mapped IO (MMIO) Hardware Allocator class (multiple devices)
     * @tparam Number Device Number
     * @tparam DeviceList Device configuration list.
     */
    template<std::size_t Number, memory_mapped_device_info ... DeviceList>
    requires (sizeof...(DeviceList) > 0)
    struct basic_hardware_allocator final {
      public:
        /**
         * @brief Allocator method for on-system hardware.
         * @param size Unused parameter.
         * @return void pointer to base address of hardware.
         */
        static void* allocate([[maybe_unused]]std::size_t size) noexcept {
          auto base_addr = find_base_address<Number,DeviceList...>();
          return base_addr == 0 ? nullptr : reinterpret_cast<void*>(base_addr);
        }
        /**
         * @brief De-allocator method for on-system hardware.
         * @param ptr void pointer to object.
         */
        static void deallocate([[maybe_unused]]void* ptr) noexcept { }
    };

    /**
     * @brief Templated Memory Mapped IO (MMIO) device list allocator class.
     * @tparam IndexType Indicates if the index type is a character (char) or number (std::size_t).
     * @tparam DeviceList List of devices that can be allocated with the the T::allocator(std::size_t, ItemType) method.
     *
     * @details
     * This template class is used to simplify the process of setting up MMIO multiple device drivers. The two template parameters
     * are used to set the index type and set the available valid devices for this device allocator class. The IndexType
     * parameter must be a char type or std::size_t type. The DeviceList variadic template parameter must have at least 2
     * items for the parameter to be valid. A compiler error is generated if either template parameters do not met these
     * requirements. Once this parameters are set the user can call the
     * basic_mmio_device_list_allocator::allocate(std::size_t, const ItemType) static method to get a pointer to the
     * memory mapped device registers base address for the index requested. If the index is not part of the list provided
     * a nullptr will be returned.
     *
     * @example
     * UART device list and GPIO port list allocator.
     *
     * @code{.cpp}
     *
     * using uart_allocator_t = basic_mmio_device_list_allocator<std::size_t,{1_uz, 0x5000'0000}{2_uz, 0x5000'1000}{3_uz, 0x5000'2000}>;
     * using gpio_allocator_t = basic_mmio_device_list_allocator<char,{'A', 0x4000'0000}{'B', 0x4000'1000},{'H', 0x4000'5000}>;
     *
     * @endcode
     *
     * @note Unit Tested.
     */
    template<memory_mapped_device_info ... DeviceList>
    requires (sizeof...(DeviceList) > 1)
    struct basic_mmio_device_list_allocator final {
      public:

        static void* allocate([[maybe_unused]]std::size_t sz, const std::size_t n) noexcept {
          if constexpr (host_allocation::value){
            return std::malloc(sz);
          } else {
            return basic_mmio_device_allocator<DeviceList...>(n);
          }
        }

        static void deallocate([[maybe_unused]]void* ptr) noexcept {
          if constexpr (host_allocation::value){
            std::free(ptr);
          }
        }
    };

    /**
     * @brief Templated Memory Mapped IO (MMIO) single device allocator class.
     * @tparam Device Single device information.
     * @details
     * This template class is used to simplify the process of setting up MMIO single device driver. The single template
     * parameter is used to set the device information (ie. register base address). Once set the user can call the
     * basic_mmio_single_device_allocator<>::allocate(std::size_t) method to get the pointer to the memory mapped base
     * register. The allocate methods will always return a pointer, nullptr will be returned.
     *
     * @example
     * Reset and Clock control device allocator.
     *
     * @code{.cpp}
     *
     *  using rcc_allocator_t = basic_mmio_single_device_allocator<{0x4001'4000}>;
     *
     * @endcode
     *
     * @note Unit Tested.
     */
    template<memory_mapped_device_info Device>
    struct basic_mmio_single_device_allocator final {
      public:
        static void* allocate([[maybe_unused]]std::size_t sz) noexcept {
          if constexpr (host_allocation::value){
            return std::malloc(sz);
          } else {
            return basic_mmio_device_allocator<Device>();
          }
        }

        static void deallocate([[maybe_unused]]void* ptr) noexcept {
          if constexpr (host_allocation::value){
            std::free(ptr);
          }
        }
    };
}

#endif //EMBEDDED_TL_EMBEDDED_ALLOCATOR_HPP
