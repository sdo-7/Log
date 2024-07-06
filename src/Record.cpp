#include <map>
#include <sstream>
#include "InvalidDataError.hpp"
#include "Record.hpp"

Record::Record (const Record &that) {
  *this = that;
}

Record &Record::operator = (const Record &that) {
  content = that.content;
  time = that.time;
  type = that.type;
  id = that.id;

  return *this;
}

Record::Record(Record &&that) noexcept {
  *this = std::move(that);
}

Record &Record::operator = (Record &&that) noexcept {
  content = std::move(that.content);
  time = std::move(that.time);
  type = std::move(that.type);
  id = std::move(that.id);

  return *this;
}

bool Record::isStartRecord () const {
  return type == "Request started";
}

bool Record::isEndRecord () const {
  return type == "Request completed" ||
         type == "Request failed";
}

void Record::parse (std::istream &i) {
  std::getline(i, content, '\n');
  std::stringstream ss(content);

  std::string str;

  std::getline(ss, str, '\t');
  parseTime(str);

  ss.ignore(std::numeric_limits<std::streamsize>::max(), '\t');

  std::getline(ss, str, '(');
  parseType(str.substr(0, str.size() - 1));

  std::getline(ss, str, ')');
  parseDetails(str);
}

void Record::parseTime (const std::string &str) {
  static const char *format = "%4d-%2d-%2dT%2d:%2d:%2d,%6d";

  int year, month, day;
  int hours, minutes, seconds, microseconds;

  if (sscanf_s(str.c_str(), format,
               &year, &month, &day,
               &hours, &minutes, &seconds, &microseconds) != 7)
    throw InvalidDataError("could not parse time field");

  std::tm tm = {seconds, minutes, hours, day, month, year-1900};
  std::time_t time = std::mktime(&tm);
  this->time = {time, microseconds * 1000};
}

void Record::parseType (const std::string &str) {
  this->type = str;
}

void Record::parseDetails (const std::string &str) {
  static const char name[] = "TraceId:";
  const auto pos = str.find(name);
  if (pos == std::string::npos)
    throw InvalidDataError("could not parse traceId field");

  std::stringstream ss(str);
  ss.ignore(pos + sizeof(name)/sizeof(name[0]));

  ss >> this->id;
}
