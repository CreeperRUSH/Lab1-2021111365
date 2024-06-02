#pragma once
#include "ui.h"
namespace ui
{
class Text : public UI
{
public:
  bool option (std::string info);
  void message (std::string info);
  uint menu (std::string info, std::vector<std::string> items);
  std::vector<std::string> input (std::string info,
                                  std::vector<std::string> items);
};
} // namespace ui
