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

#include <embedded_types.hpp>
#include <embedded_concepts.hpp>

namespace embtl {


    /**
     * @brief Memory Mapped IO (MMIO) Region configuration data.
     */
    struct mmio_region_info final {
      public:
        constexpr mmio_region_info() noexcept = delete;
        constexpr mmio_region_info(address_t base) noexcept : mmio_region_info(0, base) { }
        constexpr mmio_region_info(std::size_t n, address_t base) noexcept : number(n), base_address(base) { }

        constexpr mmio_region_info(const mmio_region_info&) noexcept = default;
        constexpr mmio_region_info& operator=(const mmio_region_info&) noexcept = default;
        constexpr mmio_region_info(mmio_region_info&&) noexcept = default;
        constexpr mmio_region_info& operator=(mmio_region_info&&) noexcept = default;

        std::size_t number;
        address_t base_address;
    };

    /**
     * @brief Finds Base address in list of Device memory configurations.
     * @tparam Number Device number to find.
     * @tparam Unit First Device configuration.
     * @tparam UnitList Device configuration List.
     * @return If device number found, its base address, else 0.
     */
    template<std::size_t Number, mmio_region_info Unit, mmio_region_info ... UnitList>
    consteval address_t find_base_address() noexcept {
      if constexpr (Number == Unit.number){
        return Unit.base_address;
      } else {
        if constexpr (sizeof...(UnitList) > 0){
          return find_base_address<Number, UnitList...>();
        } else {
          return 0;
        }
      }
    }

    /**
     * @brief Memory Mapped IO (MMIO) Hardware Allocator class (multiple devices)
     * @tparam Number Device Number
     * @tparam DeviceList Device configuration list.
     */
    template<std::size_t Number, mmio_region_info ... DeviceList>
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
        static void deallocate([[maybe_unused]]void* ptr) noexcept {

        }
    };
}

#endif //EMBEDDED_TL_EMBEDDED_ALLOCATOR_HPP
