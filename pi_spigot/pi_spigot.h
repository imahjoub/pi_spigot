///////////////////////////////////////////////////////////////////
//                                                               //
//  Copyright Iliass Mahjoub 2022 - 2023.                        //
//  Copyright Christopher Kormanyos 2019 - 2023.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
//                                                               //
///////////////////////////////////////////////////////////////////

#ifndef PI_SPIGOT_2022_06_08_H
  #define PI_SPIGOT_2022_06_08_H

  #include <algorithm>
  #include <cstdint>
  #include <ctime>
  #include <iomanip>
  #include <iterator>
  #include <string>
  #include <vector>

  #include <pi_spigot/pi_spigot_ctrl.h>

  #if (__cplusplus >= 201703L)
  namespace math::constants {
  #else
  namespace math { namespace constants {
  #endif

  template<const std::uint32_t ResultDigit,
           const std::uint32_t LoopDigit>
  class pi_spigot
  {
  private:
    static constexpr auto result_digit = ResultDigit;
    static constexpr auto loop_digit   = LoopDigit;

    static_assert(result_digit <= UINT32_C(1001001),
                  "Error: result_digit exceeds its limit of 1,001,001");

    static_assert((loop_digit >= UINT32_C(4)) && (loop_digit <= UINT32_C(9)),
                  "Error: loop_digit is outside its range of 4...9");

  public:
    static inline const std::string pi_control_string = // NOLINT(cert-err58-cpp)
      []()
      {
        std::string str { };

        for(auto pstr : pi_control_data) // NOLINT(llvm-qualified-auto,readability-qualified-auto)
        {
          str.insert(str.length(), pstr); // LCOV_EXCL_LINE
        }

        return str;
      }();

    using output_value_type = std::uint8_t;

    constexpr pi_spigot() = default; // LCOV_EXCL_LINE

    constexpr pi_spigot(const pi_spigot&) = delete;

    constexpr pi_spigot(pi_spigot&&) = delete;

    constexpr virtual ~pi_spigot() = default; // LCOV_EXCL_LINE

    constexpr auto operator=(const pi_spigot&) -> pi_spigot& = delete;

    constexpr auto operator=(pi_spigot&&) -> pi_spigot& = delete;

    static constexpr auto get_output_static_size() -> std::uint32_t
    {
      return result_digit;
    }

    static constexpr auto get_input_static_size() -> std::uint32_t
    {
      return input_scale(get_output_static_size());
    }

    [[nodiscard]] constexpr auto get_operation_count() const -> std::uintmax_t
    {
      return my_operation_count;
    }

    [[nodiscard]] constexpr auto get_output_digit_count() const -> std::uint32_t
    {
      return (std::min)(my_j, get_output_static_size());
    }

    template<typename InputIteratorType,
             typename OutputIteratorType>
    auto calculate(InputIteratorType  input_first, OutputIteratorType output_first) -> void
    {
      // Use pi_spigot::calculate() to calculate
      // result_digit decimal digits of pi.

      // The caller is responsible for providing both
      // input memory for the internal calculation details
      // as well as output memory for the result of pi.

      my_c               = UINT32_C(0);
      my_output_count    = UINT32_C(0);
      my_operation_count = UINTMAX_C(0);

      // Operation count Mathematica(R), example for loop_digit=9.
      // Sum[Floor[((d - j) (Floor[((10 9)/3)] + 1))/9], {j, 0, Floor[d/9] 9, 9}]
      for(my_j = UINT32_C(0); my_j < result_digit; my_j += loop_digit)
      {
        my_d = UINT64_C(0);

        auto i =
          static_cast<std::int32_t>
          (
            input_scale(result_digit - my_j) - INT32_C(1)
          );

        for( ; i >= INT32_C(0); --i)
        {
          const std::uint32_t di =
            ((my_j == UINT32_C(0)) ? d_init() : input_first[static_cast<std::uint32_t>(i)]);

          my_d +=
            static_cast<std::uint64_t>(static_cast<std::uint64_t>(di) * pow10(loop_digit));

          const auto b =
            static_cast<std::uint32_t>
            (
              static_cast<std::uint32_t>(static_cast<std::uint32_t>(i) * UINT32_C(2)) + UINT32_C(1)
            );

          input_first[static_cast<std::uint32_t>(i)] = static_cast<std::uint32_t>(my_d % b);

          my_d /= b;

          if(i > INT32_C(1))
          {
            my_d *= static_cast<std::uint32_t>(i);
          }

          ++my_operation_count;
        }

        do_extract_digit_group(output_first);
      }
    }

  private:
    std::uint32_t  my_c               = 0U;
    std::uint64_t  my_d               = 0U;
    std::uint32_t  my_j               = 0U;
    std::uintmax_t my_operation_count = 0U;
    std::uint32_t  my_output_count    = 0U;

    static constexpr auto input_scale(std::uint32_t x) -> std::uint32_t
    {
      return
        static_cast<std::uint32_t>
        (
          static_cast<std::uint32_t>(x * static_cast<std::uint32_t>((static_cast<std::uint32_t>(UINT32_C(10) * loop_digit) / UINT32_C(3)) + UINT32_C(1))) / loop_digit
        );
    }

    static constexpr auto pow10(std::uint32_t n) -> std::uint32_t
    {
      return ((n == UINT32_C(0)) ? UINT32_C(1) : pow10(n - UINT32_C(1)) * UINT32_C(10));
    }

    static constexpr auto d_init() -> std::uint32_t { return pow10(loop_digit) / UINT32_C(5); }

    template<typename OutputInputIterator>
    auto do_extract_digit_group(OutputInputIterator output_first) -> void
    {
      // Parse the next digits of pi, where the group has loop_digit digits.
      // If loop_digit is 4, for instance, then successive groups
      // of digits have a form such as: 3141, 5926, ..., etc.

      using local_output_input_iterator_type = OutputInputIterator;

      const auto next_digits =
        static_cast<std::uint32_t>
        (
          my_c + static_cast<std::uint32_t>(my_d / pow10(loop_digit))
        );

      my_c = static_cast<std::uint32_t>(my_d % pow10(loop_digit));

      const auto n =
        (std::min)
        (
          loop_digit,
          static_cast<std::uint32_t>(result_digit - my_j)
        );

      std::uint32_t scale10 = pow10(loop_digit - UINT32_C(1));

      for(auto i = static_cast<std::size_t>(0U); i < static_cast<std::size_t>(n); ++i)
      {
        using local_diff_type = typename std::iterator_traits<local_output_input_iterator_type>::difference_type;

        const auto out_index =
          static_cast<local_diff_type>
          (
            static_cast<std::size_t>(my_j) + i
          );

        output_first[out_index] =
          static_cast<output_value_type>
          (
            static_cast<std::uint32_t>(static_cast<std::uint32_t>(next_digits / scale10) % UINT32_C(10))
          );

        scale10 = static_cast<std::uint32_t>(scale10 / UINT32_C(10));
      }

      my_output_count += n;
    }
  };

  #if (__cplusplus >= 201703L)
  } // namespace math::constants
  #else
  } // namespace constants
  } // namespace math
  #endif

#endif // PI_SPIGOT_2022_06_08_H
