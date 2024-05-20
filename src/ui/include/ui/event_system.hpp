#pragma once

#include "event.hpp"
#include "key.hpp"
#include <deque>
#include <tl/optional.hpp>
#include <unordered_set>

namespace ui {
    class EventSystem final {
    private:
        std::unordered_set<Key> m_pressed_keys;
        std::deque<Event> m_event_queue;
        tl::optional<utils::Vec2i> m_mouse_position;
        std::unordered_set<MouseButton> m_pressed_mouse_buttons;
        utils::Vec2i m_mouse_delta;

    public:
        void update();
        [[nodiscard]] tl::optional<Event> next_event();
        void enqueue_event(Event const& event);
        [[nodiscard]] bool is_key_down(Key key) const;
        [[nodiscard]] utils::Vec2i mouse_position() const;
        [[nodiscard]] bool is_mouse_button_down(MouseButton button) const;
        [[nodiscard]] utils::Vec2i mouse_delta() const;

    private:
        void update_keyboard();
        void update_mouse();
        void update_mouse_position();
        void update_mouse_buttons();
        void update_mouse_wheel();
    };
} // namespace ui
