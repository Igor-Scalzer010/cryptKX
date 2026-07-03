#pragma once

#include <cstddef>

namespace cryptkx::core {

struct PasswordOptions {
    static constexpr std::size_t minLength = 8;
    static constexpr std::size_t maxLength = 64;

    std::size_t length{20};
    bool useUppercase{true};
    bool useLowercase{true};
    bool useDigits{true};
    bool useSymbols{true};
};

} // namespace cryptkx::core
