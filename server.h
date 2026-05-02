#ifndef SERVER_H
#define SERVER_H

#include <QHash>
#include <QObject>
#include <QUdpSocket>

#include "clientsession.h"
#include "fragmentcollector.h"

#define SERV_PORT 51820
#define SERV_ADDR QHostAddress("192.168.0.6")

class Server : public QObject {
  Q_OBJECT
public:
  explicit Server(QObject *parent = nullptr);
  ~Server();

  void initSocket(const QHostAddress &addr, quint16 port = 0);
  void setFragCollector(FragmentCollector *collector);

private slots:
  void onReadyRead();
  void onSessionTimeout(const quint32 clientIp);

private:
  DnsHeader parseDnsHeader(QByteArray &input);
  DnsQuestion parseDnsQuestion(QByteArray &input);
  QByteArray getQName(QByteArray &input);

private:
  QUdpSocket *m_listenSocket{nullptr};
  FragmentCollector *m_fragCollector{nullptr};

  QHash<quint32, ClientSession *> m_sessions;
};

#endif // SERVER_H
