#pragma once
#include <string>
#include <vector>

namespace utils
{
int genRandomIndex (int range);

void showDirectedGraph ();
std::string queryBridgeWords (std::string word1_str, std::string word2_str);
std::string calcShortestPath (std::string word1_str, std::string word2_str);
std::string generateNewText (std::string text);
std::string randomWalk ();
namespace string
{
  bool isValid (std::string str);
  std::string tolower (std::string str);
  std::vector<std::string> split (std::string line);

} // namespace string

void readMap (std::basic_istream<char> &in);
void clearRandomWalk ();
} // namespace utils
