#include "core/PasswordGenerator.hpp"

#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kUppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
constexpr std::string_view kLowercase = "abcdefghijklmnopqrstuvwxyz";
constexpr std::string_view kDigits = "0123456789";
constexpr std::string_view kSymbols = "!@#$%^&*()-_=+[]{};:,.<>?";

void require(bool condition, std::string_view message)
{
    if (!condition) {
        throw std::runtime_error(std::string(message));
    }
}

bool containsOnly(std::string_view value, std::string_view allowed)
{
    return value.find_first_not_of(allowed) == std::string_view::npos;
}

void expectInvalidArgument(const std::function<void()>& action, std::string_view expectedMessagePart)
{
    try {
        action();
    } catch (const std::invalid_argument& error) {
        require(std::string_view(error.what()).find(expectedMessagePart) != std::string_view::npos,
                "invalid_argument message did not include the expected text");
        return;
    }

    throw std::runtime_error("expected std::invalid_argument was not thrown");
}

void testDefaultPasswordIsNonEmptyAndHasDefaultLength()
{
    const auto password = cryptkx::core::PasswordGenerator::generate();

    require(!password.empty(), "default password should not be empty");
    require(password.size() == 20, "default password should have length 20");
}

void testCustomLength()
{
    cryptkx::core::PasswordOptions options;
    options.length = 32;

    const auto password = cryptkx::core::PasswordGenerator::generate(options);

    require(password.size() == options.length, "password should have the requested length");
}

void testInvalidLength()
{
    cryptkx::core::PasswordOptions tooShort;
    tooShort.length = cryptkx::core::PasswordOptions::minLength - 1;

    expectInvalidArgument([&] { cryptkx::core::PasswordGenerator::generate(tooShort); },
                          "between 8 and 64");

    cryptkx::core::PasswordOptions tooLong;
    tooLong.length = cryptkx::core::PasswordOptions::maxLength + 1;

    expectInvalidArgument([&] { cryptkx::core::PasswordGenerator::generate(tooLong); },
                          "between 8 and 64");
}

void testNoCharacterSet()
{
    cryptkx::core::PasswordOptions options;
    options.useUppercase = false;
    options.useLowercase = false;
    options.useDigits = false;
    options.useSymbols = false;

    expectInvalidArgument([&] { cryptkx::core::PasswordGenerator::generate(options); },
                          "At least one character set");
}

void testDigitsOnly()
{
    cryptkx::core::PasswordOptions options;
    options.length = 16;
    options.useUppercase = false;
    options.useLowercase = false;
    options.useDigits = true;
    options.useSymbols = false;

    const auto password = cryptkx::core::PasswordGenerator::generate(options);

    require(password.size() == options.length, "digits-only password should have requested length");
    require(containsOnly(password, kDigits), "digits-only password should contain only digits");
}

void testSymbolsOnly()
{
    cryptkx::core::PasswordOptions options;
    options.length = 16;
    options.useUppercase = false;
    options.useLowercase = false;
    options.useDigits = false;
    options.useSymbols = true;

    const auto password = cryptkx::core::PasswordGenerator::generate(options);

    require(password.size() == options.length, "symbols-only password should have requested length");
    require(containsOnly(password, kSymbols), "symbols-only password should contain only symbols");
}

void testAllGroups()
{
    cryptkx::core::PasswordOptions options;
    options.length = 32;
    options.useUppercase = true;
    options.useLowercase = true;
    options.useDigits = true;
    options.useSymbols = true;

    const auto password = cryptkx::core::PasswordGenerator::generate(options);
    const std::string allCharacters = std::string(kUppercase)
        + std::string(kLowercase)
        + std::string(kDigits)
        + std::string(kSymbols);

    require(password.size() == options.length, "all-groups password should have requested length");
    require(containsOnly(password, allCharacters), "all-groups password should use only configured groups");
}

} // namespace

int main()
{
    try {
        testDefaultPasswordIsNonEmptyAndHasDefaultLength();
        testCustomLength();
        testInvalidLength();
        testNoCharacterSet();
        testDigitsOnly();
        testSymbolsOnly();
        testAllGroups();
    } catch (const std::exception& error) {
        std::cerr << "cryptkx_core_tests failed: " << error.what() << '\n';
        return 1;
    }

    std::cout << "cryptkx_core_tests passed\n";
    return 0;
}
