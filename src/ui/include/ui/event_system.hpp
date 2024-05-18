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

    public:
        void update();
        [[nodiscard]] tl::optional<Event> next_event();

    private:
        void update_keyboard();
        void update_mouse();
        void update_mouse_position();
        void update_mouse_buttons();
        void update_mouse_wheel();
    };
} // namespace ui
