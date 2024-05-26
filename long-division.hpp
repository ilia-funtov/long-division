#pragma once

#include <cmath>
#include <type_traits>
#include <stdexcept>
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
            if ((next_divisor >> 1) == current_divisor) // check if new divisor got overflow
            {
                // don't have overflow, use new divisor
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
