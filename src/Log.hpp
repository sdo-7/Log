#pragma once

#include <ctime>
#include <istream>
#include <list>
#include "Request.hpp"

class Log {
  using Requests = std::list< Request >;

  const int longestCount;
  Requests requests;
  std::timespec lastTime = {0};

public:
  class const_iterator {
    Requests::const_iterator it;
    Requests::const_iterator endIt;
    std::size_t pos = 0;
    std::size_t endPos = 0;

  public:
    using value_type        = Requests::value_type;
    using reference         = Requests::const_reference;
    using pointer           = Requests::const_pointer;
    using iterator_category = std::input_iterator_tag;

  public:
    const_iterator () = default;
    const_iterator (Requests::const_iterator it, Requests::const_iterator endIt, size_t endPos = 0);
    const_iterator (const_iterator &that);
    const_iterator &operator = (const const_iterator &that);

    bool operator == (const const_iterator &that) const;
    bool operator != (const const_iterator &that) const;
    const_iterator &operator ++ ();
    const_iterator operator ++ (int);
    reference operator * () const;
    pointer operator -> () const;
  } ;

public:
  Log (int longestCount = 5);

  inline const_iterator begin () const {
    return const_iterator(requests.begin(), requests.end(), longestCount);
  }
  inline const_iterator cbegin () const {
    return const_iterator(requests.cbegin(), requests.cend(), longestCount);
  }
  inline const_iterator end () const {
    return const_iterator(requests.end(), requests.end(), longestCount);
  }
  inline const_iterator cend () const {
    return const_iterator(requests.cend(), requests.cend(), longestCount);
  }
  void parse (std::istream &i);
private:
  Request *findRequest (const std::string &id);
  Record parseRecord (std::istream &i);
  void appendToRequest (const Record &record);
  void sortRequests ();
  void removeShortRequests ();
} ;
