#include "wordMap.h"
#include "utils.h"
#include <algorithm>
#include <iterator>
#include <queue>
#include <unordered_map>

Word::Word (std::string word_str) : word_str (word_str) {}
void
Word::addNext (std::shared_ptr<Word> p_word)
{
  auto ele = std::make_pair (p_word, std::make_shared<int> (1));
  auto it = next.find (ele);
  if (it == next.end ())
    next.insert (std::move (ele));
  else
    *(it->second) += 1;
  return;
}
void
Word::addPrev (std::shared_ptr<Word> p_word)
{
  auto ele = std::make_pair (p_word, std::make_shared<int> (1));
  auto it = prev.find (ele);
  if (it == prev.end ())
    prev.insert (std::move (ele));
  else
    *(it->second) += 1;
  return;
}
std::shared_ptr<Word>
Word::getRandomNext ()
{
  auto it = next.cbegin ();
  std::advance (it, utils::genRandomIndex (next.size ()));
  if (it == next.cend ())
    return nullptr;
  return it->first;
}
std::set<std::pair<std::shared_ptr<Word>, std::shared_ptr<int> >, WordComp> &
Word::getNext ()
{
  return this->next;
}
std::vector<std::pair<std::shared_ptr<Word>, std::shared_ptr<int> > >
Word::getBridgeWord (std::shared_ptr<Word> word)
{
  std::vector<std::pair<std::shared_ptr<Word>, std::shared_ptr<int> > > bridge;
  auto comp
      = [] (const std::pair<std::shared_ptr<Word>, std::shared_ptr<int> > &a,
            const std::pair<std::shared_ptr<Word>, std::shared_ptr<int> > &b) {
          return &*a.first > &*b.first;
        };
  std::set_intersection (
      next.cbegin (), next.cend (), word->prev.cbegin (), word->prev.cend (),
      std::insert_iterator<decltype (bridge)> (bridge, bridge.begin ()), comp);
  return std::move (bridge);
}
std::vector<std::pair<std::shared_ptr<Word>, int> >
shortestPath (std::shared_ptr<Word> begin, std::shared_ptr<Word> end)
{
  std::unordered_map<std::shared_ptr<Word>,
                     std::pair<std::shared_ptr<Word>, int> >
      prev;
  std::unordered_map<std::shared_ptr<Word>, int> distance;
  class Cmp
  {
  public:
    bool
    operator() (std::pair<std::shared_ptr<Word>, int> a,
                std::pair<std::shared_ptr<Word>, int> b)
    {
      return a.second >= b.second;
    }
  };
  std::priority_queue<std::pair<std::shared_ptr<Word>, int>,
                      std::vector<std::pair<std::shared_ptr<Word>, int> >, Cmp>
      heap;
  heap.push (std::make_pair (begin, 0x3f3f3f3f));
  while (!heap.empty ())
    {
      auto q = heap.top ();
      if (q.first == end)
        {
          break;
        }
      heap.pop ();
      if (distance.contains (q.first) && distance[q.first] < q.second)
        {
          continue;
        }
      for (auto next : q.first->getNext ())
        {
          int d = *next.second + q.second;
          if (distance.contains (next.first) && distance[next.first] <= d)
            continue;
          distance[next.first] = d;
          prev[next.first] = std::make_pair (q.first, *next.second);
          heap.push (std::make_pair (next.first, d));
        }
    }
  if (!prev.contains (end))
    return {};
  std::vector<std::pair<std::shared_ptr<Word>, int> > ret;
  while (prev.contains (end))
    {
      ret.push_back (prev[end]);
      end = prev[end].first;
      if (end == begin)
        break;
    }
  std::reverse (ret.begin (), ret.end ());
  return ret;
}


std::shared_ptr<Word>
WordMap::add (std::string word_str)
{
  auto word = map.find (word_str);
  if (word == map.cend ())
    {
      auto word = std::make_shared<Word> (word_str);
      auto ret = map.insert (std::make_pair (word_str, std::move (word)));
      if (!ret.second)
        throw "Insert word failed";
      return ret.first->second;
    }
  return word->second;
}
std::shared_ptr<Word>
WordMap::add (std::string word_src_str, std::string word_dest_str)
{
  auto word_src = this->map.find (word_src_str);
  if (word_src == this->map.cend ())
    throw "Existed word!";
  auto word_dest = this->add (word_dest_str);
  word_src->second->addNext (word_dest);
  word_dest->addPrev (word_src->second);
  return word_dest;
}

std::shared_ptr<Word>
WordMap::add (std::shared_ptr<Word> word_src, std::string word_dest_str)
{
  auto word_dest = this->add (word_dest_str);
  word_src->addNext (word_dest);
  word_dest->addPrev (word_src);
  return word_dest;
}

std::shared_ptr<Word>
WordMap::get (std::string word_str) const
{
  auto word = map.find (word_str);
  if (word == map.cend ())
    {
      return nullptr;
    }
  return word->second;
}

decltype (WordMap::map.cbegin ()) WordMap::begin () const
{
  return map.cbegin ();
}

decltype (WordMap::map.cend ()) WordMap::end () const { return map.cend (); }
std::size_t
WordMap::size () const
{
  return map.size ();
}
