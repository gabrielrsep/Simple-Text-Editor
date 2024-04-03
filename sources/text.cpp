#include "../headers/text.hpp"
#include <fstream>
#include <iostream>

Text::Text(): filePath(""), lines({""}) {
  lines.reserve(100);
}

bool Text::extractLinesFromFile() {
  std::ifstream file(filePath);
  std::string line;

  if(!file.is_open()) {
    return false;
  }
  lines.clear();

  while(std::getline(file, line)) {
    if(lines.size() == lines.capacity())
      lines.reserve(lines.size() + 20);
    lines.emplace_back(line);
  }
  return true;
}

std::size_t Text::countChars() const {
  std::size_t charsCount = lines.size() - 1;

  for (const std::string &line: lines) {
    charsCount += line.length();
  }
  return charsCount;
}

void Text::joinLines(std::string &text) const {
  text.reserve(countChars());

  for (const std::string &line: lines) {
    text.append(line);
    if(&line != &lines.back()) {
      text.append("\n");
    }
  }
}

bool Text::writeToFile() const {
  std::ofstream file(filePath);
  std::string text;
  joinLines(text);
  bool isOpen = file.is_open();
  if(isOpen) {
    file << text;
    file.close();
  }
  return isOpen;
}