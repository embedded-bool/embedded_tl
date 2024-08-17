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
