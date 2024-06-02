#include "ui/newt.h"
#include <cstring>
#include <iostream>
#include <newt.h>
#include <string>

namespace ui
{

Newt::Newt (std::string name) : name (name) { newtInit (); }
Newt::~Newt () { newtFinished (); }
bool
Newt::option (std::string info)
{
  int ret = newtWinChoice (name.data (), std::string ("Yes").data (),
                           std::string ("No").data (), info.data (), nullptr);
  if (ret == 1)
    return true;
  return false;
}
void
Newt::message (std::string info)
{
  newtWinMessage (name.data (), std::string ("OK").data (), info.data (),
                  nullptr);
  return;
}
uint
Newt::menu (std::string info, std::vector<std::string> items)
{
  char **items_list = new char *[items.size () + 1];
  int i = 0;
  for (auto item : items)
    {
      items_list[i] = new char[item.size () + 1];
      strcpy (items_list[i], item.c_str ());
      i++;
    }
  items_list[i] = nullptr;
  int out;
  if (newtWinMenu (name.data (), info.data (), 100, 5, 5, 20, items_list, &out,
                   std::string ("OK").data (), std::string ("Exit").data (),
                   nullptr)
      == 2)
    {
      for (i = 0; i < items.size (); i++)
        {
          delete items_list[i];
        }
      delete[] items_list;
      return 0;
    }
  for (i = 0; i < items.size (); i++)
    {
      delete items_list[i];
    }
  delete[] items_list;
  return out + 1;
}
std::vector<std::string>
Newt::input (std::string info, std::vector<std::string> items)
{
  auto *li = new struct newtWinEntry[items.size () + 1];
  char *values[items.size ()];
  int i = 0;
  for (auto item : items)
    {

      li[i].text = new char[item.size () + 1];
      strcpy (li[i].text, item.c_str ());
      values[i] = new char[128];
      values[i][0] = '\0';
      li[i].value = &values[i];
      li[i].flags = 0;
      i++;
    }
  li[i].text = nullptr;
  li[i].value = nullptr;
  li[i].flags = 0;
  newtWinEntries (name.data (), info.data (), 100, 5, 5, 40, li,
                  std::string ("Commit").data (), nullptr);
  std::vector<std::string> ret;
  for (auto *value : values)
    {
      ret.push_back (std::string (value));
    }
  for (i = 0; i < items.size (); i++)
    {
      delete values[i];
      delete li[i].text;
    }
  delete[] li;
  return ret;
}

} // namespace ui
