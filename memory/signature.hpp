#pragma once
#include "../stdafx.hpp"

namespace ve {

    class signature {
    public:
        signature(std::string_view ida_sig);
        explicit signature(const void* bytes, std::string_view mask);
        bool match_at(const void* data) const;
        std::size_t size() const;

        inline signature(const char* ida_sig) :
            signature(std::string_view(ida_sig)) {
        }

    public:
        std::vector<std::optional<std::uint8_t>> m_bytes;
    };

}
