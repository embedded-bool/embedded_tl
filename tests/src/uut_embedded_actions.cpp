/**
 * @file uut_embedded_actions.cpp
 * @date 2024-10-05
 * @author Robert Morley
 *
 * @brief Unit Tests for embtl::status_action_if<>() template function.
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#include <uut_catch2.hpp>
#include <embedded_actions.hpp>
#include <algorithm>

using base_rvs_t = embtl::basic_return_value_status<embtl::arch_type, 0>;
using bool_rvs_t = embtl::basic_return_value_status<bool, false>;
using address_rvs_t = embtl::basic_return_value_status<embtl::address_t, 0>;

struct HasStatusInitializer final {
  public:
    HasStatusInitializer() = delete;

    explicit HasStatusInitializer(embtl::STATUS stat) noexcept: status(stat) {}

    [[nodiscard]] embtl::STATUS GetStatus() const noexcept { return status; }

  private:
    embtl::STATUS status;
};

struct HasNoStatusInitializer final {
  public:
    HasNoStatusInitializer() = default;

    [[nodiscard]] embtl::STATUS GetStatus() const noexcept { return status; }

  private:
    embtl::STATUS status;
};

/**
 * @brief Unit test for embtl::status_initializer concept.
 */
TEMPLATE_TEST_CASE("embtl action : status_initializer test", "[hal][actions][concepts]",
                   HasStatusInitializer, HasNoStatusInitializer) {
  SECTION("concept") {
    SECTION("pass") {
      if constexpr (std::is_same_v<HasStatusInitializer, TestType>) {
        STATIC_REQUIRE(embtl::status_initializer<TestType>);
      }
    }SECTION("fail") {
      if constexpr (std::is_same_v<HasNoStatusInitializer, TestType>) {
        STATIC_REQUIRE_FALSE(embtl::status_initializer<TestType>);
      }
    }
  }
}

/**
 * @brief Unit test for embtl::action_return_type concept.
 */
TEST_CASE("embtl action : action_rt concept", "[hal][actions][concepts]") {
  auto func1 = []() -> void {};
  auto func2 = []() -> embtl::STATUS { return embtl::STATUS::OK; };
  auto func3 = []() -> base_rvs_t { return base_rvs_t{}; };
  auto func4 = []() -> bool_rvs_t { return bool_rvs_t{}; };
  auto func5 = []() -> HasStatusInitializer { return HasStatusInitializer{embtl::STATUS::OK}; };

  auto func6 = []() -> int { return 0; };
  auto func7 = []() -> float { return 0.1f; };
  auto func8 = []() -> double { return 0.2; };
  auto func9 = []() -> bool { return false; };
  auto func10 = []() -> HasNoStatusInitializer { return {}; };

  SECTION("Concept") {
    SECTION("Pass") {
      STATIC_REQUIRE(embtl::action_return_type<decltype(func1())>);
      STATIC_REQUIRE(embtl::action_return_type<decltype(func2())>);
      STATIC_REQUIRE(embtl::action_return_type<decltype(func3())>);
      STATIC_REQUIRE(embtl::action_return_type<decltype(func4())>);
      STATIC_REQUIRE(embtl::action_return_type<decltype(func5())>);
    }SECTION("Fail") {
      STATIC_REQUIRE_FALSE(embtl::action_return_type<decltype(func6())>);
      STATIC_REQUIRE_FALSE(embtl::action_return_type<decltype(func7())>);
      STATIC_REQUIRE_FALSE(embtl::action_return_type<decltype(func8())>);
      STATIC_REQUIRE_FALSE(embtl::action_return_type<decltype(func9())>);
      STATIC_REQUIRE_FALSE(embtl::action_return_type<decltype(func10())>);
    }
  }
}

/**
 * @brief Unit test for embtl::action_can_return concept.
 */
TEST_CASE("embtl action : action_can_return", "[embtl][actions][concepts]") {
  SECTION("Concept") {
    SECTION("Pass") {
      STATIC_REQUIRE(embtl::action_can_return<bool_rvs_t>);
      STATIC_REQUIRE(embtl::action_can_return<base_rvs_t>);
      STATIC_REQUIRE(embtl::action_can_return<address_rvs_t>);
      STATIC_REQUIRE(embtl::action_can_return<embtl::STATUS>);
      STATIC_REQUIRE(embtl::action_can_return<HasStatusInitializer>);
    }SECTION("Fail") {
      STATIC_REQUIRE_FALSE(embtl::action_can_return<void>);
      STATIC_REQUIRE_FALSE(embtl::action_can_return<HasNoStatusInitializer>);
    }
  }
}

/**
 * @brief Unit test for embtl::status_action_if template function with no parameters.
 */
TEST_CASE("status_action_if function test : no parameters", "[embtl][action][template][function][no_parameters]") {
  using embtl::STATUS;
  auto status_value = std::array<STATUS, 10>{STATUS::ERROR, STATUS::NOT_IMPLEMENTED, STATUS::NOT_AVAILABLE,
                                             STATUS::BUFFER_OVERFLOW, STATUS::INVALID_PARAMETER,
                                             STATUS::OUT_OF_RANGE, STATUS::UNINITIALIZED,
                                             STATUS::OK, STATUS::BUSY, STATUS::INITIALIZED};
  SECTION("Return Type := void") {
    SECTION("return STATUS::ERROR") {
      auto result = embtl::status_action_if(false, []() {});
      REQUIRE(result == embtl::STATUS::ERROR);
    }SECTION("return STATUS::OK") {
      auto result = embtl::status_action_if(true, []() {});
      REQUIRE(result == embtl::STATUS::OK);
    }
  }

  SECTION("Return Type := STATUS initializer") {
    auto x = GENERATE(take(10, random(1_uz, 1000_uz)));

    for (auto &status: status_value) {
      DYNAMIC_SECTION("base_rvs_t { value := " << x << ", status := " << status << " }") {
        auto result = embtl::status_action_if(true, [&x, &status]() -> base_rvs_t {
            return base_rvs_t{static_cast<embtl::arch_type>(x), status};
        });

        REQUIRE(result.get_value() == static_cast<embtl::arch_type>(x));
        REQUIRE(result.get_status() == status);
      }
    }
  }

  SECTION("Return Type := STATUS") {
    for (auto &status: status_value) {
      DYNAMIC_SECTION("status := " << status) {
        auto result = embtl::status_action_if(true, [status]() -> embtl::STATUS {
            return status;
        });

        REQUIRE(result == status);
      }
    }
  }
}

/**
 * @brief Unit test for embtl::status_action_if template function with parameters.
 */
TEST_CASE("status_action_if function test : with parameters", "[embtl][action][template][function][with_parameters]") {
  using embtl::STATUS;
  using int_rvs_t = embtl::basic_return_value_status<int, 0>;

  auto param0 = GENERATE(take(100, random(std::numeric_limits<int>::min(), std::numeric_limits<int>::max())));
  auto param1 = GENERATE(take(10, random(std::numeric_limits<int>::min(), std::numeric_limits<int>::max())));
  auto param2 = GENERATE(take(10, random(std::numeric_limits<int>::min(), std::numeric_limits<int>::max())));

  auto sum_initializer = [](int p0, int p1, int p2) -> int_rvs_t {
      std::array<int, 3> parameters{p0, p1, p2};
      auto sum = std::accumulate(parameters.begin(), parameters.end(), 0);
      return int_rvs_t{sum, STATUS::OK};
  };
  auto sum_status = [](int p0, int p1, int p2) -> STATUS {
      std::array<int, 3> parameters{p0, p1, p2};
      auto sum = std::accumulate(parameters.begin(), parameters.end(), 0);
      return sum > 0 ? STATUS::OK : STATUS::ERROR;
  };

  SECTION("Returns := STATUS Initializer") {
    auto result = embtl::status_action_if(true, sum_initializer, param0, param1, param2);
    auto expected = param0 + param1 + param2;

    REQUIRE(result.get_value() == expected);
    REQUIRE(result.get_status() == STATUS::OK);
  }

  SECTION("Returns := STATUS") {
    auto result = embtl::status_action_if(true, sum_status, param0, param1, param2);
    auto expected = param0 + param1 + param2;

    if (expected > 0) {
      REQUIRE(result == STATUS::OK);
    } else {
      REQUIRE(result == STATUS::ERROR);
    }
  }
}
