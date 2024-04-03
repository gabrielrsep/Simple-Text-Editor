#pragma once
#include <vector>
#include <string>

struct Cursor {
  static size_t YLimits[2];
  size_t x, y;
  Cursor(std::vector<std::string>&);
  void moveLeft();
  void moveRigth();
  void moveUp();
  void moveUpEnd();
  void moveDown();
  bool inBegin() const;
  bool inBegin(const std::string&) const;
  bool inDown(bool) const;
  std::string& currentLine() const;
  private:
    std::vector<std::string>& lines;
};
