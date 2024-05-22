#pragma once

#include "scene_stack.hpp"
#include <common/resource_manager.hpp>
#include <common/service_provider.hpp>
#include <gfx/window.hpp>

class Application final : public ServiceProvider {
private:
    gfx::Window m_window;
    ResourceManager m_resource_manager;
    SceneStack m_scene_stack;

public:
    Application();

    // since we pass the this-pointer to class members, we must not copy or move the instance
    Application(Application const& other) = delete;
    Application(Application&& other) noexcept = delete;
    Application& operator=(Application const& other) = delete;
    Application& operator=(Application&& other) noexcept = delete;
    ~Application() override = default;

    [[nodiscard]] gfx::Window const& window() const override;
    [[nodiscard]] gfx::Window& window() override;
    [[nodiscard]] ResourceManager const& resource_manager() const override;
    [[nodiscard]] SceneManager& scene_manager() override;

private:
    void run();

    void handle_events();
    void update();
    void render() const;
};
