#include <array>
#include <cassert>
#include <raylib.h>
#include <ui/event_system.hpp>

// clang-format off
static constexpr auto mouse_buttons = std::array{
    ui::MouseButton::Left,
    ui::MouseButton::Right,
    ui::MouseButton::Middle,
    ui::MouseButton::Side,
    ui::MouseButton::Extra,
    ui::MouseButton::Forward,
    ui::MouseButton::Back,
};
// clang-format on

namespace ui {

    void EventSystem::update() {
        assert(m_event_queue.empty() and "all previous events should have been consumed");
        update_keyboard();
        update_mouse();
    }

    [[nodiscard]] tl::optional<Event> EventSystem::next_event() {
        if (m_event_queue.empty()) {
            return tl::nullopt;
        }
        auto const result = m_event_queue.front();
        m_event_queue.pop_front();
        return result;
    }

    void EventSystem::update_keyboard() {
        for (auto it = m_pressed_keys.begin(); it != m_pressed_keys.end();) {
            if (IsKeyReleased(static_cast<int>(*it))) {
                m_event_queue.emplace_back(KeyReleased{ *it });
                it = m_pressed_keys.erase(it);
            } else {
                ++it;
            }
        }

        while (true) {
            auto const key = GetKeyPressed();
            if (key == 0) {
                break;
            }
            m_pressed_keys.insert(Key{ key });
            m_event_queue.emplace_back(KeyPressed{ Key{ key } });
        }
    }

    void EventSystem::update_mouse() {
        update_mouse_position();
        update_mouse_buttons();
        update_mouse_wheel();
    }

    void EventSystem::update_mouse_position() {
        auto const mouse_position = utils::Vec2i{ GetMouseX(), GetMouseY() };
        if (m_mouse_position.has_value()) {
            auto const delta = mouse_position - m_mouse_position.value();
            if (delta != utils::Vec2i{}) {
                m_event_queue.emplace_back(MouseMoved{ delta, mouse_position });
            }
        }
        m_mouse_position = mouse_position;
    }

    void EventSystem::update_mouse_buttons() {
        assert(m_mouse_position.has_value());
        for (auto const button : mouse_buttons) {
            if (IsMouseButtonPressed(static_cast<int>(button))) {
                m_event_queue.emplace_back(MouseClicked{ m_mouse_position.value(), button });
            }
            if (IsMouseButtonReleased(static_cast<int>(button))) {
                m_event_queue.emplace_back(MouseReleased{ m_mouse_position.value(), button });
            }
        }
    }

    void EventSystem::update_mouse_wheel() {
        auto const wheel_movement = GetMouseWheelMoveV();
        if (wheel_movement.x != 0 or wheel_movement.y != 0) {
            m_event_queue.emplace_back(MouseWheelMoved{
                    utils::Vec2f{ wheel_movement.x, wheel_movement.y }
            });
        }
    }

} // namespace ui
