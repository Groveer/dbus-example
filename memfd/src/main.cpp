#include "client.h"
#include "server.h"

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("qtdbus");

    QCommandLineOption serverOption({ "s", "server" }, "Start the server.");
    QCommandLineOption clientOption({ "c", "client" }, "Start the client.");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOptions({ serverOption, clientOption });
    parser.process(app);

    if (parser.isSet(serverOption)) {
        // must use stacked variable to ensure server life cycle
        auto server = new Server(&app);
    }

    if (parser.isSet(clientOption)) {
        auto client = new Client(&app);
        client->method_call_async();
    }
    return app.exec();
}
