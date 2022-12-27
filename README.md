pi_spigot
==================

<p align="center">
    <a href="https://github.com/imahjoub/pi_spigot/actions">
        <img src="https://github.com/imahjoub/pi_spigot/actions/workflows/pi_spigot.yml/badge.svg" alt="Build Status"></a>
    <a href="https://github.com/imahjoub/pi_spigot/issues?q=is%3Aissue+is%3Aopen+sort%3Aupdated-desc">
        <img src="https://custom-icon-badges.herokuapp.com/github/issues-raw/imahjoub/pi_spigot?logo=github" alt="Open Issues" /></a>
    <a href="https://github.com/imahjoub/pi_spigot/actions/workflows/pi_spigot_codeql.yml">
        <img src="https://github.com/imahjoub/pi_spigot/actions/workflows/pi_spigot_codeql.yml/badge.svg" alt="CodeQL"></a>
    <a href="https://app.codacy.com/gh/imahjoub/pi_spigot/dashboard">
        <img src="https://app.codacy.com/project/badge/Grade/4f390d179be84d87a637adbf5bd473bc" alt="Codacy Badge" /></a>
    <a href="https://sonarcloud.io/summary/new_code?id=imahjoub_pi_spigot">
        <img src="https://sonarcloud.io/api/project_badges/measure?project=imahjoub_pi_spigot&metric=alert_status" alt="Quality Gate Status"></a>
    <a href="https://codecov.io/gh/imahjoub/pi_spigot">
        <img src="https://codecov.io/gh/imahjoub/pi_spigot/branch/main/graph/badge.svg?token=3LIK8E96FC" alt="code coverage"></a>
    <a href="https://github.com/imahjoub/pi_spigot" alt="GitHub code size in bytes">
        <img src="https://img.shields.io/github/languages/code-size/imahjoub/pi_spigot" /></a>
    <a href="https://github.com/imahjoub/pi_spigot/blob/main/LICENSE_1_0.txt">
        <img src="https://img.shields.io/badge/license-BSL%201.0-blue.svg" alt="Boost Software License 1.0"></a>
    <a href="https://github.com/imahjoub/pi_spigot" alt="Activity">
        <img src="https://img.shields.io/github/commit-activity/y/imahjoub/pi_spigot" /></a>
</p>

The pi_spigot repository implements
a spigot-type algorithm in modern C++ template code.
It calculates many decimal
digits of the mathematical constant $\pi$.

The spigot-type algorithm used here has rather slow quadratic
order $N^{2}$ computational complexity.
So you won't break
any digit or speed records with this implementation.
It is, however, capable of readily computing many thousands
of digits (up to a $\sim 100,000$ decimal digits or more)
of $\pi$ - right out of the box and with just a few lines
of computational code.

The default setting of the tests computes
$100,001$ decimal digits of $\pi$.

## Mathematical details

Spigot-type algorithms are known for various mathematical constants
including logarithms and $\pi$.

The basic spigot algorithm for computing digits of $\pi$
in base-10 is given by
(Eq. 6.1, Sect. 6.1, page 78 of Arndt and Haenel [2]):

$$\dfrac{1}{3}\Biggl(2 + \dfrac{2}{5}\Biggl( 2 + \dfrac{3}{7}\Biggl(2 + \ldots \Biggr)\Biggr)\Biggr)$$

In the code, this equation is primarily implemented in the
`calculate()` method of the `pi_spigot` template class
which resides in namespace `math::constants`.

## Using the `pi_spigot` template class

The signature of the `pi_spigot` template class is shown below.

```cpp
namespace math::constants {

template<const std::uint32_t ResultDigit,
         const std::uint32_t LoopDigit>
class pi_spigot;

}
```

The template parameters `ResultDigit` and `LoopDigit`
can be used to vary the size and granularity of the
calculation (within certain limits).

In order to compute, let's say,
$10,001$ decimal digits of $\pi$,
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
digit characteristics used in the spigot calculation.

```cpp
using pi_spigot_type = math::constants::pi_spigot<100001U, 9U>;
```

The default configuration
in the pi_spigot repository uses `100001` decimal digits
with digit granularity of `9`, as shown.
The exact template instantiation is actually done in two steps in the code
(via secondary subroutine). The instantiation actually
takes place with named parameters in the `test_pi_spigot()`
template subroutine.

## Timing and memory consumption

Timing and memory consumption are provided in closed form equations
in [3]. In that work, we find empirically that the memory grows linearly
with input size while the computational complexity grows
quadratically.

The table below lists memory consumption and computation time
on a PC platform running GCC 9 with instantiation of
`pi_spigot<N, 9U>`, where `N` varies.

|N (digits)  | Memory Consumption | Operation Count  | Time (s) |
| ---------- | ------------------ | ---------------- | -------- |
| 10,001     | 137,788            | 19,155,868       |  0.23    |
| 50,001     | 688,900            | 478,496,610      |  5.2     |
| 100,001    | 1,377,788          | 1,913,780,868    |  21      |

## Testing, continuous integration and quality

Testing and continuous integration runs on GitHub Actions.
Various OS/compiler combinations are used including
GCC/clang/MSVC.

Code coverage uses GCC/gcov/lcov and has a
quality-gate with comparison/baseline-check provided by third-party
[Codecov](https://app.codecov.io/gh/imahjoub/pi_spigot).

Additional quality checks are performed on pull-request
and merge to master using modern third party open-source services.
These include
[LGTM](https://lgtm.com/projects/g/imahjoub/pi_spigot/alerts/?mode=list)
and [CodeSonar](https://sonarcloud.io/summary/new_code?id=imahjoub_pi_spigot).

## Possible extensions

The pi-spigot algorithm in this repository can successfully
compute up to a million digits of $\pi$.
It is, however, very slow with computational complexity
growing quadratically in proportion to digit size.

Looking at the implementation of the algorithm, however,
it might be possible to parallelize some or part of the
algorithmic loops. If this is possible, a modern GPU
massive parallelization would reduce the calculation
time dramatically.

At the moment, the `LoopDigit` parameter is limited to
9 decimal digits due to the size of multiplication
which needs to remain bounded within `uint64_t`.
Possible extension to 128-bit or multiple-precision
integral types would allow for much larger
digit counts per iteration, however, at the
cost of higher time for the individual operations.

## References

The original publication of the description of the spigot algorithm
can be found in [1].
The expression of this algorithm (as used in this repository)
has been further influenced by descriptions in [2].
The pi_spigot implementation in C++ code in this repository
has been inspired by Section 10.8 of [3].

[1] S. Rabinowitz and S. Wagon:
_A_ _Spigot_ _Algorithm_ _for_ _the_ _Digits_ _of_ _Pi_,
American Mathematical Monthly 102: 195-203, 1995

[2] J. Arndt and C. Haenel:
$\pi$ _Unleashed_ (Springer Verlag, Heidelberg, 2001)

[3] C.M. Kormanyos: _Real-Time_ _C++:_
_Efficient_ _Object-Oriented_
_and_ _Template_ _Microcontroller_ _Programming_, _4th_ _Edition_
(Springer Verlag, Heidelberg, 2021). ISBN 9783662629956.
