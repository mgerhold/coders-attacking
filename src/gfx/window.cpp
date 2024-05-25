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

    [[nodiscard]] double Window::delta_seconds() const {
        return static_cast<double>(GetFrameTime());
    }

    [[nodiscard]] double Window::elapsed_seconds() const {
        return GetTime();
    }

    [[nodiscard]] tl::optional<Font> Window::load_font(char const* filename, int const font_size) const {
        // the following code is proudly presented by ChatGPT 4o
        // clang-format off
        auto codepoints = std::array{
            // Printable ASCII characters 32-126
            32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
            64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
            96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
            // Common UTF-8 characters used in Europe and the US
            8364, 8218, 402, 8222, 8230, 8224, 8225, 710, 8240, 352, 8249, 338, 381, 8216, 8217, 8220, 8221, 8226, 8211, 8212, 732, 8482, 353, 8250, 339, 382, 376,
            161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
            192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
            224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255,
            161, 191
        };
        // clang-format on
        auto const font = LoadFontEx(filename, font_size, codepoints.data(), static_cast<int>(codepoints.size()));
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
