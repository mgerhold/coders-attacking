#include <game/uuid.hpp>

[[nodiscard]] uuids::uuid generate_uuid() {
    static auto generator = uuids::uuid_system_generator{};
    return generator();
}
