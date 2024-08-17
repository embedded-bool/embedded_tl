/**
 * @file embedded_bitmanip.hpp
 * @date 2024-08-07
 * @author Robert Morley
 *
 * @brief
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#ifndef EMBEDDED_TL_EMBEDDED_BITMANIP_HPP
#define EMBEDDED_TL_EMBEDDED_BITMANIP_HPP

#include <embedded_types.hpp>
#include <embedded_concepts.hpp>

namespace embtl {

    /**
   * @brief Constexpr Bit Mask generator.
   * @tparam T Return Type.
   * @param pos [in] Starting position of the mask.
   * @param size [in] Size of the mask.
   * @return Generated mask.
   */
    template<embedded_base_type T>
    constexpr auto make_mask(const std::size_t pos, const std::size_t size) noexcept -> T {
      using value_type = T;

      if((pos + size) > std::numeric_limits<value_type>::digits){
        return 0;
      } else {
        if(size == std::numeric_limits<value_type>::digits){
          return std::numeric_limits<value_type>::max();
        } else {
          if constexpr (std::is_unsigned_v<value_type>){
            auto mask = static_cast<value_type>((0b1U << size) - 1);
            return mask << pos;
          } else {
            auto mask = static_cast<value_type>((0b1 << size) - 1);
            return mask << pos;
          }
        }
      }
    }

    struct mask_parameters final {
      public:
        constexpr mask_parameters() = delete;

        constexpr mask_parameters(const std::size_t pos) noexcept : mask_parameters(pos, 1) { }
        constexpr mask_parameters(const std::size_t pos, const std::size_t sz) noexcept : position(pos), size(sz) { }

        std::size_t position{};
        std::size_t size{};
    };

    namespace details {
        /**
         * @brief Constexpr bit mask generator using mask_parameters.
         * @tparam T Integral type.
         * @tparam PS0 Mask Parameter 0
         * @tparam POS_SIZE Additional Mask Parameters
         */
        template<typename T, mask_parameters PS0, mask_parameters ... POS_SIZE>
        struct mask {
            using value_type = T;

            static constexpr value_type value() noexcept {
              return mask<T, PS0>::value() | mask<T, POS_SIZE ...>::value();
            }
        };
        /**
         * @brief Constexpr bit mask generator using mask_parameters, for single value
         * @tparam T Integral Type.
         * @tparam PS Mask Parameters.
         */
        template<typename T, mask_parameters PS>
        struct mask<T, PS> {
            using value_type = T;

            static constexpr value_type value() noexcept {
              return make_mask<T>(PS.position, PS.size);
            }
        };
    }

    template<embedded_base_type T, mask_parameters ... PosSize>
    constexpr auto static_mask = details::mask<T, PosSize ...>::value();

    template<mask_parameters ... PosSize>
    constexpr auto mask_u64b = static_mask<std::uint64_t, PosSize ...>;

    template<mask_parameters ... PosSize>
    constexpr auto mask_64b = static_mask<std::int64_t, PosSize ...>;

    template<mask_parameters ... PosSize>
    constexpr auto mask_u32b = static_mask<std::uint32_t, PosSize ...>;

    template<mask_parameters ... PosSize>
    constexpr auto mask_32b = static_mask<std::int32_t, PosSize ...>;

    template<mask_parameters ... PosSize>
    constexpr auto mask_u16b = static_mask<std::uint16_t, PosSize ...>;

    template<mask_parameters ... PosSize>
    constexpr auto mask_16b = static_mask<std::int16_t, PosSize ...>;

    template<mask_parameters ... PosSize>
    constexpr auto mask_u8b = static_mask<std::uint8_t, PosSize ...>;

    template<mask_parameters ... PosSize>
    constexpr auto mask_8b = static_mask<std::int8_t, PosSize ...>;
}

#endif //EMBEDDED_TL_EMBEDDED_BITMANIP_HPP
