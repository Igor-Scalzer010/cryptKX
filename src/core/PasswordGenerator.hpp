#pragma once

#include "core/PasswordOptions.hpp"

#include <string>

namespace cryptkx::core {

class PasswordGenerator {
public:
    static std::string generate(const PasswordOptions& options = {});
};

} // namespace cryptkx::core
