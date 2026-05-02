#include "clientsession.h"
#include "fragmentbuilder.h"
#include "structs.h"
#include <QByteArray>
#include <QDebug>

ClientSession::ClientSession(const QHostAddress &clientAddr, quint16 clientPort,
                             const QHostAddress &serverAddr, quint16 serverPort,
                             QObject *parent)
    : QObject{parent} {
  m_clientAddress = clientAddr;
  m_clientPort = clientPort;

  m_serverAddress = serverAddr;
  m_serverPort = serverPort;

  m_udpSocket = new QUdpSocket(this);
  m_idleTimer = new QTimer(this);

  m_udpSocket->bind(QHostAddress::AnyIPv4, m_serverPort);
  connect(m_udpSocket, &QUdpSocket::readyRead, this,
          &ClientSession::onReadyRead);

  connect(m_idleTimer, &QTimer::timeout, this, &ClientSession::onTimeout);

  qDebug() << "Session created!"
           << "\nClient: " << clientAddr.toString() << ":" << clientPort
           << "\nServer: " << serverAddr.toString() << ":" << serverPort;
}

ClientSession::~ClientSession() {
  m_queries.clear();

  if (m_udpSocket) {
    if (m_udpSocket->isOpen())
      m_udpSocket->close();
    delete m_udpSocket;
  }

  if (m_idleTimer) {
    if (m_idleTimer->isActive())
      m_idleTimer->stop();
    delete m_idleTimer;
  }
}

void ClientSession::sendData(const QByteArray &datagram) {
  m_udpSocket->writeDatagram(datagram, m_serverAddress, m_serverPort);
  m_idleTimer->start(IDLE_TIME);
}

void ClientSession::addQuery(const DnsQuery &query) {
  if (m_queries.size() > QUEUE_SIZE) {
    m_queries.dequeue();
  }
  m_queries.enqueue(query);
}

quint32 ClientSession::getClientIp() const {
  return m_clientAddress.toIPv4Address();
}

void ClientSession::onReadyRead() {
  while (m_udpSocket->hasPendingDatagrams()) {
    QByteArray datagram;
    datagram.resize(m_udpSocket->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;

    m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender,
                              &senderPort);

    if (senderPort == m_serverPort && sender.isEqual(m_serverAddress) &&
        !m_queries.isEmpty()) {
      DnsQuery query = m_queries.dequeue();
      QVector<QByteArray> fragments = FragmentBuilder::build(query, datagram);
      for (const auto &packet : fragments) {
        m_udpSocket->writeDatagram(packet, m_clientAddress, m_clientPort);
      }
      m_idleTimer->start(IDLE_TIME);
    }
  }
}

void ClientSession::onTimeout() {
  qDebug() << "Connection closed!";
  m_udpSocket->close();
  m_queries.clear();
  emit s_connectionClosed(m_clientAddress.toIPv4Address());
}
