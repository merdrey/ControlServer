#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include <QHostAddress>
#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QUdpSocket>

#include "structs.h"

#define IDLE_TIME 60 * 1000
#define QUEUE_SIZE 100

class ClientSession : public QObject {
  Q_OBJECT
public:
  explicit ClientSession(const QHostAddress &clientAddr, quint16 clientPort,
                         const QHostAddress &serverAddr, quint16 serverPort,
                         QObject *parent = nullptr);
  ~ClientSession();

  void sendData(const QByteArray &datagram); // sends data to server
  void addQuery(const DnsQuery &query);

  quint32 getClientIp() const;

private slots:
  void onReadyRead(); // sends data to clients
  void onTimeout();

signals:
  void s_connectionClosed(const quint32 clientIp);

private:
  QUdpSocket *m_udpSocket{nullptr};

  QQueue<DnsQuery> m_queries;

  QHostAddress m_clientAddress;
  quint16 m_clientPort;

  QHostAddress m_serverAddress;
  quint16 m_serverPort;

  QTimer *m_idleTimer{nullptr};
};

#endif // CLIENTSESSION_H
