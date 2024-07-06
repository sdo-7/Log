#include <sstream>
#include "InvalidDataError.hpp"
#include "Log.hpp"

Log::const_iterator::const_iterator (Log::Requests::const_iterator it, Requests::const_iterator endIt, size_t endPos)
: it (it)
, endIt (endIt)
, endPos (endPos) {
}

Log::const_iterator::const_iterator (Log::const_iterator &that)
: it (that.it) {
}

Log::const_iterator &Log::const_iterator::operator = (const Log::const_iterator &that) {
  it = that.it;
  endIt = that.endIt;
  pos = that.pos;
  endPos = that.endPos;
  return *this;
}

bool Log::const_iterator::operator == (const Log::const_iterator &that) const {
  const auto result = pos == that.pos &&
                      endPos == that.endPos &&
                      it == that.it &&
                      endIt == that.endIt;

  return result;
}

bool Log::const_iterator::operator != (const Log::const_iterator &that) const {
  const auto result = pos != that.pos ||
                      endPos != that.endPos ||
                      it != that.it ||
                      endIt != that.endIt;

  return result;
}

Log::const_iterator &Log::const_iterator::operator ++ () {
  ++pos;
  if (pos >= endPos) {
    pos = 0;
    it = endIt;
  } else
    ++it;

  return *this;
}

Log::const_iterator Log::const_iterator::operator ++ (int) {
  auto prev = it++;

  ++pos;
  if (pos >= endPos) {
    pos = 0;
    it = endIt;
  }

  return const_iterator(prev, endIt, endPos);
}

Log::const_iterator::reference Log::const_iterator::operator * () const {
  return *it;
}

Log::const_iterator::pointer Log::const_iterator::operator -> () const {
  return it.operator -> ();
}

Log::Log (int longestCount)
: longestCount (longestCount) {
}

void Log::parse (std::istream &i) {
  size_t line;
  try {
    for (line = 1; i.good(); ++line) {
        auto record = parseRecord(i);
        appendToRequest(record);
        sortRequests();
        removeShortRequests();
    }
  } catch (InvalidDataError &e) {
    throw InvalidDataError("error on line ", line, ": ", e.what());
  }
}

Request *Log::findRequest (const std::string &id) {
  const auto it = std::find(requests.begin(), requests.end(), id);
  if (it == requests.end())
    return nullptr;

  return &*it;
}

Record Log::parseRecord (std::istream &i) {
  Record record;
  i >> record;

  lastTime = record.getTime();

  return record;
}

void Log::appendToRequest (const Record &record) {
  Request *request = findRequest(record.getId());

  if (record.isStartRecord()) {
    if (request)
      throw InvalidDataError("the request with this traceId is already started");
    request = &requests.emplace_back();
  } else {
    if (!request)
      throw InvalidDataError("the request with this traceId is not started");
  }

  request->push_back(record);
}

void Log::sortRequests () {
  auto compare = [=] (const Request &a, const Request &b) {
    const auto ad = a.getDuration(this->lastTime);
    const auto bd = b.getDuration(this->lastTime);
    const auto result = bd < ad;
    return result;
  };

  requests.sort(compare);
}

void Log::removeShortRequests () {
  if (requests.size() <= longestCount)
    return;

  auto it = requests.begin();
  const auto end = requests.end();

  for (int i = 0; i < longestCount; ++i)
    ++it;
  if (it == end)
    return;

  while (it != end) {
    auto curit = it++;

    if (curit->isEnded())
      requests.erase(curit);
  }
}
