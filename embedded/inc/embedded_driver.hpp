/**
 * @file embedded_driver.hpp
 * @date 2024-08-07
 * @author Robert Morley
 *
 * @brief Embedded Template Library : Device Driver Template base class.
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#ifndef EMBEDDED_TL_EMBEDDED_DRIVER_HPP
#define EMBEDDED_TL_EMBEDDED_DRIVER_HPP

#include <embedded_region.hpp>

namespace embtl {

    template<typename DeviceIoRegion>
    struct basic_driver {
      public:
        basic_driver() noexcept : reg_map(new DeviceIoRegion) { }

        basic_driver(const basic_driver&) noexcept = default;
        basic_driver& operator=(const basic_driver&) noexcept = default;
        basic_driver(basic_driver&&) noexcept = delete;
        basic_driver& operator=(basic_driver&&) noexcept = delete;

        [[nodiscard]] bool has_error() const noexcept{
          return reg_map == nullptr;
        }

      protected:
        DeviceIoRegion* reg_map;
    };

}

#endif //EMBEDDED_TL_EMBEDDED_DRIVER_HPP
