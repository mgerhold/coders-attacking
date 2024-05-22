#pragma once

#include <memory>
#include <utils/rect.hpp>

class Scene;

class SceneManager {
public:
    SceneManager() = default;
    SceneManager(SceneManager const& other) = default;
    SceneManager(SceneManager&& other) noexcept = default;
    SceneManager& operator=(SceneManager const& other) = default;
    SceneManager& operator=(SceneManager&& other) noexcept = default;
    virtual ~SceneManager() = default;

    virtual void end_scene() = 0;
    virtual void enqueue(std::unique_ptr<Scene> scene) = 0;
};
