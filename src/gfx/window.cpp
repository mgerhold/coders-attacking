#include <gfx/window.hpp>
#include <raylib.h>
#include <utility>

static void set_window_flag(ConfigFlags const flag, bool const enabled) {
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

    Window::Window(utils::Vec2i const resolution, char const* const title) : m_window_handle{ std::monostate{} } {
        InitWindow(resolution.x, resolution.y, title);
        SetExitKey(0);
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

    [[nodiscard]] float Window::delta_seconds() const {
        return GetFrameTime();
    }

    [[nodiscard]] tl::optional<Font> Window::load_font(char const* filename, int font_size) const {
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
