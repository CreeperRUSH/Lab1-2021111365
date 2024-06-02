#include "ui/text.h"
#include <format>
#include <iostream>

namespace ui
{

bool
Text::option (std::string info)
{
  std::string input;
  std::cout << info << "(Y/n): ";
  std::getline (std::cin, input);
  if (input == "n" || input == "N")
    {
      return false;
    }
  return true;
}
void
Text::message (std::string info)
{
  std::cout << info << std::endl;
}

uint
Text::menu (std::string info, std::vector<std::string> items)
{
  std::cout << info << std::endl;
  uint i = 1, opt = 0;
  std::string input;
  for (auto item : items)
    {
      std::cout << i << ". " << item << std::endl;
      i++;
    }
  std::cout << "Input your option: ";
  std::getline (std::cin, input);
  try
    {
      opt = std::stoi (input);
    }
  catch (...)
    {
      opt = 0;
    }
  if (opt >= i)
    opt = 0;
  return opt;
}
std::vector<std::string>
Text::input (std::string info, std::vector<std::string> items)
{
  std::vector<std::string> ret;
  std::cout << info << std::endl;
  for (auto item : items)
    {
      std::string input;
      std::cout << std::format ("{}: ", item);
      std::getline (std::cin, input);
      ret.push_back (std::move (input));
    }
  return ret;
}

} // namespace ui
