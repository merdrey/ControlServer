#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUdpSocket>

#include "enums.h"

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

private:
    quint16 rgbToRgb565(const char r, const char g, const char b);

private:
    QUdpSocket* m_pudp;

    QHostAddress m_addr;
    quint16 m_port;
};

#endif // SERVER_H
