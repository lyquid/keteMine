/**
 * @file log.hpp
 * @author Alejandro Castillo Blanco (alexcastilloblanco@gmail.com)
 * @brief A log system.
 * @version 0.1
 * @date 2022-11-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#if !defined(KETEMINE_SRC_GUI_LOG_HPP_)
#define KETEMINE_SRC_GUI_LOG_HPP_

#include "../../lib/imgui/imgui.h"
#include <string>

namespace ktp { namespace gui {

class Log {
 public:
  Log() { clear(); }
  void add(const char* fmt, ...) IM_FMTARGS(2);
  void addError(const char* fmt, ...) IM_FMTARGS(2);
  void addWarning(const char* fmt, ...) IM_FMTARGS(2);
  void draw(const char* title);
 private:
  void clear();
  void saveLogFile(const std::string& file_name);
  ImGuiTextBuffer m_buf {};
  ImGuiTextFilter m_filter {};
  ImVector<int>   m_line_offsets {}; // Index to lines offset. We maintain this with add() calls.
  bool            m_auto_scroll {true};  // Keep scrolling if already at the bottom.
};

} } // namespace gui / ktp

#endif // KETEMINE_SRC_GUI_LOG_HPP_
