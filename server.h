#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUdpSocket>

#include "enums.h"

#define UDP_PORT 9000
#define IP_ADDR QHostAddress("10.0.0.10")

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

    void initSocket(const QHostAddress &addr, quint16 port = 0);

    Q_INVOKABLE void sendCommand(const QVariant &data, const Enums::Commands command);

private slots:
    void onReadyRead();

signals:
    void sendMessage(const QString &message, const Enums::Messages msg);

private:
    quint16 rgbToRgb565(const char r, const char g, const char b);

private:
    QUdpSocket* m_pudp;
};

#endif // SERVER_H
