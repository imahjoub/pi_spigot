///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2019 - 2022.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// pi_spigot.cpp

// This program can be used to compute many thousands
// of decimal digits of digits of pi. Although it uses
// a so-called spigot algorithm having quadratic complexity,
// this program can, albeit slowly, compute up to a few million
// decimal digits of pi on a PC host. The test data provided
// below, however, only allow for testing up to about 100,000
// decimal digits.

// cd /mnt/c/Users/User/Documents/Ks/PC_Software/Test
// g++ -Wall -Wextra -Wpedantic -O3 -std=c++11 -finline-functions -Wconversion -Wsign-conversion pi_spigot.cpp -o pi_spigot.exe

#include <iostream>
#include <pi_spigot/pi_spigot.h>


template<const std::uint32_t result_digit,
         const std::uint32_t loop_digit>
auto test_pi_spigot_single() -> bool
{
  using pi_spigot_type = math::constants::pi_spigot_single<result_digit, loop_digit>;

  using input_container_type  = std::vector<std::uint32_t>;
  using output_container_type = std::vector<std::uint8_t>;

  input_container_type  pi_in (pi_spigot_type::get_input_static_size());
  output_container_type pi_out(pi_spigot_type::get_output_static_size());

  const std::clock_t start = std::clock();

  pi_spigot_type ps;
  ps.calculate(pi_in.begin(), pi_out.begin());

  // TBD: Use <chrono> functions for timing measurements.
  const auto stop = std::clock();

  const auto elapsed =
    static_cast<float>
    (
      static_cast<float>(stop - start) / static_cast<float>(CLOCKS_PER_SEC)
    );

  {
    const bool result_test_pi_spigot_single_is_ok =
      std::equal(pi_out.cbegin(),
                 pi_out.cend(),
                 pi_spigot_type::pi_control_string().cbegin(),
                 [](const std::uint8_t& by, const char& c) -> bool
                 {
                   return (by == static_cast<std::uint8_t>(static_cast<std::uint8_t>(c) - UINT8_C(0x30)));
                 });

    std::cout << "result_digit: "
              << result_digit
              << std::endl
              << "result_test_pi_spigot_single_is_ok: "
              << std::boolalpha
              << result_test_pi_spigot_single_is_ok
              << std::endl
              << "elapsed time: "
              << std::setprecision(3)
              << std::fixed
              << elapsed
              << "s"
              << std::endl
              << "operation_count: "
              << ps.get_operation_count()
              << std::endl
              << "input memory consumption: "
              << pi_in.size() * sizeof(input_container_type::value_type)
              << std::endl
              << std::endl;

    return result_test_pi_spigot_single_is_ok;
  }
}

auto main() -> int
{
  #if defined(PI_SPIGOT_HAS_COVERAGE)
  const bool result_is_ok = test_pi_spigot_single< 10001U, 9U>();
  #else
  const bool result_is_ok = test_pi_spigot_single<100001U, 9U>();
  #endif

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;

  return (result_is_ok ? 0 : -1);
}
