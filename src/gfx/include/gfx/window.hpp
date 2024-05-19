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
        Window(int width, int height, char const* title);

        [[nodiscard]] bool should_close() const;
        [[nodiscard]] utils::Vec2i size() const;

        [[nodiscard]] Renderer renderer() {
            return Renderer{};
        }

        void poll_events();

        [[nodiscard]] tl::optional<ui::Event> next_event();
    };
} // namespace gfx
