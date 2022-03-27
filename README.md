# Overview

Introduce the bash-like pipe functionality to C++. This is a header only library.

# Building

```
$ cd cppipe # local repository
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ ctest -C Debug # run tests if needed
$ sudo cmake --install .
```

# Example

```cpp
#include <cppipe.h>

using namespace cppipe;

int main(int argc, char** argv)
{
  Result<int> start = 1;
  auto inc = [](auto&& i) -> Result<int> { return i + 1; };
  Result<int> result = start
    >> [](auto&& i) -> Result<int> { return i * 2; }
    >> inc
    >> inc;

  int res = run(result,
    [](auto&& i) { return i; },
    [](auto&& err) { return -1; });

  ...
}
```
Please take a look at [tests](test/) for more
