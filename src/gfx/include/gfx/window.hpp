#pragma once

#include "renderer.hpp"
#include <lib2k/unique_value.hpp>
#include <ui/event_system.hpp>
#include <utils/vec2.hpp>
#include <variant>

namespace gfx {
    class Window final {
    private:
        struct Deleter final {
            void operator()(std::monostate) const;
        };

        c2k::UniqueValue<std::monostate, Deleter> m_window_handle;
        ui::EventSystem m_event_system;

    public:
        Window(utils::Vec2i resolution, char const* title);

        [[nodiscard]] bool should_close() const;
        [[nodiscard]] utils::Vec2i size() const;
        [[nodiscard]] utils::IntRect area() const;
        void set_resizable(bool enabled);

        [[nodiscard]] double delta_seconds() const;
        [[nodiscard]] double elapsed_seconds() const;

        [[nodiscard]] tl::optional<Font> load_font(char const* filename, int font_size) const;
        [[nodiscard]] bool was_resized() const;
        void poll_events();

        [[nodiscard]] tl::optional<ui::Event> next_event();

        [[nodiscard]] ui::EventSystem const& event_system() const {
            return m_event_system;
        }
    };
} // namespace gfx
