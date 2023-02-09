///////////////////////////////////////////////////////////////////
//                                                               //
//  Copyright Iliass Mahjoub 2022 - 2023.                        //
//  Copyright Christopher Kormanyos 2019 - 2023.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
//                                                               //
///////////////////////////////////////////////////////////////////

// pi_spigot.cpp

// This program can be used to compute many thousands
// of decimal digits of digits of pi. Although it uses
// a so-called spigot algorithm having quadratic complexity,
// this program can, albeit slowly, compute up to a few million
// decimal digits of pi on a PC host. The test data provided
// below, however, only allow for testing up to about 100,000
// decimal digits.

// Support up to one million one thousand and one decimal digits.
// In this program, however, we only use up to about 100 thousand
// decimal digits.

// Table of calculation characteristics
//   digits(*)   operation count   time[s]   memory[byte]
//  -----------------------------------------------------
//   100,001(9)    1,913,780,868      21       1,377,788
//    50,001(9)      478,496,610       5.3       688,900
//    25,001(9)      119,649,849       1.4       344,456
//    10,001(9)       19,155,868       0.23      137,788
//     5,001(9)        4,794,110       0.055      68,900
//     1,001(9)          193,368       0.002      13,788

//    50,001(8)      527,446,878
//    25,001(8)      131,887,503
//    10,001(8)       21,114,378

//    54,932(4)    1,320,263,154
//    50,001(4)    1,093,875,003
//    25,001(4)      273,500,003
//    10,001(4)       43,775,003
//     5,001(4)       10,950,003
//     2,001(4)        1,755,003
//     1,001(4)          440,003
//       501(4)          110,628
//       201(4)           18,003
//       101(4)            4,628
//        51(4)            1,222
//        21(4)              228
//        11(4)               72

// (*) Here, the number in parentheses such as
// (9), (8) or (4) means calculating groups
// of 9, 8 or 4 digits per loop, corresponding
// to the template parameter loop_digit.

// cd /mnt/c/Users/User/Documents/Ks/PC_Software/Test
// g++ -Wall -Wextra -Wpedantic -O3 -std=c++11 -finline-functions -Wconversion -Wsign-conversion pi_spigot.cpp -o pi_spigot.exe

#include <chrono>
#include <iostream>

#include <pi_spigot/pi_spigot.h>

template<const std::uint32_t result_digit,
         const std::uint32_t loop_digit>
auto test_pi_spigot() -> bool
{
  using pi_spigot_type = math::constants::pi_spigot<result_digit, loop_digit>;

  using input_container_type  = std::vector<std::uint32_t>;
  using output_container_type = std::vector<std::uint8_t>;
  using local_clock_type      = std::chrono::high_resolution_clock;

  input_container_type  pi_in(pi_spigot_type::get_input_static_size());
  output_container_type pi_out(pi_spigot_type::get_output_static_size());

  // record start time
  const auto start = local_clock_type::now();

  pi_spigot_type ps;

  auto result_is_ok = (ps.get_operation_count() == UINTMAX_C(0));

  ps.calculate(pi_in.begin(), pi_out.begin());

  // record end time
  const auto stop = local_clock_type::now();

  auto elapsed =
    static_cast<double>
    (
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()
    );

  {
    const auto original_flags = std::cout.flags();

    const auto result_test_pi_spigot_single_is_ok =
      std::equal(pi_out.cbegin(),
                 pi_out.cend(),
                 pi_spigot_type::pi_control_string.cbegin(),
                 [](const std::uint8_t& by, const char& c) -> bool
                 {
                   return (by == static_cast<std::uint8_t>(static_cast<std::uint8_t>(c) - UINT8_C(0x30)));
                 });

    result_is_ok = (result_test_pi_spigot_single_is_ok && result_is_ok);

    const auto operation_count = ps.get_operation_count();

    const auto result_operation_count_is_ok = (operation_count > UINTMAX_C(0));

    result_is_ok = (result_operation_count_is_ok && result_is_ok);

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
              << elapsed / 1000000.0
              << "s"
              << std::endl
              << "operation_count: "
              << operation_count
              << std::endl
              << "input memory consumption: "
              << pi_in.size() * sizeof(input_container_type::value_type)
              << std::endl
              << std::endl;

    std::cout.flags(original_flags);
  }

  return result_is_ok;
}

auto main() -> int
{
  #if defined(PI_SPIGOT_HAS_COVERAGE)
  const bool result_is_ok = test_pi_spigot< 10001U, 9U>();
  #else
  const bool result_is_ok = test_pi_spigot<100001U, 9U>();
  #endif

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;

  return (result_is_ok ? 0 : -1);
}
