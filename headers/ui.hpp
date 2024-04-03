#pragma once
#include <ncurses.h>
#include <string>

namespace UI {
  extern int screenRows, screenCols;
  enum WindowID { CONTENT, INFO_BAR };
  struct InfoData {
    size_t chars, line;
    InfoData();
  };
  WINDOW* createContentPad();
  WINDOW* createContentPad(size_t);
  void createInfoBar(std::string_view);
  void refreshInfoBar();
  void updateInfoData();
  void refreshContent(int);
  WINDOW* getWindow(WindowID);
  InfoData& getInfoData();
  void clear();
}