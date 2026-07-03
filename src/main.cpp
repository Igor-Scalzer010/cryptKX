#include "app/PasswordController.hpp"

#include <sodium.h>

#include <QCoreApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QtLogging>

#include <cstdlib>

int main(int argc, char *argv[])
{
    if (sodium_init() < 0) {
        qCritical("libsodium nao foi inicializada com sucesso.");
        return EXIT_FAILURE;
    }

    QGuiApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("cryptKX"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.1.0"));
    QCoreApplication::setOrganizationName(QStringLiteral("cryptKX"));
    QQuickStyle::setStyle(QStringLiteral("Basic"));

    cryptkx::app::PasswordController passwordController;

    QQmlApplicationEngine engine;
    engine.addImportPath(QCoreApplication::applicationDirPath() + QStringLiteral("/qml"));
    engine.rootContext()->setContextProperty(QStringLiteral("passwordController"), &passwordController);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(EXIT_FAILURE); },
        Qt::QueuedConnection);

    engine.loadFromModule(QStringLiteral("CryptKX"), QStringLiteral("Main"));

    if (engine.rootObjects().isEmpty()) {
        return EXIT_FAILURE;
    }

    return app.exec();
}
