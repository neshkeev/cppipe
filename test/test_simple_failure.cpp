#include <cppipe.h>
#include <cassert>

using namespace cppipe;

int main(int argc, char** argv)
{
  Result<int> start = 1;
  auto dec = [](auto&& i) -> Result<int> { return i - 1; };
  Result<int> failure = start
    >> dec
    >> dec
    >> [](auto&& i) { return i < 0 ? Result<int> { "negative value" } : i; }
    >> dec;

  std::string_view res = run(failure,
    [](auto&& i) { return ""; },
    [](auto&& err) { return err; });

  assert(res == "negative value");

  return 0;
}
