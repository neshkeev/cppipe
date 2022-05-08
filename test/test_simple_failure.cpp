#include <cppipe.h>
#include <cassert>

using namespace cppipe;

std::string_view neg_value_msg = "negative value";

int main(int argc, char** argv)
{
  Result<int> start = 1;
  auto dec = [](auto&& i) -> Result<int> { return i - 1; };
  Result<int> failure = start
    >> dec
    >> dec
    >> [](auto&& i) { return i < 0 ? Result<int> { neg_value_msg } : i; }
    >> dec;

  run(failure,
    [](auto&& i) { assert(false); },
    [](auto&& err) { assert(err == neg_value_msg); }
  );

  return 0;
}
