#include "../headers/Cursor.hpp"

using std::string, std::vector;

std::size_t Cursor::YLimits[2];

#define SPAN 2
#define CURRENT_LINE_LENGTH currentLine().length()

enum ScrollTo { UP = -SPAN, DONW = SPAN };

void scroll(ScrollTo scrollTo) {
 for(int i = 0; i < 2; i++) {
  Cursor::YLimits[i] += scrollTo;
 }
}

Cursor::Cursor(vector<string> &lines):
  x(0), y(0), lines(lines)
{}

void Cursor::moveRigth() {
  if(inDown(true)) {
    scroll(DONW);
    moveRigth();
  } else if(x == currentLine().length() && lines.size() > 1) {
    y++;
    x = 0;
  } else if (x < currentLine().length()) {
    x++;
  }
}

bool Cursor::inDown(bool rigth) const {
  bool maxY = y == YLimits[1];
  return rigth ? maxY && x == currentLine().length() : maxY;
}

void Cursor::moveDown() {
  if(inDown(false)) {
    scroll(DONW);
  } else if(y + 1 < lines.size()) {
    y++;
    if(x > currentLine().length()) {
      x = currentLine().length();
    }
  }
}

void Cursor::moveUp() {
  if(!inBegin()) {
    if(y == YLimits[0])
      scroll(UP);
    else
      y--;
  }
}

void Cursor::moveUpEnd() {
  moveUp();
  x = currentLine().length();
}

void Cursor::moveLeft() {
  if(!inBegin()) {
    if(x == 0 && y == YLimits[0]) {
      scroll(UP);
    } else if (x == 0 && y > 0) {
      moveUpEnd();
    } else x--;
  }
}

bool Cursor::inBegin() const {
  return x == 0 && y == 0;
}

bool Cursor::inBegin(const string& line) const {
  return x == 0 && &currentLine() == &line;
}

string& Cursor::currentLine() const {
  return lines.at(y);
}