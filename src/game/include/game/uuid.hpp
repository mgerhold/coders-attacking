#pragma once

#include <nlohmann/json.hpp>
#include <stduuid/uuid.h>

[[nodiscard]] uuids::uuid generate_uuid();

namespace nlohmann {
    template<>
    struct adl_serializer<uuids::uuid> {
        static void to_json(json& j, uuids::uuid const& uuid) {
            if (uuid.is_nil()) {
                j = nullptr;
            } else {
                j = to_string(uuid);
            }
        }

        static void from_json(json const& j, uuids::uuid& uuid) {
            if (j.is_null()) {
                uuid = uuids::uuid{};
            } else {
                auto const optional = uuids::uuid::from_string(j.get<std::string>());
                uuid = optional.value(); // throws if string is invalid
            }
        }
    };
} // namespace nlohmann
