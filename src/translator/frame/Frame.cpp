#include <sstream>
#include <vector>

#include "screen/Screen.h"
#include "translator/Translator.h"

class Frame : public Translator {
 public:
  virtual ~Frame() = default;
  virtual std::string operator()(const std::string& input,
                                 const std::string& options_string) {
    auto options = SerializeOption(options_string);

    bool ascii_only = false;
    if (options.count("ascii_only") && options.at("ascii_only") == "true")
      ascii_only = true;

    bool line_number = true;
    if (options.count("line_number") && options.at("line_number") == "false")
      line_number = false;

    // cut by lines.
    std::stringstream ss(input);
    std::vector<std::wstring> lines;
    std::string line;
    while (std::getline(ss, line)) {
      lines.push_back(to_wstring(line));
    }

    int number_length = 0;
    int max_number = 1;
    while (max_number <= (int)lines.size()) {
      max_number *= 10;
      number_length++;
    }

    int text_max_width = 0;
    for (const auto& line : lines) {
      text_max_width = std::max(text_max_width, (int)line.size());
    }

    int width, height, text_x;
    if (line_number) {
      width = number_length + text_max_width + 3;
      height = lines.size() + 2 + ascii_only;
      text_x = number_length + 2;
    } else {
      width = text_max_width + 2;
      height = lines.size() + 2 + ascii_only;
      text_x = 1;
    }
    int text_y = ascii_only ? 2 : 1;

    Screen screen(width, height);

    // Draw text.
    for (int y = 0; y < lines.size(); ++y) {
      screen.DrawText(text_x, text_y + y, lines[y]);
    }

    // Draw line number.
    if (line_number) {
      for (int y = 0; y < lines.size(); ++y) {
        screen.DrawText(1, text_y + y, to_wstring(std::to_string(y + 1)));
      }
    }

    // Draw box.
    if (ascii_only) {
      screen.DrawHorizontalLine(1, width - 2, 0, U'_');
      screen.DrawHorizontalLine(1, width - 2, height - 1, U'_');
      screen.DrawVerticalLine(1, height - 1, 0);
      screen.DrawVerticalLine(1, height - 1, width - 1);
    } else {
      screen.DrawBox(0, 0, width, height);
    }

    // Draw the line number separator.
    if (line_number) {
      if (ascii_only) {
        screen.DrawVerticalLine(1, height - 1, number_length + 1, U'|');
      } else {
        screen.DrawPixel(number_length + 1, 0, U'┬');
        screen.DrawVerticalLine(1, height - 1, number_length + 1);
        screen.DrawPixel(number_length + 1, height - 1, U'┴');
      }
    }

    return screen.ToString();
  }
};

std::unique_ptr<Translator> FrameTranslator() {
  return std::make_unique<Frame>();
}