#include <cppipe.h>

using namespace cppipe;

int main(int argc, char** argv)
{
  Result<int> start = 1;
  auto inc = [](auto&& i) -> Result<int> { return i + 1; };
  Result<int> success = start
    >> [](auto&& i) -> Result<int> { return i * 2; }
    >> inc
    >> inc;

  int res = run(success,
    [](auto&& i) { return i; },
    [](auto&& err) { return -1; });

  return res == 4 ? 0 : 1;
}
