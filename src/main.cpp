#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "database/context.hh"
#include "core.hpp"
#include "database.hh"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    
    db::context ctx{"build/src/database/database.db"};

    ctx.raw_query("INSERT INTO user (username, password) VALUES (\"Kmicic\", \"maslo\");",
    []([[maybe_unused]] void* user_data, int argc, char** argv, char** col_name) -> int {
            printf("%s",argv[0]);
            return 0;
        });

    auto user = ctx.select_one<User>();

    std::cout << user.id << '\n';
    std::cout << user.password << '\n';
    std::cout << user.username << '\n';


    return app.exec();
}
