#pragma once
#include <string>
namespace utils
{
class Args
{
public:
  Args (int argc, char **argv);
  std::string filename = "";
  bool tui = false;
};
} // namespace utils
