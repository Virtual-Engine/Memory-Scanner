#include "scanner.hpp"

namespace ve {

    scanner g_scanner;

    void scanner::add(std::string name, signature pattern, std::function<void(wrapper)> callback) {
        m_entries.emplace_back(std::move(name), std::move(pattern), std::move(callback));
    }

    void scanner::initialize(segment region) {
        auto timestamp = std::chrono::high_resolution_clock::now();
        bool all_found = true;
        for (auto& entry : m_entries) {
            if (auto result = region.scan(entry.m_pattern)) {
                if (entry.m_callback) {
                    std::invoke(std::move(entry.m_callback), result);
                }
                else {
                    all_found = false;
                    std::cout << "Failed to find '" << entry.m_name << "'" << std::endl;
                }
            }
            else {
                all_found = false;
                std::cout << "Failed to find '" << entry.m_name << "'" << std::endl;
            }
        }
        std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timestamp).count() << "ms to scan " << m_entries.size() << " patterns." << std::endl;

        m_entries.clear();
        if (!all_found) {
            throw pattern_not_found_exception("Failed to find some patterns.");
        }
    }

    void scanner::cancel() {
        m_cancelled = true;
    }

    std::vector<wrapper> scanner::get_results() const {
        return m_results;
    }

    scanner* get_scanner() { return &g_scanner; }
}