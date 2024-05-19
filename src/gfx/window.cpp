#include <gfx/window.hpp>
#include <raylib.h>
#include <utility>

static void set_window_flag(int const flag, bool const enabled) {
    if (enabled) {
        SetWindowState(flag);
    } else {
        ClearWindowState(flag);
    }
}

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

    [[nodiscard]] utils::IntRect Window::area() const {
        return utils::IntRect{ utils::Vec2i{}, size() };
    }

    void Window::set_resizable(bool const enabled) {
        set_window_flag(FLAG_WINDOW_RESIZABLE, enabled);
    }

    [[nodiscard]] tl::optional<Font> Window::load_font(char const* filename, int font_size) {
        auto const font = LoadFontEx(filename, font_size, nullptr, 0);
        if (not IsFontReady(font) or font.texture.id == GetFontDefault().texture.id) {
            return tl::nullopt;
        }
        return Font{ std::unique_ptr<::Font, Font::Deleter>{ new ::Font{ font } } };
    }

    [[nodiscard]] bool Window::was_resized() const {
        return IsWindowResized();
    }

    void Window::poll_events() {
        m_event_system.update();
    }

    [[nodiscard]] tl::optional<ui::Event> Window::next_event() {
        return m_event_system.next_event();
    }
} // namespace gfx
