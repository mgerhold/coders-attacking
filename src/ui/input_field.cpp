#include "ui/focus_manager.hpp"


#include <lib2k/utf8/char.hpp>
#include <sstream>
#include <ui/input_field.hpp>

namespace ui {

    InputField::InputField(std::string placeholder, u32 const focus_id, gfx::Font const& font)
        : InputField{ {}, std::move(placeholder), focus_id, font } { }

    InputField::InputField(WidgetName&& widget_name, std::string placeholder, u32 const focus_id, gfx::Font const& font)
        : Widget{ std::move(widget_name) },
          m_placeholder_label{ std::move(placeholder),   font, 40, placeholder_color, Alignment::Left,
                               VerticalAlignment::Middle },
          m_text_label{ "", font, 40, regular_color, Alignment::Left, VerticalAlignment::Middle },
          m_focus_id{ focus_id } { }

    void InputField::on_enter_pressed(std::function<void(InputField&)> function) {
        m_on_enter_pressed = std::move(function);
    }

    [[nodiscard]] HandleEventResult InputField::handle_event(Event const event) {
        if (auto const mouse_clicked = std::get_if<MouseClicked>(&event);
            mouse_clicked != nullptr and mouse_clicked->button == MouseButton::Left) {
            if (area().contains(mouse_clicked->position)) {
                focus_manager().focus(*this);
            } else if (focus_manager().has_focus(*this)) {
                focus_manager().unfocus(*this);
            }
        }

        if (not focus_manager().has_focus(*this)) {
            return HandleEventResult::EventNotHandled;
        }

        if (auto const char_typed = std::get_if<CharTyped>(&event)) {
            if (auto const c = c2k::Utf8Char::from_codepoint(char_typed->codepoint)) {
                auto stream = std::ostringstream{};
                stream << c.value();
                m_text += std::move(stream).str();
                m_text_label.caption(std::string{ m_text.c_str() } + "_");
            } else {
                spdlog::error("invalid UTF8 char: {}", char_typed->codepoint);
            }

            return HandleEventResult::EventHandled;
        }
        auto const backspace_pressed = std::invoke([&] {
            if (auto const key_pressed = std::get_if<KeyPressed>(&event);
                key_pressed != nullptr and key_pressed->key == Key::Backspace) {
                return true;
            }
            if (auto const key_repeated = std::get_if<KeyRepeated>(&event);
                key_repeated != nullptr and key_repeated->key == Key::Backspace) {
                return true;
            }
            return false;
        });
        if (backspace_pressed) {
            if (not m_text.is_empty()) {
                m_text = m_text.substring(m_text.begin(), m_text.end() - 1);
                m_text_label.caption(std::string{ m_text.c_str() } + "_");
            }
        }

        if (auto const key_pressed = std::get_if<KeyPressed>(&event);
            m_on_enter_pressed and key_pressed != nullptr
            and (key_pressed->key == Key::Enter or key_pressed->key == Key::KpEnter)) {
            m_on_enter_pressed(*this);
        }

        return HandleEventResult::EventNotHandled;
    }

    void InputField::render(gfx::Renderer& renderer) const {
        renderer.draw_filled_rectangle(area(), background_color);
        renderer.draw_rectangle_outline(area(), utils::Color::Black);
        if (text().is_empty()) {
            m_placeholder_label.render(renderer);
        }
        m_text_label.render(renderer);
    }

    [[nodiscard]] c2k::Utf8String const& InputField::text() const {
        return m_text;
    }

    void InputField::recalculate_absolute_area(utils::IntRect const& outer_area) {
        Widget::recalculate_absolute_area(outer_area);
        auto const label_area = outer_area.inset(10);
        m_placeholder_label.recalculate_absolute_area(label_area);
        m_text_label.recalculate_absolute_area(label_area);
    }

    [[nodiscard]] tl::optional<u32> InputField::focus_id() const {
        return m_focus_id;
    }

    void InputField::on_focused() {
        Widget::on_focused();
        m_text_label.caption(m_text_label.caption() + '_');
    }

    void InputField::on_unfocused() {
        Widget::on_unfocused();
        auto caption = m_text_label.caption();
        caption.erase(caption.length() - 1);
        m_text_label.caption(std::move(caption));
    }

} // namespace ui
