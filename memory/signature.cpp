#include "../stdafx.hpp"

namespace ve {

    signature::signature(std::string_view ida_sig) {
        auto to_hex = [](char c) -> std::optional<uint8_t> {
            if (std::isdigit(c)) return static_cast<uint8_t>(c - '0');
            c = std::toupper(c);
            if (c >= 'A' && c <= 'F') return static_cast<uint8_t>(c - 'A' + 10);
            return std::nullopt;
            };

        for (size_t i = 0; i < ida_sig.size(); ++i) {
            if (ida_sig[i] == ' ') continue;

            if (ida_sig[i] == '?') {
                m_bytes.push_back(std::nullopt);
                if (i + 1 < ida_sig.size() && ida_sig[i + 1] == '?') ++i;
            }
            else {
                if (i + 1 < ida_sig.size()) {
                    auto high = to_hex(ida_sig[i]);
                    auto low = to_hex(ida_sig[i + 1]);
                    if (high && low) {
                        m_bytes.emplace_back((*high << 4) | *low);
                        ++i;
                    }
                }
            }
        }
    }

    signature::signature(const void* bytes, std::string_view mask) {
        auto byte_ptr = static_cast<const uint8_t*>(bytes);
        for (size_t i = 0; i < mask.size(); ++i) {
            if (mask[i] == '?') {
                m_bytes.push_back(std::nullopt);
            }
            else {
                m_bytes.emplace_back(byte_ptr[i]);
            }
        }
    }

    bool signature::match_at(const void* data) const {
        auto data_ptr = static_cast<const uint8_t*>(data);
        for (size_t i = 0; i < m_bytes.size(); ++i) {
            if (m_bytes[i] && *m_bytes[i] != data_ptr[i]) {
                return false;
            }
        }
        return true;
    }

    std::size_t signature::size() const {
        return m_bytes.size();
    }
}