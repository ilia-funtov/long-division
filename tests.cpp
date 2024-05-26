#include <cmath>
#include <iostream>
#include <random>
#include <stdexcept>
#include <utility>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "long-division.hpp"

template <typename T>
void test_long_division(T dividend, T divisor)
{
    const auto [quotient, remainder] = long_division(dividend, divisor);
    if constexpr (std::is_signed_v<T>)
    {
        REQUIRE(std::abs(remainder) < std::abs(divisor));
        if (dividend >= 0 && divisor > 0)
        {
            REQUIRE(quotient >= 0);
            REQUIRE(remainder >= 0);
        }
        else if (dividend < 0 && divisor > 0)
        {
            REQUIRE(quotient <= 0);
            REQUIRE(remainder >= 0);
        }
        else if (dividend < 0 && divisor < 0)
        {
            if (quotient < 0)
            {                
                REQUIRE(quotient >= 0);
                REQUIRE(remainder <= 0);
            }
            REQUIRE(quotient >= 0);
            REQUIRE(remainder <= 0);
        }
        else if (dividend >= 0 && divisor < 0)
        {
            REQUIRE(quotient <= 0);
            REQUIRE(remainder <= 0);
        }
        else
        {
            REQUIRE(false);
        }
    }
    else
    {
        REQUIRE(remainder < divisor);
    }
    
    REQUIRE(quotient * divisor + remainder == dividend);
}

template <typename T>
void long_division_sample_test(int sample_count)
{
    std::default_random_engine eng(3);
    std::mt19937 gen(eng());
    
    T min_shift = 0;
    if (std::numeric_limits<T>::is_signed)
    {
        min_shift = 1;
    }

    std::uniform_int_distribution<T> dis(std::numeric_limits<T>::min() + min_shift, std::numeric_limits<T>::max());

    for (int n = 0; n != sample_count; ++n)
    {
        const auto dividend = dis(gen);
        auto divisor = dis(gen);
        while (divisor == 0)
        {
            divisor = dis(gen);
        }
        test_long_division(dividend, divisor);
    }
}

template <typename T>
void long_division_small_divisor_test(int sample_count)
{
    std::default_random_engine eng(3);
    std::mt19937 gen(eng());
    std::uniform_int_distribution<T> dis(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

    T divisor = 1;
    for (int n = 0; n != sample_count; ++n)
    {
        T dividend = dis(gen);

        test_long_division(dividend, divisor);

        if (std::numeric_limits<T>::max() - (divisor + 1) < divisor)
        {
            break;
        }

        divisor += divisor + 1;        
    }
}

template <typename T>
void long_division_test(int sample_count)
{
    long_division_sample_test<T>(sample_count);
    long_division_small_divisor_test<T>(sample_count);
}

TEST_CASE("Basic division")
{
    REQUIRE(long_division(1, 1).first == 1);
    REQUIRE(long_division(1, 1).second == 0);
    REQUIRE(long_division(0, 1).first == 0);
    REQUIRE(long_division(1, 1).first == 1);
    REQUIRE(long_division(2, 1).first == 2);
    
    {
        const auto [quotient, remainder] = long_division(3, 2);
        REQUIRE(quotient == 1);
        REQUIRE(remainder == 1);
    }

    {
        const auto [quotient, remainder] = long_division(64, 4);
        REQUIRE(quotient == 16);
        REQUIRE(remainder == 0);
    }

    {
        const auto [quotient, remainder] = long_division(64, 4);
        REQUIRE(quotient == 16);
        REQUIRE(remainder == 0);
    }

    {
        const auto [quotient, remainder] = long_division(27, 11);
        REQUIRE(quotient == 2);
        REQUIRE(remainder == 5);
    }
}

TEST_CASE("Sample values division")
{
    const auto sample_count = 1000000;

    long_division_test<std::int8_t>(sample_count);
    long_division_test<std::uint8_t>(sample_count);

    long_division_test<std::int16_t>(sample_count);
    long_division_test<std::uint16_t>(sample_count);

    long_division_test<std::int32_t>(sample_count);
    long_division_test<std::uint32_t>(sample_count);

    long_division_test<std::int64_t>(sample_count);
    long_division_test<std::uint64_t>(sample_count);
}
