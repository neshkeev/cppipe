#include <cppipe.h>
#include <cassert>

using namespace cppipe;

constexpr Result<int> twice(const int i)
{
  return i * 2;
}

int main(int argc, char** argv)
{
  const Result<int> start = 1;
  auto inc = [](auto&& i) -> Result<int> { return i + 1; };
  Result<int> success = start
    >> inc
    >> twice;

  int res = run(success,
    [](auto&& i) { return i; },
    [](auto&& err) { return -1; });

  assert(res == 4);

  return 0;
}
