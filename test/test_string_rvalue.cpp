#include <cppipe.h>
#include <string_view>
#include <utility>
#include <cassert>

using namespace cppipe;

std::string_view res = "Hello";

constexpr auto to_str(const int i) -> Result<std::string_view>
{
  return Result<std::string_view> { std::in_place_index<1>, res };
}

int main(int argc, char** argv)
{
  Result<int> start = 1;
  auto inc = [](auto&& i) -> Result<int> { return i + 1; };
  auto success = start
    >> inc
    >> to_str;

  run(success,
    [](auto&& msg) { assert(msg == res); },
    [](auto&& err) { assert(false); });

  return 0;
}
