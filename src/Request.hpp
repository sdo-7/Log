#pragma once

#include <ctime>
#include <vector>
#include "Record.hpp"

class Request {
  using Records = std::vector< std::string >;
  Records records;
  std::timespec startTime = {0};
  std::timespec lastTime = {0};
  std::string id;
  bool ended = false;

public:
  class const_iterator {
    Records::const_iterator it;

  public:
    using value_type        = Records::value_type;
    using reference         = Records::const_reference;
    using pointer           = Records::const_pointer;
    using iterator_category = std::input_iterator_tag;

  public:
    const_iterator () = default;
    const_iterator (Records::const_iterator it);
    const_iterator (const const_iterator &that);
    const_iterator &operator = (const const_iterator &that);

    bool operator == (const const_iterator &that) const;
    bool operator != (const const_iterator &that) const;
    const_iterator &operator ++ ();
    const_iterator operator ++ (int);
    reference operator * () const;
    pointer operator -> () const;
  } ;

public:
  Request () = default;
  Request (const Record &startRecord);
  Request (const Request &that);
  Request &operator = (const Request &that);
  Request (Request &&that) noexcept;
  Request &operator = (Request &&that) noexcept;

  inline const std::string &getId () const {
    return id;
  }
  inline const std::timespec &getStartTime () const {
    return startTime;
  }
  inline const std::timespec &getLastTime () const {
    return lastTime;
  }
  inline bool isEnded () const {
    return ended;
  }
  inline const_iterator begin () const {
    return const_iterator(records.begin());
  }
  inline const_iterator cbegin () const {
    return const_iterator(records.cbegin());
  }
  inline const_iterator end () const {
    return const_iterator(records.end());
  }
  inline const_iterator cend () const {
    return const_iterator(records.cend());
  }
  std::timespec getDuration (const std::timespec &lastTime) const;
  void push_back (const Record &record);
} ;

bool operator < (const std::timespec &a, const std::timespec &b);

bool operator == (const Request &request, const std::string &id);
