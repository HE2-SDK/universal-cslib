#pragma once
#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace ucsl {
    template <size_t N = 4>
    struct magic_t {
        std::array<char, N> buffer{};

        constexpr magic_t(const auto... characters) : buffer{ characters... } {}
        constexpr magic_t(const std::array<char, N> buffer) : buffer{ buffer } {}
        constexpr magic_t(const char(&str)[N + 1]) { std::copy_n(str, N, std::data(buffer)); }
        constexpr magic_t(const std::string_view& str) { strncpy(std::data(buffer), str.data(), N); } // This will pad with zeroes if str is shorter than N. This is intentional.
        constexpr magic_t(const std::string& str) { strncpy(std::data(buffer), str.c_str(), N); } // This will pad with zeroes if str is shorter than N. This is intentional.

        operator std::string() const { return std::string(std::data(buffer), N); }
        constexpr operator std::string_view() const { return std::string_view(std::data(buffer), N); }

        bool operator==(const magic_t<N>& other) const { return buffer == other.buffer; }
    };
}
