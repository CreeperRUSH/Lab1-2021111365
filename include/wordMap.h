#pragma once
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
class Word;

struct WordComp
{
  bool
  operator() (
      const std::pair<std::shared_ptr<Word>, std::shared_ptr<int> > &a,
      const std::pair<std::shared_ptr<Word>, std::shared_ptr<int> > &b) const
  {
    return &*a.first > &*b.first;
  }
};
class Word
{
public:
  Word (std::string);
  ~Word () = default;
  friend std::ostream &
  operator<< (std::ostream &os, const Word &word)
  {
    os << word.word_str;
    return os;
  };
  void addNext (std::shared_ptr<Word>);
  void addPrev (std::shared_ptr<Word>);
  std::shared_ptr<Word> getRandomNext ();
  std::set<std::pair<std::shared_ptr<Word>, std::shared_ptr<int> >, WordComp> &
  getNext ();
  std::vector<std::pair<std::shared_ptr<Word>, std::shared_ptr<int> > >
      getBridgeWord (std::shared_ptr<Word>);

  std::string word_str;

private:
  std::set<std::pair<std::shared_ptr<Word>, std::shared_ptr<int> >, WordComp>
      next;
  std::set<std::pair<std::shared_ptr<Word>, std::shared_ptr<int> >, WordComp>
      prev;
};

class WordMap
{
private:
  std::map<std::string, std::shared_ptr<Word> > map;

public:
  WordMap () = default;
  ~WordMap () = default;
  std::shared_ptr<Word> add (std::string);
  std::shared_ptr<Word> add (std::string, std::string);
  std::shared_ptr<Word> add (std::shared_ptr<Word>, std::string);
  std::shared_ptr<Word> get (std::string) const;
  decltype (WordMap::map.cbegin ()) begin () const;
  decltype (WordMap::map.cend ()) end () const;
  std::size_t size () const;
};
std::vector<std::pair<std::shared_ptr<Word>, int> >
shortestPath (std::shared_ptr<Word> begin, std::shared_ptr<Word> end);
