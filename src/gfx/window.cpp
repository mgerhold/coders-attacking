#include <raylib.h>
#include <utility>
#include <gfx/window.hpp>

namespace gfx {
    void Window::Deleter::operator()(std::monostate) const {
        CloseWindow();
    }

    Window::Window(int const width, int const height, char const* const title) : m_window_handle{ std::monostate{} } {
        InitWindow(width, height, title);
    }

    [[nodiscard]] bool Window::should_close() const {
        return WindowShouldClose();
    }

    [[nodiscard]] utils::Vec2i Window::size() const {
        return utils::Vec2i{ GetScreenWidth(), GetScreenHeight() };
    }

    void Window::poll_events() {
        m_event_system.update();
    }

    [[nodiscard]] tl::optional<ui::Event> Window::next_event() {
        return m_event_system.next_event();
    }
} // namespace gfx
