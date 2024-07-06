#include <sstream>
#include "InvalidDataError.hpp"
#include "Request.hpp"

Request::const_iterator::const_iterator (Records::const_iterator it)
: it (std::move(it)) {
}

Request::const_iterator::const_iterator (const const_iterator &that)
: it (that.it) {
}

Request::const_iterator &Request::const_iterator::operator = (const const_iterator &that) {
  it = that.it;
  return *this;
}

bool Request::const_iterator::operator == (const const_iterator &that) const {
  return it == that.it;
}

bool Request::const_iterator::operator != (const const_iterator &that) const {
  return it != that.it;
}

Request::const_iterator &Request::const_iterator::operator ++ () {
  ++it;
  return *this;
}

Request::const_iterator Request::const_iterator::operator ++ (int) {
  auto prev = it++;
  return prev;
}

Request::const_iterator::reference Request::const_iterator::operator * () const {
  return *it;
}

Request::const_iterator::pointer Request::const_iterator::operator -> () const {
  return it.operator -> ();
}

Request::Request (const Record &startRecord) {
  push_back(startRecord);
}

Request::Request (const Request &that) {
  *this = that;
}

Request &Request::operator = (const Request &that) {
  records = that.records;
  startTime = that.startTime;
  lastTime = that.lastTime;
  id = that.id;
  ended = that.ended;

  return *this;
}

Request::Request (Request &&that) noexcept {
  *this = std::move(that);
}

Request &Request::operator = (Request &&that) noexcept {
  records = std::move(that.records);
  startTime = std::move(that.startTime);
  lastTime = std::move(that.lastTime);
  id = std::move(that.id);
  ended = std::move(that.ended);

  return *this;
}

std::timespec Request::getDuration (const std::timespec &lastTime) const {
  if (records.empty())
    return {0};

  std::timespec duration = !ended ? lastTime :
                                    this->lastTime;

  if (duration.tv_nsec < startTime.tv_nsec) {
    duration.tv_nsec = 1'000'000'000 - startTime.tv_nsec + duration.tv_nsec;
    duration.tv_sec -= 1;
  } else
    duration.tv_nsec -= startTime.tv_nsec;

  duration.tv_sec -= startTime.tv_sec;

  return duration;
}

void Request::push_back (const Record &record) {
  if (isEnded())
    throw InvalidDataError("the request is already ended");
  else if (record.getTime() < lastTime)
    throw InvalidDataError("time field value of the record is less than previous one");

  if (record.isStartRecord()) {
    startTime = record.getTime();
    id = record.getId();
  } else if (record.isEndRecord()) {
    ended = true;
  }

  records.emplace_back(record.getContent());
  lastTime = record.getTime();
}

bool operator < (const std::timespec &a, const std::timespec &b) {
  if (b.tv_sec == 0 && b.tv_nsec == 0)
    return false;

  if (a.tv_sec == b.tv_sec)
    return a.tv_nsec < b.tv_nsec;
  return a.tv_sec < b.tv_sec;
}

bool operator == (const Request &request, const std::string &id) {
  return request.getId() == id;
}
