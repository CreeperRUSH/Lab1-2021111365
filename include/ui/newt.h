#pragma once
#include "ui.h"

namespace ui
{

class Newt : public UI
{
public:
  Newt (std::string name);
  ~Newt ();
  bool option (std::string info);
  void message (std::string info);
  uint menu (std::string info, std::vector<std::string> items);
  std::vector<std::string> input (std::string info,
                                  std::vector<std::string> items);

private:
  std::string name;
};
} // namespace ui
