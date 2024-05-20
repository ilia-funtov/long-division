#include <cmath>
#include <iostream>
#include <random>
#include <stdexcept>
#include <utility>

#include "long-division.hpp"

void check(bool condition)
{
    if (!condition)
    {
        throw std::runtime_error("Test failed");
    }
}

template <typename T>
void test_long_division(T dividend, T divisor)
{
    const auto [quotient, remainder] = long_division(dividend, divisor);
    if constexpr (std::is_signed_v<T>)
    {
        check(std::abs(remainder) < std::abs(divisor));
        if (dividend >= 0 && divisor > 0)
        {
            check(quotient >= 0);
            check(remainder >= 0);
        }
        else if (dividend < 0 && divisor > 0)
        {
            check(quotient <= 0);
            check(remainder >= 0);
        }
        else if (dividend < 0 && divisor < 0)
        {
            if (quotient < 0)
            {                
                check(quotient >= 0);
                check(remainder <= 0);
            }
            check(quotient >= 0);
            check(remainder <= 0);
        }
        else if (dividend >= 0 && divisor < 0)
        {
            check(quotient <= 0);
            check(remainder <= 0);
        }
        else
        {
            check(false);
        }
    }
    else
    {
        check(remainder < divisor);
    }
    
    check(quotient * divisor + remainder == dividend);
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

void run_tests()
{
    check(long_division(1, 1).first == 1);
    check(long_division(1, 1).second == 0);
    check(long_division(0, 1).first == 0);
    check(long_division(1, 1).first == 1);
    check(long_division(2, 1).first == 2);
    
    {
        const auto [quotient, remainder] = long_division(3, 2);
        check(quotient == 1);
        check(remainder == 1);
    }

    {
        const auto [quotient, remainder] = long_division(64, 4);
        check(quotient == 16);
        check(remainder == 0);
    }

    {
        const auto [quotient, remainder] = long_division(64, 4);
        check(quotient == 16);
        check(remainder == 0);
    }

    {
        const auto [quotient, remainder] = long_division(27, 11);
        check(quotient == 2);
        check(remainder == 5);
    }

    const auto sample_count = 100000;

    long_division_test<std::int8_t>(sample_count);
    long_division_test<std::uint8_t>(sample_count);

    long_division_test<std::int16_t>(sample_count);
    long_division_test<std::uint16_t>(sample_count);

    long_division_test<std::int32_t>(sample_count);
    long_division_test<std::uint32_t>(sample_count);

    long_division_test<std::int64_t>(sample_count);
    long_division_test<std::uint64_t>(sample_count);
}

int main()
{  
    try
    {
        run_tests();
        std::cout << "All tests passed\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}