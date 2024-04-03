#pragma once
#include <vector>
#include <string>
#include <fstream>

struct Text {
  const char *filePath;
  std::vector<std::string> lines;
  Text();
  bool extractLinesFromFile();
  std::size_t countChars() const;
  bool writeToFile() const;
  private:
    void joinLines(std::string&) const;
};