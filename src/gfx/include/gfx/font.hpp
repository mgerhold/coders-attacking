#pragma once

#include <memory>
#include <utils/vec2.hpp>

struct Font; // forward declare the raylib type

namespace gfx {
    class Renderer;

    class Font final {
        friend class Renderer;
        friend class Window;

    private:
        struct Deleter {
            void operator()(::Font const* handle) const;
        };

        std::unique_ptr<::Font, Deleter> m_handle; // pointer indirection to not expose the type to the outside

        explicit Font(std::unique_ptr<::Font, Deleter> handle) : m_handle{ std::move(handle) } { }

    public:
        [[nodiscard]] utils::Vec2f measure_text(char const* text, float font_size) const;
    };
} // namespace gfx
