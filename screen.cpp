#include <ctype.h>
#include <cassert>

#include "headers/text.hpp"
#include "headers/Cursor.hpp"
#include "headers/ui.hpp"

using std::string;
using UI::getWindow;

#define CTRL_KEY(x) ((x) & 0x1f)

enum KeyMap {
  DELETE = 127
};

void onExit() {
  UI::clear();
  endwin();
}

void initScreen() {
  initscr();
  raw();
  cbreak();
  noecho();
  scrollok(stdscr, true);

  getmaxyx(stdscr, UI::screenRows, UI::screenCols);
}

void refreshContent() {
  UI::refreshContent(Cursor::YLimits[0]);
}

void printLines(const std::vector<string> &lines, const size_t startIndex) {
  WINDOW *content = getWindow(UI::CONTENT);
  wclrtobot(content);
  for (size_t i = startIndex; i < lines.size(); i++) {
    mvwaddstr(content, i, 0, lines[i].c_str());
  }
}

void fromBuffer(string &data) {
  WINDOW* content = getWindow(UI::CONTENT);
  int input = 0;
  int cury = getcury(content);

  while (input != '\n') {
    input = wgetch(content);
    if(isprint(input)) {
      data += (char) input;
      waddch(content, input);
    } else if (input == DELETE) {
      int curx = getcurx(content);
      if(data.length() > 0) {
        data.pop_back();
        wmove(content, cury, curx - 1);
        wdelch(content);
      }
    }
    wrefresh(content);
  }
}

int main(int argc, const char **argv) {
  initScreen();
  std::atexit(onExit);
  WINDOW *contentPad;

  Text text;
  std::vector<string> &lines = text.lines;

  Cursor::YLimits[0] = 0;
  Cursor::YLimits[1] = UI::screenRows - 2;
  Cursor cursor(lines);

  UI::InfoData &infoData = UI::getInfoData();

  switch(argc) {
    case 1: {
      contentPad = UI::createContentPad();
      waddstr(contentPad, "Welcome to MyTextEditor! :)\n");
      waddstr(contentPad, "type the name of file to create.\n: ");
      refreshContent();
      string fileName;
      fileName.reserve(200);
      fromBuffer(fileName);
      text.filePath = fileName.c_str();
      werase(contentPad);
      refreshContent();
    } break;

    default: {
      std::string_view filePath = argv[1];
      size_t lastSlash = filePath.find_last_of('/');
      std::string_view fileName = filePath;
      if(lastSlash != string::npos) {
        fileName = filePath.substr(lastSlash + 1);
      }
      text.filePath = argv[1];
      bool extracted = text.extractLinesFromFile();

      if(extracted) {
        infoData.chars = text.countChars();
        contentPad = UI::createContentPad(lines.size());
        printLines(lines, 0);
      } else {
        contentPad = UI::createContentPad();
      }
      refreshContent();
    } break;
  }
  UI::createInfoBar(text.filePath);
  UI::refreshInfoBar();
  
  while (1) {
    int input = wgetch(contentPad);
    string &line = cursor.currentLine();

    switch (input) {
      case 27:
        return EXIT_SUCCESS;

      case KEY_UP:
        cursor.moveUp();
        break;

      case KEY_DOWN:
        cursor.moveDown();
        break;

      case KEY_RIGHT:
        cursor.moveRigth();
        break;

      case KEY_LEFT:
        cursor.moveLeft();
        break;
      
      case '\r':
      case '\n': {
        std::string_view view(line);
        lines.emplace(lines.begin() + cursor.y + 1, view.substr(cursor.x));
        string& line = cursor.currentLine();
        line.erase(line.begin() + cursor.x, line.end());
        cursor.moveDown();
        cursor.x = 0;
        printLines(lines, cursor.y - 1);
        infoData.chars++;
      } break;

      case DELETE:
        if(!cursor.inBegin()) {
          auto lineToRemove = lines.begin() + cursor.y;
          if (line.empty()) {
            lines.erase(lineToRemove); 
            cursor.moveUpEnd();
            printLines(lines, cursor.y);
          } else if (cursor.inBegin(line)) {
            cursor.moveUpEnd();
            cursor.currentLine().append(line);
            lines.erase(lineToRemove);
            printLines(lines, cursor.y);
          } else {
            auto charToRemove = line.begin() + cursor.x - 1;
            line.erase(charToRemove);

            wmove(contentPad, cursor.y, 0);
            wclrtoeol(contentPad);
            waddstr(contentPad, line.c_str());
            cursor.moveLeft();
          }
          infoData.chars--;
        }
        break;
      
      case KEY_HOME:
        cursor.x = 0;
        break;

      case KEY_END:
        cursor.x = line.length();
        break;
      
      case CTRL_KEY('s'):
        text.writeToFile();
      break;

      default:
        if (isprint(input)) {
          auto posToInsert = line.begin() + cursor.x;
          line.insert(posToInsert, input);
          wmove(contentPad, cursor.y, 0);
          wclrtoeol(contentPad);
          waddstr(contentPad, line.c_str());
          cursor.moveRigth();
          infoData.chars++;
        }
    }

    assert(wmove(contentPad, cursor.y, cursor.x) != ERR);
    infoData.line = cursor.y + 1;
    UI::updateInfoData();
    refreshContent();
  }
}