#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

#include "enums.h"
#include "server.h"
#include "udpclient.h"

int main(int argc, char *argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
  QGuiApplication app(argc, argv);

  qmlRegisterUncreatableType<Enums>("App.Enums", 1, 0, "Enums", "Enums only");

  QQmlApplicationEngine engine;
  const QUrl url(QStringLiteral("qrc:/main.qml"));

  Server controlServer;
  controlServer.initSocket(QHostAddress::AnyIPv4, 53);

  UdpClient udpClient;
  udpClient.initSocket(QHostAddress::AnyIPv4, UDP_PORT);

  FragmentCollector *fragCollector = new FragmentCollector();
  controlServer.setFragCollector(fragCollector);

  engine.rootContext()->setContextProperty("udpClient", &udpClient);
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
