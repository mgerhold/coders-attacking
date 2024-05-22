#include <cassert>
#include <functional>
#include <lib2k/string_utils.hpp>
#include <ranges>
#include <tl/optional.hpp>
#include <ui/label.hpp>

namespace ui {

    void Label::render(gfx::Renderer& renderer) const {
        namespace rng = std::ranges::views;
        using namespace utils;

        auto line_widths = std::vector<float>{};
        line_widths.reserve(m_wrapped_text.size());
        auto line_height = tl::optional<float>{};
        auto total_height = 0.0f;
        for (auto const& line : m_wrapped_text) {
            auto const size = m_font->measure_text(line.c_str(), m_font_size);
            line_widths.push_back(size.x);
            if (not line_height.has_value()) {
                line_height = size.y;
                total_height += line_height.value();
            } else {
                total_height += line_height.value() * m_line_height;
            }
        }

        auto const y = std::invoke([&] {
            switch (m_vertical_alignment) {
                case VerticalAlignment::Top:
                    return area().top_left.y;
                case VerticalAlignment::Middle:
                    return area().top_left.y + (area().size.y - static_cast<int>(total_height)) / 2;
                case VerticalAlignment::Bottom:
                    return area().top_left.y + (area().size.y - static_cast<int>(total_height));
            }
            assert(false and "unreachable");
            return 0;
        });
        for (auto const& [i, line_data] : rng::zip(m_wrapped_text, line_widths) | rng::enumerate) {
            auto const& [line, line_width] = line_data;
            auto const x = std::invoke([&] {
                switch (m_alignment) {
                    case Alignment::Left:
                        return area().top_left.x;
                    case Alignment::Center:
                        return area().top_left.x + (area().size.x - static_cast<int>(line_width)) / 2;
                    case Alignment::Right:
                        return area().top_left.x + (area().size.x - static_cast<int>(line_width));
                }
                assert(false and "unreachable");
                return 0;
            });

            renderer.draw_text(
                    *m_font,
                    line.c_str(),
                    utils::Vec2i{
                            x,
                            static_cast<int>(
                                    static_cast<float>(y) + line_height.value() * m_line_height * static_cast<float>(i)
                            ),
                    },
                    m_font_size,
                    m_color
            );
        }
    }

    void Label::recalculate_absolute_area(utils::IntRect const& outer_area) {
        Widget::recalculate_absolute_area(outer_area);
        recalculate_wrapping();
    }

    void Label::recalculate_wrapping() {
        auto const parts = c2k::split(m_caption, " ");
        assert(not parts.empty());

        auto font_size = m_max_font_size;
        auto last_try = std::vector<std::string>{};
        while (font_size > 1) {
            last_try.clear();
            auto it = parts.cbegin();
            auto current_try = std::vector<std::string>{};
            current_try.emplace_back(*it);
            if (not does_fit(current_try, font_size, m_text_size)) {
                --font_size;
                continue;
            }


            ++it;
            if (it == parts.cend()) {
                // there's only one word and the word fits
                m_wrapped_text = std::move(current_try);
                m_font_size = static_cast<float>(font_size);
                return;
            }

            auto is_start_of_line = false;
            while (true) {
                auto appended = current_try;
                if (not is_start_of_line) {
                    appended.back() += ' ';
                }
                appended.back() += *it;
                if (not does_fit(appended, font_size, m_text_size)) {
                    if (is_start_of_line) {
                        // even the first word on this line does not fit
                        // -> reduce font size and try again from the start
                        --font_size;
                        last_try = std::move(current_try);
                        break;
                    }
                    // the current word does not fit, but it's not the first word
                    // -> wrap line instead of appending the word
                    current_try.emplace_back();
                    is_start_of_line = true;
                } else {
                    // current word can be appended and everything still fits
                    current_try = std::move(appended);
                    is_start_of_line = false;
                    ++it;

                    if (it == parts.cend()) { // all words processed, everything fits
                        m_wrapped_text = std::move(current_try);
                        m_font_size = static_cast<float>(font_size);
                        return;
                    }
                }
            }
        }

        // not everything fits, but we cannot go any smaller -> trim the text so that it just fits
        m_font_size = static_cast<float>(std::max(font_size, 1));
        m_wrapped_text = std::move(last_try);
    }

    bool Label::does_fit(std::vector<std::string> lines, int const font_size, utils::Vec2f& out_text_size) {
        namespace rng = std::ranges::views;

        assert(not lines.empty());
        auto const max_area = utils::Vec2f{ area().size };
        auto area = m_font->measure_text(lines.front().c_str(), static_cast<float>(font_size));
        auto const line_height = area.y;

        for (auto const& line : lines | rng::drop(1)) {
            area.y += (1.0f - m_line_height) * line_height;
            auto const size = m_font->measure_text(line.c_str(), static_cast<float>(font_size));
            area.x = std::max(area.x, size.x);
            area.y += size.y;
        }
        out_text_size = area;
        return area.x <= max_area.x and area.y <= max_area.y;
    }
} // namespace ui
