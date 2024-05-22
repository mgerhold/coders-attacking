#include <gfx/render_context.hpp>
#include <raylib.h>

namespace gfx {

    void RenderContext::Deleter::operator()(std::monostate) const {
        EndDrawing();
    }

    RenderContext::RenderContext(Window const&) : m_handle{ std::monostate{} } {
        BeginDrawing();
    }

} // namespace gfx
