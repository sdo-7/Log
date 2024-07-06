#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Log.hpp"
#include "InvalidDataError.hpp"

void parseArgs (const char *argv[], int &longestCount, std::string &filePath) {
  std::string longestCountStr(argv[1]);
  std::stringstream ss(longestCountStr);
  ss >> longestCount;

  filePath = std::string(argv[2]);
}

int main (int argc, const char *argv[]) try {
  if (argc != 3)
    throw std::runtime_error("log.exe COUNT FILE\nFor example:\nlog.exe 3 test.log");

  int longestCount = 0;
  std::string filePath;
  parseArgs(argv, longestCount, filePath);

  std::fstream file;
  file.open(filePath, std::ios_base::in);
  if (file.fail())
    throw std::runtime_error("could not open the file");

  Log log(longestCount);
  log.parse(file);

  for (auto rit = log.begin(), rend = log.end(); rit != rend; ++rit) {
    for (auto it = rit->begin(), end = rit->end(); it != end; ++it) {
      std::cout << *it << "\n";
    }
    std::cout << std::endl;
  }

  return 0;
} catch (const InvalidDataError &e) {
  std::cerr << e.what() << std::endl;
  return -2;
} catch (const std::runtime_error &e) {
  std::cerr << e.what() << std::endl;
  return -1;
}
