#pragma once

#include <QObject>
#include <QString>

namespace cryptkx::app {

class PasswordController final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString password READ password NOTIFY passwordChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(bool hasError READ hasError NOTIFY errorMessageChanged)

public:
    explicit PasswordController(QObject *parent = nullptr);

    QString password() const;
    QString errorMessage() const;
    bool hasError() const;

    Q_INVOKABLE QString generatePassword(int length,
                                         bool uppercase,
                                         bool lowercase,
                                         bool digits,
                                         bool symbols);
    Q_INVOKABLE bool copyToClipboard(const QString &text);
    Q_INVOKABLE void clearError();

signals:
    void passwordChanged();
    void errorMessageChanged();

private:
    void setPassword(const QString &password);
    void setErrorMessage(const QString &message);

    QString m_password;
    QString m_errorMessage;
};

} // namespace cryptkx::app
