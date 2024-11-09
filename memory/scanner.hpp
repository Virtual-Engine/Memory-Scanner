#pragma once

#include "../stdafx.hpp"
#include "signature.hpp" 

namespace ve {

    class pattern_not_found_exception : public std::runtime_error {
    public:
        explicit pattern_not_found_exception(const std::string& message)
            : std::runtime_error(message) {}
    };

    struct pattern_entry {
        std::string m_name;
        signature m_pattern;
        std::function<void(wrapper)> m_callback;

        pattern_entry(std::string name, signature pattern, std::function<void(wrapper)> callback = nullptr)
            : m_name(std::move(name)), m_pattern(std::move(pattern)), m_callback(std::move(callback)) {}
    };

    template<std::size_t N>
    struct template_str {
        char value[N];

        constexpr template_str(const char(&str)[N]) {
            std::copy_n(str, N, value);
        }
    };

    class scanner {
    public:
        void add(std::string name, signature pattern, std::function<void(wrapper)> callback);

        void initialize(segment region);
        void cancel();
        std::vector<wrapper> get_results() const;

    private:
        bool m_cancelled = false;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_end_time;
        std::vector<pattern_entry> m_entries;
        std::vector<wrapper> m_results;
    };

    scanner* get_scanner();
}
