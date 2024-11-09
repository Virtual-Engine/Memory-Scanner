#include "../stdafx.hpp"

namespace ve {

    segment::segment(wrapper base, std::size_t size) :
        m_base(base), m_size(size) {
        assert(size > 0 && "Memory segment size must be greater than zero");
    }

    wrapper segment::begin() const {
        return m_base;
    }

    wrapper segment::end() const {
        return m_base.add(m_size);
    }

    std::size_t segment::size() const {
        return m_size;
    }

    bool segment::contains(wrapper h) const {
        std::uintptr_t h_ptr = h.as<std::uintptr_t>();
        return h_ptr >= begin().as<std::uintptr_t>() && h_ptr < end().as<std::uintptr_t>();
    }

    bool segment::contains_segment(const segment& other) const {
        return other.begin().as<std::uintptr_t>() >= begin().as<std::uintptr_t>() &&
            other.end().as<std::uintptr_t>() <= end().as<std::uintptr_t>();
    }

    bool segment::pattern_matches(std::uint8_t* target, const std::optional<std::uint8_t>* sig, std::size_t length) {
        for (std::size_t i = 0; i < length; ++i) {
            if (sig[i] && *sig[i] != target[i]) {
                return false;
            }
        }
        return true;
    }

    bool segment::pattern_matches_masked(std::uint8_t* target, const std::optional<std::uint8_t>* sig, std::size_t length) {
        for (std::size_t i = 0; i < length; ++i) {
            if (sig[i] && *sig[i] != target[i]) {
                return false;
            }
            else if (!sig[i]) {
                continue;
            }
        }
        return true;
    }

    bool segment::is_valid_signature(const signature& sig) {
        return !sig.m_bytes.empty();
    }

    wrapper segment::scan(const signature& sig) {
        if (!is_valid_signature(sig)) {
            throw std::invalid_argument("Signature cannot be empty");
        }

        auto data = sig.m_bytes.data();
        auto length = sig.m_bytes.size();
        for (std::uintptr_t i = 0; i <= m_size - length; ++i) {
            if (pattern_matches(m_base.add(i).as<std::uint8_t*>(), data, length)) {
                return m_base.add(i);
            }
        }
        return nullptr;
    }

    std::vector<wrapper> segment::scan_all(const signature& sig, std::size_t max_results) {
        std::vector<wrapper> result;
        if (!is_valid_signature(sig)) {
            throw std::invalid_argument("Signature cannot be empty");
        }

        auto data = sig.m_bytes.data();
        auto length = sig.m_bytes.size();
        for (std::uintptr_t i = 0; i <= m_size - length; ++i) {
            if (pattern_matches(m_base.add(i).as<std::uint8_t*>(), data, length)) {
                result.push_back(m_base.add(i));
                if (max_results > 0 && result.size() >= max_results) {
                    break;
                }
            }
        }
        return result;
    }

    wrapper segment::find(const signature& sig, std::size_t offset) {
        if (!is_valid_signature(sig)) {
            throw std::invalid_argument("Signature cannot be empty");
        }

        auto data = sig.m_bytes.data();
        auto length = sig.m_bytes.size();
        if (offset >= m_size) {
            return nullptr;
        }

        for (std::uintptr_t i = offset; i <= m_size - length; ++i) {
            if (pattern_matches(m_base.add(i).as<std::uint8_t*>(), data, length)) {
                return m_base.add(i);
            }
        }
        return nullptr;
    }

    std::size_t segment::offset_of(const signature& sig) {
        if (!is_valid_signature(sig)) {
            throw std::invalid_argument("Signature cannot be empty");
        }

        auto data = sig.m_bytes.data();
        auto length = sig.m_bytes.size();
        for (std::uintptr_t i = 0; i <= m_size - length; ++i) {
            if (pattern_matches(m_base.add(i).as<std::uint8_t*>(), data, length)) {
                return i;
            }
        }
        return static_cast<std::size_t>(-1);
    }

    bool segment::replace_pattern(const signature& old_sig, const signature& new_sig) {
        if (!is_valid_signature(old_sig) || !is_valid_signature(new_sig)) {
            throw std::invalid_argument("Both old and new signatures must be valid");
        }

        auto old_data = old_sig.m_bytes.data();
        auto old_length = old_sig.m_bytes.size();
        if (old_length != new_sig.m_bytes.size()) {
            return false;
        }

        for (std::uintptr_t i = 0; i <= m_size - old_length; ++i) {
            if (pattern_matches(m_base.add(i).as<std::uint8_t*>(), old_data, old_length)) {
                auto new_data = new_sig.m_bytes.data();
                std::memcpy(m_base.add(i).as<std::uint8_t*>(), new_data, old_length);
                return true;
            }
        }
        return false;
    }

}