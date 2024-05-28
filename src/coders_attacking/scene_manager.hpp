#pragma once

#include <any>
#include <memory>
#include <tl/optional.hpp>
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
    virtual void push_value(std::any value) = 0;
    [[nodiscard]] virtual tl::optional<std::any> pop_value() = 0;

    template<typename T>
    [[nodiscard]] tl::optional<T> pop_value()
        requires(not std::same_as<T, std::any>)
    {
        if (auto value = pop_value()) {
            if (auto result = std::any_cast<T>(&value.value())) {
                return std::move(*result);
            }
        }
        return tl::nullopt;
    }
};
