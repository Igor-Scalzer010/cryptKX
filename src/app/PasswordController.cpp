#include "app/PasswordController.hpp"

#include "core/PasswordGenerator.hpp"

#include <QClipboard>
#include <QGuiApplication>

#include <cstddef>
#include <stdexcept>
#include <string>

namespace {

constexpr int kMinimumPasswordLength = 8;
constexpr int kMaximumPasswordLength = 64;

QString invalidLengthMessage()
{
    return QStringLiteral("Choose a length between 8 and 64 characters.");
}

QString missingGroupMessage()
{
    return QStringLiteral("Select at least one character set.");
}

QString generationFailureMessage()
{
    return QStringLiteral("Could not generate the password right now. Try again.");
}

} // namespace

namespace cryptkx::app {

PasswordController::PasswordController(QObject *parent)
    : QObject(parent)
{
}

QString PasswordController::password() const
{
    return m_password;
}

QString PasswordController::errorMessage() const
{
    return m_errorMessage;
}

bool PasswordController::hasError() const
{
    return !m_errorMessage.isEmpty();
}

QString PasswordController::generatePassword(int length,
                                            bool uppercase,
                                            bool lowercase,
                                            bool digits,
                                            bool symbols)
{
    if (length < kMinimumPasswordLength || length > kMaximumPasswordLength) {
        setPassword({});
        setErrorMessage(invalidLengthMessage());
        return {};
    }

    if (!uppercase && !lowercase && !digits && !symbols) {
        setPassword({});
        setErrorMessage(missingGroupMessage());
        return {};
    }

    try {
        cryptkx::core::PasswordOptions options;
        options.length = static_cast<std::size_t>(length);
        options.useUppercase = uppercase;
        options.useLowercase = lowercase;
        options.useDigits = digits;
        options.useSymbols = symbols;

        const std::string generated = cryptkx::core::PasswordGenerator::generate(options);

        const QString generatedPassword = QString::fromStdString(generated);
        setPassword(generatedPassword);
        clearError();
        return generatedPassword;
    } catch (const std::invalid_argument &) {
        setPassword({});
        setErrorMessage(QStringLiteral("Review the password generation options."));
    } catch (const std::exception &) {
        setPassword({});
        setErrorMessage(generationFailureMessage());
    } catch (...) {
        setPassword({});
        setErrorMessage(generationFailureMessage());
    }

    return {};
}

bool PasswordController::copyToClipboard(const QString &text)
{
    if (text.isEmpty()) {
        setErrorMessage(QStringLiteral("Generate a password before copying."));
        return false;
    }

    QClipboard *clipboard = QGuiApplication::clipboard();
    if (clipboard == nullptr) {
        setErrorMessage(QStringLiteral("The clipboard is not available."));
        return false;
    }

    clipboard->setText(text, QClipboard::Clipboard);
    if (clipboard->supportsSelection()) {
        clipboard->setText(text, QClipboard::Selection);
    }

    clearError();
    return true;
}

void PasswordController::clearError()
{
    setErrorMessage({});
}

void PasswordController::setPassword(const QString &password)
{
    if (m_password == password) {
        return;
    }

    m_password = password;
    emit passwordChanged();
}

void PasswordController::setErrorMessage(const QString &message)
{
    if (m_errorMessage == message) {
        return;
    }

    m_errorMessage = message;
    emit errorMessageChanged();
}

} // namespace cryptkx::app
