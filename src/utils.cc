#include "utils.h"
#include "wordMap.h"
#include <algorithm>
#include <format>
#include <graphviz/gvc.h>
#include <memory>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <utility>
#include <vector>
namespace utils
{

int
genRandomIndex (int range)
{
  std::random_device rd;
  std::uniform_real_distribution<> distrib (0, range - 1);
  int index = distrib (rd);
  return index;
}

std::shared_ptr<Word> walk_status;
WordMap word_map;
void
showDirectedGraph (
    std::set<std::pair<std::shared_ptr<Word>, std::shared_ptr<Word> > > pth,
    std::string output_path)
{
  std::vector<std::shared_ptr<Word> > word_vec;
  std::vector<std::string> word_name_vec;
  std::map<std::shared_ptr<Word>, int> word_to_index;
  std::vector<std::pair<size_t, size_t> > edges;
  std::vector<size_t> w_edges;
  int count = 0;
  Agraph_t *g = agopen (const_cast<char *> ("G"), Agstrictdirected, nullptr);
  Agedge_t *edge;
  for (auto &kv : word_map)
    {
      word_vec.push_back (kv.second);
      word_name_vec.push_back (kv.first);
      word_to_index[kv.second] = count++;
      agnode (g, const_cast<char *> (kv.first.c_str ()), TRUE);
    }
  for (const auto word : word_vec)
    {
      for (const auto &word_dst : word->getNext ())
        {
          /*int src = word_to_index[word];
          int dst = word_to_index[word_dst.first];
          edges.push_back (std::make_pair (src, dst));
          w_edges.push_back (*word_dst.second);*/
          Agnode_t *out = agnode (
              g, const_cast<char *> (word->word_str.c_str ()), FALSE);
          Agnode_t *in = agnode (
              g, const_cast<char *> (word_dst.first->word_str.c_str ()),
              FALSE);
          edge = agedge (g, out, in, nullptr, TRUE);

          // attr = (myedge_t *)agbindrec(edge, "myedge_t", sizeof(myedge_t),
          // FALSE);
          agsafeset (
              edge, const_cast<char *> ("label"),
              const_cast<char *> (std::to_string (*word_dst.second).c_str ()),
              "");
          if (pth.contains (std::make_pair (word, word_dst.first)))
            {
              agsafeset (edge, const_cast<char *> ("color"), "red", "");
            }
          // attr->weight = *word_dst.second;
        }
    }
  /*auto g = plt::graph (edges);
g->edge_labels (w_edges);
g->node_labels (word_name_vec);
plt::plot(g);
plt::show ();*/
  GVC_t *gvc = gvContext ();
  agsafeset (g, const_cast<char *> ("dpi"), "100", "");
  gvLayout (gvc, g, "dot");
  gvRenderFilename (gvc, g, "png", output_path.c_str ());
  gvFreeLayout (gvc, g);
  agclose (g);
  gvFreeContext (gvc);
  return;
}
void
showDirectedGraph ()
{
  std::set<std::pair<std::shared_ptr<Word>, std::shared_ptr<Word> > > ts;
  showDirectedGraph (ts, "./graph.png");
}
std::string
queryBridgeWords (std::string word1_str, std::string word2_str)
{
  std::stringstream oss;
  word1_str = utils::string::tolower (word1_str);
  word2_str = utils::string::tolower (word2_str);
  if (!string::isValid (word1_str))
    return std::format ("\"{}\" is not a valid word", word1_str);
  if (!string::isValid (word2_str))
    return std::format ("\"{}\" is not a valid word", word2_str);
  auto word1 = word_map.get (word1_str);
  auto word2 = word_map.get (word2_str);
  if (!word1 && !word2)
    {
      return std::format ("No \"{}\" and \"{}\" in the graph!", word1_str,
                          word2_str);
    }
  if (!word1)
    {
      return std::format ("No \"{}\" in the graph!", word1_str);
    }
  if (!word2)
    {
      return std::format ("No \"{}\" in the graph!", word2_str);
    }
  auto bridge_words = word1->getBridgeWord (word2);
  if (bridge_words.empty ())
    {
      return std::format ("No bridge words from \"{}\" to \"{}\" !", word1_str,
                          word2_str);
    }
  auto end = bridge_words.back ();
  bridge_words.pop_back ();
  if (bridge_words.empty ())
    {
      oss << *end.first;
      return std::format ("The bridge words from \"{}\" to \"{}\" is: {}.",
                          word1_str, word2_str, oss.str ());
    }
  {
    for (auto &bridge : bridge_words)
      {
        oss << *bridge.first << ", ";
      }
    oss << "and " << *end.first;
    return std::format ("The bridge words from \"{}\" to \"{}\" are: {}.",
                        word1_str, word2_str, oss.str ());
  }
}
std::string
calcShortestPath (std::string word1_str, std::string word2_str)
{
  std::stringstream oss;
  std::set<std::pair<std::shared_ptr<Word>, std::shared_ptr<Word> > > pth;
  word1_str = utils::string::tolower (word1_str);
  word2_str = utils::string::tolower (word2_str);
  if (!string::isValid (word1_str))
    return std::format ("\"{}\" is not a valid word", word1_str);
  if (!string::isValid (word2_str))
    return std::format ("\"{}\" is not a valid word", word2_str);
  auto word1 = word_map.get (word1_str);
  auto word2 = word_map.get (word2_str);
  int length = 0;
  if (word1_str == word2_str)
    {

      if (!word1)
        return std::format ("No \"{}\" in the graph!", word1_str);
      return "Same Node!\nLength of path is 0";
    }
  if (!word1 && !word2)
    {
      return std::format ("No \"{}\" and \"{}\" in the graph!", word1_str,
                          word2_str);
    }
  if (!word1)
    {
      return std::format ("No \"{}\" in the graph!", word1_str);
    }
  if (!word2)
    {
      return std::format ("No \"{}\" in the graph!", word2_str);
    }
  auto path = shortestPath (word1, word2);
  if (path.empty ())
    return std::format ("No such a path from \"{}\" to \"{}\"", word1_str,
                        word2_str);
  std::shared_ptr<Word> prev_word = nullptr;
  for (auto edge : path)
    {
      auto word = edge.first;
      length += edge.second;
      pth.insert (std::make_pair (prev_word, word));
      prev_word = word;
      oss << *word << std::format (" ({}) ", edge.second);
    }
  pth.insert (std::make_pair (prev_word, word2));
  oss << word2_str;
  oss << std::endl;
  oss << std::format ("Length of path is: {}.", length) << std::endl;
  oss << "Saved Graph";
  showDirectedGraph (pth, "path.png");
  return oss.str ();
}
std::string
generateNewText (std::string text)
{
  std::stringstream oss;
  std::stringstream iss (std::move (text));
  std::string word_str;
  decltype (word_map.get (word_str)) p_prev = nullptr;
  while (iss >> word_str)
    {
      if (p_prev)
        {
          auto word = word_map.get (utils::string::tolower (word_str));
          if (word)
            {
              auto bridge_words = p_prev->getBridgeWord (word);
              if (!bridge_words.empty ())
                {
                  auto bridge = bridge_words.at (
                      genRandomIndex (bridge_words.size ()));
                  oss << *bridge.first << " ";
                }
            }
          p_prev = word;
          oss << word_str << " ";
        }
      else
        {
          p_prev = word_map.get (utils::string::tolower (word_str));
          oss << word_str << " ";
        }
    }
  return oss.str ();
}

void
clearRandomWalk ()
{
  utils::walk_status = nullptr;
}

std::string
randomWalk ()
{
  std::stringstream oss;
  static std::set<std::pair<std::shared_ptr<Word>, std::shared_ptr<Word> > >
      edge;
  if (!walk_status)
    {
      edge.clear ();
      auto it = word_map.begin ();
      if (it == word_map.end ())
        return "";
      std::advance (it, genRandomIndex (word_map.size ()));
      auto word = it->second;
      walk_status = it->second;
      oss << *walk_status;
      return oss.str ();
    }

  auto word = walk_status->getRandomNext ();
  if (!word)
    return "";
  if (edge.find (std::make_pair (walk_status, word)) == edge.end ())
    edge.insert (std::make_pair (walk_status, word));
  else
    return "";
  oss << *word;
  walk_status = word;
  return oss.str ();
}

namespace string
{
bool
isValid (std::string str)
{
  std::regex word ("[a-z]+");
  return std::regex_match (str, word);
}

std::string
tolower (std::string str)
{
  std::transform (str.begin (), str.end (), str.begin (), ::tolower);
  return str;
}
std::vector<std::string>
split (std::string line)
{
  std::regex num ("[0-9]");
  line = std::regex_replace (line, num, "");
  std::vector<std::string> ret;
  std::regex word ("[a-zA-Z]+");
  for (std::smatch sm; std::regex_search (line, sm, word);)
    {
      ret.push_back (tolower (sm.str ()));
      line = sm.suffix ();
    }
  return ret;
}

} // namespace string

void
readMap (std::basic_istream<char> &in)
{

  auto line = std::string ();
  decltype (word_map.add (line)) word_prev = nullptr;
  // while (std::cin >> str)
  //   {
  //     if (word_prev)
  //       word_prev = word_map.add (word_prev, str);
  //     else
  //       word_prev = word_map.add (str);
  //   }
  while (std::getline (in, line))
    {
      auto words = utils::string::split (line);
      for (auto str : words)
        {
          if (word_prev)
            word_prev = word_map.add (word_prev, str);
          else
            word_prev = word_map.add (str);
        }
    }
}

} // namespace utils
