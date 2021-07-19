# ez-math

This library adds math constants, polynomial functions, trigonometric functions, and color classes.

Once the package is installed, it can be found using the cmake find_package command:
`find_package(ez-math CONFIG REQUIRED)`

The package provides the target `ez::math`, an interface target that specifies the location of the header files for usage.

The headers provided are:
```cpp
#include <ez/math/color.hpp>
#include <ez/math/constants.hpp>
#include <ez/math/complex.hpp>
#include <ez/math/poly.hpp>
#include <ez/math/trig.hpp>
```

Further documentation is provided by the [wiki](https://github.com/errata-c/ez-math/wiki).