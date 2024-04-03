#include "../headers/ui.hpp"
#include <array>

static std::array<WINDOW *, 2> allWindows = { NULL, NULL };
static int initInfoData;
static UI::InfoData infoData;

int UI::screenCols, UI::screenRows;

UI::InfoData::InfoData(): chars(0), line(0) {}

void iUpdateInfoData(bool refresh) {
  using UI::getWindow, UI::refreshInfoBar, UI::INFO_BAR;
  using std::to_string;

  WINDOW *infoBar = getWindow(INFO_BAR);
  wmove(infoBar, 0, initInfoData);
  wclrtoeol(infoBar);
  waddstr(infoBar, to_string(infoData.chars).c_str());
  waddch(infoBar, '\t');
  waddstr(infoBar, to_string(infoData.line).c_str());
  if(refresh)
    refreshInfoBar();
}

void UI::updateInfoData() {
  iUpdateInfoData(true);
}

WINDOW* UI::getWindow(WindowID id) {
  return allWindows[id];
}

UI::InfoData& UI::getInfoData() {
  return infoData;
}

WINDOW* UI::createContentPad() {
  return createContentPad(screenRows);
}

WINDOW* UI::createContentPad(size_t lines) {
  WINDOW *pad = newpad(lines, screenCols);
  if (pad == nullptr) {
    std::string msg = "cannot create content pad. lines: " + std::to_string(lines);
    std::__throw_runtime_error(msg.c_str());
  }
  keypad(pad, true);
  scrollok(pad, true);
  allWindows[CONTENT] = pad;
  return pad;
}

void UI::createInfoBar(std::string_view fileName) {
  WINDOW *container = newwin(1, 28, screenRows - 1, 0);
  wattron(container, A_REVERSE);
  waddch(container, '"');
  waddstr(container, fileName.data());
  waddch(container, '"');
  wattroff(container, A_REVERSE);
  waddch(container, '\t');
  initInfoData = getcurx(container) + 1;
  allWindows[INFO_BAR] = container;
  iUpdateInfoData(false);
}

void UI::refreshInfoBar() {
  wrefresh(getWindow(INFO_BAR));
}

void UI::refreshContent(int minY) {
  int maxScreenY = screenRows - 2;
  prefresh(getWindow(CONTENT), minY, 0, 0, 0, maxScreenY, screenCols);
}

void UI::clear() {
  for (WINDOW *win : allWindows)
    if(win != NULL)
      delwin(win);
}