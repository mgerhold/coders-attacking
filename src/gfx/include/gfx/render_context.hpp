#pragma once

#include "renderer.hpp"
#include <lib2k/unique_value.hpp>
#include <variant>

class Application;

namespace gfx {
    class Window;

    class RenderContext final {
    private:
        friend class ::Application;

        struct Deleter final {
            void operator()(std::monostate) const;
        };

        c2k::UniqueValue<std::monostate, Deleter> m_handle;
        Renderer m_renderer;

        [[nodiscard]] explicit RenderContext(Window const&);

    public:
        [[nodiscard]] Renderer& renderer() {
            return m_renderer;
        }
    };
} // namespace gfx
