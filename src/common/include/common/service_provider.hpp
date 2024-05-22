#pragma once

class ResourceManager;

namespace gfx {
    class Window;
}

class SceneManager;

class ServiceProvider {
public:
    ServiceProvider() = default;
    ServiceProvider(ServiceProvider const& other) = default;
    ServiceProvider(ServiceProvider&& other) noexcept = default;
    ServiceProvider& operator=(ServiceProvider const& other) = default;
    ServiceProvider& operator=(ServiceProvider&& other) noexcept = default;
    virtual ~ServiceProvider() = default;

    [[nodiscard]] virtual gfx::Window const& window() const = 0;
    [[nodiscard]] virtual gfx::Window& window() = 0;
    [[nodiscard]] virtual ResourceManager const& resource_manager() const = 0;
    [[nodiscard]] virtual SceneManager& scene_manager() = 0;
};
