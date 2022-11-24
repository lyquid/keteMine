#include "log.hpp"

#include <string>

void ktp::gui::Log::add(const char* fmt, ...) {
  auto old_size {m_buf.size()};
  va_list args {};
  va_start(args, fmt);
  m_buf.appendfv(fmt, args);
  va_end(args);

  for (auto new_size = m_buf.size(); old_size < new_size; ++old_size) {
    if (m_buf[old_size] == '\n')
      m_line_offsets.push_back(old_size + 1);
  }
}

void ktp::gui::Log::addError(const char* fmt, ...) {
  auto old_size {m_buf.size()};
  std::string final_text {fmt};
  final_text = "[ERROR] " + final_text;
  va_list args {};
  va_start(args, fmt);
  m_buf.appendfv(final_text.c_str(), args);
  va_end(args);

  for (auto new_size = m_buf.size(); old_size < new_size; ++old_size) {
    if (m_buf[old_size] == '\n')
      m_line_offsets.push_back(old_size + 1);
  }
}

void ktp::gui::Log::addWarning(const char* fmt, ...) {
  auto old_size {m_buf.size()};
  std::string final_text {fmt};
  final_text = "[WARNING] " + final_text;
  va_list args {};
  va_start(args, fmt);
  m_buf.appendfv(final_text.c_str(), args);
  va_end(args);

  for (auto new_size = m_buf.size(); old_size < new_size; ++old_size) {
    if (m_buf[old_size] == '\n')
      m_line_offsets.push_back(old_size + 1);
  }
}

void ktp::gui::Log::clear() {
  m_buf.clear();
  m_line_offsets.clear();
  m_line_offsets.push_back(0);
}

void ktp::gui::Log::draw(const char* title) {
  if (!ImGui::Begin(title)) {
    ImGui::End();
    return;
  }
  // Options menu
  if (ImGui::BeginPopup("Options")) {
    ImGui::Checkbox("Auto-scroll", &m_auto_scroll);
    ImGui::EndPopup();
  }
  // Main window
  if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
  ImGui::SameLine();
  const bool clear {ImGui::Button("Clear")};
  ImGui::SameLine();
  const bool copy {ImGui::Button("Copy")};
  ImGui::SameLine();
  m_filter.Draw("Filter", -100.0f);

  ImGui::Separator();

  if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {
    if (clear) this->clear();
    if (copy) ImGui::LogToClipboard();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    const char* buf {m_buf.begin()};
    const char* buf_end {m_buf.end()};
    if (m_filter.IsActive()) {
      // In this example we don't use the clipper when Filter is enabled.
      // This is because we don't have random access to the result of our filter.
      // A real application processing logs with ten of thousands of entries may want to store the result of
      // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
      for (auto line_no = 0; line_no < m_line_offsets.Size; ++line_no) {
        const char* line_start {buf + m_line_offsets[line_no]};
        const char* line_end {
          line_no + 1 < m_line_offsets.Size ? buf + m_line_offsets[line_no + 1] - 1 : buf_end
        };
        if (m_filter.PassFilter(line_start, line_end))
          ImGui::TextUnformatted(line_start, line_end);
      }
    } else {
      // The simplest and easy way to display the entire buffer:
      //   ImGui::TextUnformatted(buf_begin, buf_end);
      // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
      // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
      // within the visible area.
      // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
      // on your side is recommended. Using ImGuiListClipper requires
      // - A) random access into your data
      // - B) items all being the  same height,
      // both of which we can handle since we have an array pointing to the beginning of each line of text.
      // When using the filter (in the block of code above) we don't have random access into the data to display
      // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
      // it possible (and would be recommended if you want to search through tens of thousands of entries).
      ImGuiListClipper clipper {};
      clipper.Begin(m_line_offsets.Size);
      while (clipper.Step()) {
        for (auto line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; ++line_no) {
          const char* line_start {buf + m_line_offsets[line_no]};
          const char* line_end {
            line_no + 1 < m_line_offsets.Size ? buf + m_line_offsets[line_no + 1] - 1 : buf_end
          };
          ImGui::TextUnformatted(line_start, line_end);
        }
      }
    }
    ImGui::PopStyleVar();
    // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
    // Using a scrollbar or mouse-wheel will take away from the bottom edge.
    if (m_auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
      ImGui::SetScrollHereY(1.0f);
  }
  ImGui::EndChild();
  ImGui::End();
}
