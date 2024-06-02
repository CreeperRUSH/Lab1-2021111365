#pragma once
#include <string>
#include <vector>
namespace ui
{
class UI
{
public:
  virtual bool option (std::string info) = 0;
  virtual void message (std::string info) = 0;
  virtual uint menu (std::string info, std::vector<std::string> items) = 0;
  virtual std::vector<std::string> input (std::string info,
                                          std::vector<std::string> items)
      = 0;
};
} // namespace ui
