#include <ui/button.hpp>

static constexpr auto trigger_key = ui::Key::Enter;
static constexpr auto cancel_key = ui::Key::Escape;

namespace ui {
    Button::Button(
            std::string caption,
            u32 const focus_id,
            std::shared_ptr<gfx::Font> font,
            std::function<void(Button&)> on_click
    )
        : m_caption{ std::move(caption),  std::move(font),   40,
                     utils::Color::Black, Alignment::Center, VerticalAlignment::Middle },
          m_on_click{ std::move(on_click) },
          m_focus_id{ focus_id } { }

    [[nodiscard]] HandleEventResult Button::handle_event(Event const event) {
        if (m_keypress_started and not focus_manager().has_focus(*this)) {
            m_keypress_started = false;
        }
        if (auto const mouse_moved = std::get_if<MouseMoved>(&event)) {
            m_hover_is_active = area().contains(mouse_moved->position);
        } else if (auto const mouse_clicked = std::get_if<MouseClicked>(&event)) {
            if (mouse_clicked->button == MouseButton::Left) {
                if (area().contains(mouse_clicked->position)) {
                    m_click_started = true;
                    focus_manager().focus(*this);
                } else if (focus_manager().has_focus(*this)) {
                    focus_manager().unfocus(*this);
                }
            }
        } else if (auto const mouse_released = std::get_if<MouseReleased>(&event)) {
            if (m_click_started and mouse_released->button == MouseButton::Left) {
                m_click_started = false;
                if (area().contains(mouse_released->position) and m_on_click) {
                    m_on_click(*this);
                    return HandleEventResult::EventHandled;
                }
            }
        } else if (auto const key_pressed = std::get_if<KeyPressed>(&event)) {
            if (key_pressed->key == trigger_key and focus_manager().has_focus(*this)) {
                m_keypress_started = true;
            } else if (key_pressed->key == cancel_key and m_keypress_started) {
                m_keypress_started = false;
            }
        } else if (auto const key_released = std::get_if<KeyReleased>(&event)) {
            if (key_released->key == trigger_key and m_keypress_started) {
                m_keypress_started = false;
                if (m_on_click) {
                    m_on_click(*this);
                    return HandleEventResult::EventHandled;
                }
            }
        }
        return HandleEventResult::EventNotHandled;
    }

    void Button::render(gfx::Renderer& renderer) const {
        auto const color = std::invoke([&]() {
            if ((m_click_started and m_hover_is_active) or m_keypress_started) {
                return utils::Color::DarkBlue;
            }
            if (m_click_started or m_hover_is_active) {
                return utils::Color::SkyBlue;
            }
            return utils::Color::LightGray;
        });
        auto const background_area = area().move({ 1, 0 }).shrink({ 1, 1 });
        renderer.draw_filled_rounded_rectangle(background_area, 0.05f, 4, color);
        if (focus_manager().has_focus(*this)) {
            auto const focus_rect = area().move({ 5, 3 }).shrink({ 9, 8 });
            renderer.draw_rounded_rectangle_outline(focus_rect, 0.05f, 4, 1, utils::Color::DarkGray);
        }
        auto const outline_area = area().move({ 2, 1 }).shrink({ 3, 3 });
        renderer.draw_rounded_rectangle_outline(outline_area, 0.05f, 4, 1, utils::Color::Black);
        m_caption.render(renderer);
    }

    void Button::recalculate_absolute_area(utils::IntRect const& outer_area) {
        Widget::recalculate_absolute_area(outer_area);
        auto const label_area = area().scaled_from_center(0.9f);
        m_caption.recalculate_absolute_area(label_area);
    }
} // namespace ui
