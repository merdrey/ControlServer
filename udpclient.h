#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket>

#include "enums.h"

#define UDP_PORT 9000

class UdpClient : public QObject {
  Q_OBJECT
public:
  explicit UdpClient(QObject *parent = nullptr);
  ~UdpClient();

  void initSocket(const QHostAddress &addr, quint16 port = 0);

  Q_INVOKABLE void sendCommand(const QVariant &data,
                               const Enums::Commands command);
  Q_INVOKABLE void updateClientAddr(const QString &ip);

private slots:
  void onReadyRead();

signals:
  void sendMessage(const QString &message, const Enums::Messages msg);

private:
  static quint16 rgbToRgb565(const char r, const char g, const char b);
  static QByteArray convertImgToRGB565(const QImage &img);

private:
  QUdpSocket *m_udpSocket{nullptr};

  QHostAddress m_clientAddr;
};

#endif // UDPCLIENT_H
