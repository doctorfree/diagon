#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include <string>

std::string to_string(const std::wstring& s);
std::wstring to_wstring(const std::string& s);

class Screen {
 public:
  Screen(int width, int height);
  void DrawText(int x, int y, const std::wstring& text);
  void DrawBox(int x, int y, int w, int h);
  void DrawBoxedText(int x, int y, const std::wstring& text);
  std::string ToString();

 private:
  int width_;
  int height_;
  std::vector<std::wstring> lines_;
};

#endif /* end of include guard: SCREEN_H */