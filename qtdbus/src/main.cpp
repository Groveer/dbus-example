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
    QCommandLineOption testOption(
        { "t", "test" },
        "Start client with specified test.\n -0: method call\n -1: method call async",
        "0/1");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOptions({ serverOption, clientOption, testOption });
    parser.process(app);

    if (parser.isSet(serverOption)) {
        // must use stacked variable to ensure server life cycle
        auto server = new Server(&app);
    }

    if (parser.isSet(clientOption)) {
        if (!parser.isSet(testOption)) {
            qFatal("Please specify a test to run client.");
        }
        auto client = new Client(&app);
        if (parser.value(testOption) == "0") {
            client->method_call();
        } else {
            client->method_call_async();
        }
    }
    return app.exec();
}
