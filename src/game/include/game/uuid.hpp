#pragma once

#include <nlohmann/json.hpp>
#include <stduuid/uuid.h>
#include <tl/optional.hpp>

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

[[nodiscard]] uuids::uuid generate_uuid();

class NullableUuidReference final {
private:
    uuids::uuid m_target;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(NullableUuidReference, m_target);

public:
    NullableUuidReference() = default;
    NullableUuidReference(uuids::uuid const target) : m_target{ target } { } // NOLINT (implicit constructor)

    [[nodiscard]] tl::optional<uuids::uuid> target_uuid() const {
        if (m_target.is_nil()) {
            return tl::nullopt;
        }
        return m_target;
    }
};
