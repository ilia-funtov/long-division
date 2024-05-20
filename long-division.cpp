#include <cassert>
#include <cmath>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename T>
std::enable_if_t<std::is_unsigned_v<T>, std::pair<T, T>>
long_division(T dividend, T divisor)
{
    static_assert(std::is_integral_v<T>, "T must be an integral type");

    if (divisor == 0)
    {
        throw std::invalid_argument("Divisor cannot be zero");
    }

    T quotient = 0;

    T current_divisor = divisor;
    T quotient_add_term = 1;

    for (;;)
    {
        while (dividend >= current_divisor)
        {
            dividend -= current_divisor;
            quotient += quotient_add_term;
            const auto next_divisor = static_cast<decltype(current_divisor)>(current_divisor << 1);
            if ((next_divisor >> 1) == current_divisor)
            {
                current_divisor = next_divisor;
                quotient_add_term <<= 1;
            }
        }

        if (dividend < divisor)
        {
            break;
        }

        current_divisor = divisor;
        quotient_add_term = 1;
    }

    return std::make_pair(quotient, dividend);
}

template <typename T>
std::enable_if_t<std::is_signed_v<T>, std::pair<T, T>>
long_division(T dividend, T divisor)
{
    const bool negative = (dividend < 0) ^ (divisor < 0);
    using unsigned_T = typename std::make_unsigned<T>::type;
    const auto abs_dividend = static_cast<unsigned_T>(std::abs(dividend));
    if (dividend < 0 && -(static_cast<decltype(dividend)>(abs_dividend)) != dividend)
    {
        throw std::runtime_error("Dividend cast truncation");
    }

    const auto abs_divisor = static_cast<unsigned_T>(std::abs(divisor));
    if (divisor < 0 && -(static_cast<decltype(divisor)>(abs_divisor)) != divisor)
    {
        throw std::runtime_error("Divisor cast truncation");
    }

    auto [quotient, remainder] = long_division(abs_dividend, abs_divisor);
    if (negative && remainder != 0)
    {
        remainder = abs_divisor - remainder;
    }
    return std::make_pair((negative ? -(quotient + ((remainder != 0) ? 1:0)) : quotient), (divisor < 0) ? -remainder : remainder);
}

template <typename T>
void test_long_division(T dividend, T divisor)
{
    const auto [quotient, remainder] = long_division(dividend, divisor);
    if constexpr (std::is_signed_v<T>)
    {
        assert(std::abs(remainder) < std::abs(divisor));
        if (dividend >= 0 && divisor > 0)
        {
            assert(quotient >= 0);
            assert(remainder >= 0);
        }
        else if (dividend < 0 && divisor > 0)
        {
            assert(quotient <= 0);
            assert(remainder >= 0);
        }
        else if (dividend < 0 && divisor < 0)
        {
            if (quotient < 0)
            {                
                assert(quotient >= 0);
                assert(remainder <= 0);
            }
            assert(quotient >= 0);
            assert(remainder <= 0);
        }
        else if (dividend >= 0 && divisor < 0)
        {
            assert(quotient <= 0);
            assert(remainder <= 0);
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        assert(remainder < divisor);
    }
    
    assert(quotient * divisor + remainder == dividend);
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

int main()
{
    assert(long_division(1, 1).first == 1);
    assert(long_division(1, 1).second == 0);
    assert(long_division(0, 1).first == 0);
    assert(long_division(1, 1).first == 1);
    assert(long_division(2, 1).first == 2);
    
    {
        const auto [quotient, remainder] = long_division(3, 2);
        assert(quotient == 1);
        assert(remainder == 1);
    }

    {
        const auto [quotient, remainder] = long_division(64, 4);
        assert(quotient == 16);
        assert(remainder == 0);
    }

    {
        const auto [quotient, remainder] = long_division(64, 4);
        assert(quotient == 16);
        assert(remainder == 0);
    }

    {
        const auto [quotient, remainder] = long_division(27, 11);
        assert(quotient == 2);
        assert(remainder == 5);
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

    return 0;
}