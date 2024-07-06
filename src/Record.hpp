#pragma once

#include <ctime>
#include <istream>
#include <string>

class Record {
  std::string content;
  std::timespec time;
  std::string type;
  std::string id;
  
public:
  Record () = default;
  Record (const Record &that);
  Record &operator = (const Record &that);
  Record (Record &&that) noexcept;
  Record &operator = (Record &&that) noexcept;
  
  inline const std::string &getContent () const {
    return content;
  }
  inline const std::timespec &getTime () const {
    return time;
  }
  inline const std::string &getId () const {
    return id;
  }
  bool isStartRecord () const;
  bool isEndRecord () const;
  void parse (std::istream &i);
private:
  void parseTime (const std::string &str);
  void parseType (const std::string &str);
  void parseDetails (const std::string &str);
} ;

inline std::istream &operator >> (std::istream &i, Record &record) {
  record.parse(i);
  return i;
}

inline std::ostream &operator << (std::ostream &o, const Record &record) {
  o << record.getContent();
  return o;
}
