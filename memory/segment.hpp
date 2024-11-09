#pragma once

#include "../stdafx.hpp"
#include "signature.hpp"

namespace ve {

    class segment {
    public:
        segment(wrapper base, std::size_t size);

        wrapper begin() const;
        wrapper end() const;
        std::size_t size() const;
        bool contains(wrapper h) const;
        bool contains_segment(const segment& other) const;

        wrapper scan(const signature& sig);
        std::vector<wrapper> scan_all(const signature& sig, std::size_t max_results = 0);
        wrapper find(const signature& sig, std::size_t offset = 0);

        std::size_t offset_of(const signature& sig);
        bool replace_pattern(const signature& old_sig, const signature& new_sig);

    private:
        wrapper m_base;
        std::size_t m_size;

        static bool pattern_matches(std::uint8_t* target, const std::optional<std::uint8_t>* sig, std::size_t length);
        static bool is_valid_signature(const signature& sig);
        static bool pattern_matches_masked(std::uint8_t* target, const std::optional<std::uint8_t>* sig, std::size_t length);
    };

}