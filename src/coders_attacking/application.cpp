#include "application.hpp"
#include "scenes/test_scene.hpp"
#include <gfx/render_context.hpp>

// clang-format off
Application::Application()
    : m_window{
          { 1280, 720 },
          "coders attacking"
      },
      m_resource_manager{ *this },
      m_scene_stack{ *this }
{ // clang-format on
    m_window.set_resizable(true);
    m_scene_stack.emplace<TestScene>();
    run();
}

gfx::Window const& Application::window() const {
    return m_window;
}

gfx::Window& Application::window() {
    return m_window;
}

ResourceManager const& Application::resource_manager() const {
    return m_resource_manager;
}

SceneManager& Application::scene_manager() {
    return m_scene_stack.scene_manager();
}

void Application::run() {
    while (not m_scene_stack.is_empty() and not m_window.should_close()) {
        handle_events();
        update();
        render();
    }
}
void Application::handle_events() {
    m_window.poll_events();
    while (auto const event = m_window.next_event()) {
        m_scene_stack.handle_event(event.value(), m_window.event_system());
    }
}

void Application::update() {
    m_scene_stack.update(m_window.event_system(), m_window.delta_seconds());
}

void Application::render() const {
    auto render_context = gfx::RenderContext{ m_window };
    auto& renderer = render_context.renderer();
    m_scene_stack.render(renderer);
    renderer.draw_fps({ 10, 10 });
}
