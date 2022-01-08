#include <concepts>
#include <iostream>

namespace sic
{

template<typename T>
concept Integral = std::is_integral<T>::value;

template<typename T>
concept Number = std::is_fundamental<T>::value;

template<Integral T>
constexpr bool is_power_of_two(T num);


template<typename T, typename U>
constexpr T power(T base, U exponent);
}



namespace sic
{

template<Integral T>
constexpr bool is_power_of_two(T num)
{
  return ((num > 0) && ((num & (num - 1)) == 0));
}

template<typename T, typename U>
constexpr T power(T base, U exponent)
{

  if (exponent == 0) {
    return 1;
  }
  return base * power(base, exponent - 1);
}

}
