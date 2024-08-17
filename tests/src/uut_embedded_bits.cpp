/**
 * @file uut_embedded_bits.cpp
 * @date 2024-08-11
 * @author Robert Morley
 *
 * @brief Unit Test : Embedded Template Library := Bit manipulation library
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#include <uut_catch2.hpp>
#include <embedded_bits.hpp>

TEMPLATE_TEST_CASE("make_mask template function test","[embtl][bits][template][make_mask]",
                   (std::uint8_t),
                   (std::uint16_t),
                   (std::uint32_t),
                   (std::uint64_t)
){
  SECTION("mask_mask"){
    auto position = GENERATE(take(100, random(static_cast<std::size_t>(0), std::numeric_limits<TestType>::digits - static_cast<std::size_t>(1))));
    auto gen_size = GENERATE(take(100, random(static_cast<std::size_t>(1), static_cast<std::size_t>(std::numeric_limits<TestType>::digits))));

    std::size_t sz = gen_size;
    TestType mask_check { };

    if(position == 0 && sz == std::numeric_limits<TestType>::digits){
      mask_check = std::numeric_limits<TestType>::max();
    }
    else {
      if((position + sz) > std::numeric_limits<TestType>::digits){
        sz = std::numeric_limits<TestType>::digits - position;
      }
      mask_check = static_cast<TestType>((0b1U << sz) - 1U);
      mask_check <<= position;
    }

    CAPTURE(std::numeric_limits<TestType>::digits, position, gen_size, sz, mask_check);
    REQUIRE(embtl::make_mask<TestType>(position, sz) == mask_check);
  }
}

template<typename T, embtl::mask_parameters M1, embtl::mask_parameters ... Ms>
struct mask_check final {
  static constexpr auto value() noexcept -> T {
    return mask_check<T,M1>::value() | mask_check<T,Ms...>::value();
  }
};

template<typename T, embtl::mask_parameters M>
struct mask_check<T, M> final {
  static constexpr auto value() noexcept -> T {
    if constexpr (M.size >= std::numeric_limits<T>::digits){
      return std::numeric_limits<T>::max();
    } else {
      return static_cast<T>((0b1U << M.size) - 1U) << M.position;
    }
  }
};

TEMPLATE_TEST_CASE_SIG("static_mask method test", "[embtl][bits][static_mask][template]",
                       ((typename BaseType, embtl::mask_parameters...MaskParams), BaseType, MaskParams...),
                       (embtl::arch_type, {0, 32}),
                       (embtl::arch_type, {0, 4}, { 8, 8}),
                       (embtl::arch_type, { 1 }, { 24, 4 }),
                       (std::uint8_t, { 1 }, { 4, 2 }, { 7 } ),
                       (std::uint16_t, { 1, 2 }, { 8, 3 }, { 12, 4 }),
                       (std::uint64_t, { 1,8 }, {32, 3 }, { 42, 8 }, { 63} )
){
  SECTION("Mask Generations"){
    SECTION("Static Constexpr"){
      constexpr auto mask = embtl::static_mask<BaseType, MaskParams...>;
      STATIC_REQUIRE(mask == mask_check<BaseType, MaskParams...>::value());
    }
  }
}