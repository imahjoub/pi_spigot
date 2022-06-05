pi_spigot
==================

[![Build Status](https://github.com/ckormanyos/pi_spigot/actions/workflows/pi_spigot.yml/badge.svg)](https://github.com/ckormanyos/pi_spigot/actions)
[![codecov](https://codecov.io/gh/ckormanyos/pi_spigot/branch/main/graph/badge.svg?token=Q3XV9QCH82)](https://codecov.io/gh/ckormanyos/pi_spigot)
[![Boost Software License 1.0](https://img.shields.io/badge/license-BSL%201.0-blue.svg)](https://github.com/ckormanyos/pi_spigot/blob/main/LICENSE_1_0.txt)
[![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ckormanyos/pi_spigot)](https://github.com/ckormanyos/pi_spigot)

This repository is work in progress.

## Mathematical details

pi_spigot calculates many decimal digits of the mathematical constant
![pi](https://latex.codecogs.com/svg.image?\pi).
It uses a slow quadratic
(order ![N2](https://latex.codecogs.com/svg.image?N^{2}))
spigot-type algorithm.

Spigot-type algorithms are known for various mathematical constants
including lograithms and
![pi](https://latex.codecogs.com/svg.image?\pi).

The basic spigot algorithm for computing digits of
![pi](https://latex.codecogs.com/svg.image?\pi)
in base-10 is given by
(Eq. 6.1, Sect. 6.1, page 78 of Arndt and Haenel [2]):

![spigotalgo](https://latex.codecogs.com/svg.image?\pi\,=\,2\,&plus;\,\dfrac{1}{3}\Biggl(2\,&plus;\,\dfrac{2}{5}\Biggl(2\,&plus;\,\dfrac{3}{7}\Biggl(2\,&plus;\,\ldots\Biggr)\Biggr)\Biggr))

In the code, this equation is primarily implemented in the
`calculate()` method of the `pi_spigot_single` template class
which resides in namespace `math::constants`.

## Using the `pi_spigot_single` template class

The signature of the `pi_spigot_single` template class is shown below.

```cpp
namespace math::constants {

template<const std::uint32_t ResultDigit,
         const std::uint32_t LoopDigit>
class pi_spigot_single;

}
```

The template parameters `ResultDigit` and `LoopDigit`
can be used to vary the size and granularity of the
calculation (within certain limits).

In order to compute, let's say,
![tenthousand](https://latex.codecogs.com/svg.image?10,001)
decimal digits of
![pi](https://latex.codecogs.com/svg.image?\pi),
the result of `ResultDigit` should be set to `10001`.

The `LoopDigit` parameter controls the number of decimal
digits calculated within each successive iterative loop
of the program. In the classic literature, this
parameter is often set to 4 and can not be varied.
in this repository, `LoopDigit` parameter can be set to a variable number
of digits ranging from about 4 through 9, where the
best performance is obtained for 9.

The internal mathematical code requires multiplication
within the running iteration. This code uses `uint64_t`
for internal result storage. For this reason,
the maximum value of `LoopDigit` is 9 so that
when it is multiplied, the result has about 18
decimal digits, which still _fits_ inside
`uint64_t` without overflow.

A C++ alias can be used to conveniently set the
digit characteristics of the spigot calculation
used in the This exact template instantiation is used in the
`pi_spigot_single` class. The default configuration
in the pi_spigot repository `10001` decimal digits
with digit granularity of `9`, as shown below.

```
using pi_spigot_type = math::constants::pi_spigot_single<10001U, 9U>;
```

This is done in two steps, whereby the
actual instantiation takes place in the
`test_pi_spigot_single()` template subroutine.

## Timing and memory consumption

TBD

## Testing, continuous integration and quality

TBD

## Possible extensions

TBD

## References

TBD More (and better) bibliographical formatting.

The expression of this algorithm (as used in this repository)
has been further influenced by descriptions
in the original publication in [1] and also by [2].

[1] S. Rabinowitz and S. Wagon:
_A_ _Spigot_ _Algorithm_ _for_ _the_ _Digits_ _of_ _Pi_,
American Mathematical Monthly 102: 195--203, 1995

[2] J. Arndt and C. Haenel:
![pi](https://latex.codecogs.com/svg.image?\pi) _Unleashed_
(Springer Verlag, Heidelberg, 2001)

The pi_spigot implementation in this repository
has been inspired by Section 10.8 of [3].

[3] C.M. Kormanyos: _Real-Time_ _C++:_
_Efficient_ _Object-Oriented_
_and_ _Template_ _Microcontroller_ Programming_, _Fourth_ _Edition_
(Springer, Heidelberg, 2021). ISBN 9783662629956.
