#include "core/PasswordGenerator.hpp"

#include <sodium.h>

#include <cstdint>
#include <limits>
#include <mutex>
#include <stdexcept>
#include <string>
#include <string_view>

namespace cryptkx::core {
namespace {

constexpr std::string_view kUppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
constexpr std::string_view kLowercase = "abcdefghijklmnopqrstuvwxyz";
constexpr std::string_view kDigits = "0123456789";
constexpr std::string_view kSymbols = "!@#$%^&*()-_=+[]{};:,.<>?";

void ensureSodiumInitialized()
{
    static std::once_flag once;
    std::call_once(once, [] {
        if (sodium_init() < 0) {
            throw std::runtime_error("Failed to initialize libsodium.");
        }
    });
}

void validateOptions(const PasswordOptions& options)
{
    if (options.length < PasswordOptions::minLength || options.length > PasswordOptions::maxLength) {
        throw std::invalid_argument("Password length must be between 8 and 64 characters.");
    }

    if (!options.useUppercase && !options.useLowercase && !options.useDigits && !options.useSymbols) {
        throw std::invalid_argument("At least one character set must be selected.");
    }
}

std::uint32_t randomUpperBound(std::size_t size)
{
    if (size == 0 || size > static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max())) {
        throw std::invalid_argument("Character set size is outside the supported range.");
    }

    return static_cast<std::uint32_t>(size);
}

char pickRandom(std::string_view characters)
{
    const auto index = randombytes_uniform(randomUpperBound(characters.size()));
    return characters[static_cast<std::size_t>(index)];
}

std::string buildCharset(const PasswordOptions& options)
{
    std::string charset;

    if (options.useUppercase) {
        charset.append(kUppercase);
    }
    if (options.useLowercase) {
        charset.append(kLowercase);
    }
    if (options.useDigits) {
        charset.append(kDigits);
    }
    if (options.useSymbols) {
        charset.append(kSymbols);
    }

    return charset;
}

} // namespace

std::string PasswordGenerator::generate(const PasswordOptions& options)
{
    validateOptions(options);
    ensureSodiumInitialized();

    const auto charset = buildCharset(options);

    std::string password;
    password.reserve(options.length);

    while (password.size() < options.length) {
        password.push_back(pickRandom(charset));
    }

    return password;
}

} // namespace cryptkx::core
