#include "server.h"
#include <QDebug>
#include <QVariant>
#include <QtEndian>

#include "structs.h"

Server::Server(QObject *parent) : QObject{parent} {}

Server::~Server() {
  if (m_listenSocket)
    delete m_listenSocket;
}

void Server::initSocket(const QHostAddress &addr, quint16 port) {
  m_listenSocket = new QUdpSocket(this);

  m_listenSocket->bind(addr, port);

  connect(m_listenSocket, &QUdpSocket::readyRead, this, &Server::onReadyRead);
}

void Server::setFragCollector(FragmentCollector *collector) {
  if (!collector)
    return;
  m_fragCollector = collector;
}

void Server::onReadyRead() {
  while (m_listenSocket->hasPendingDatagrams()) {
    QByteArray datagram;
    datagram.resize(m_listenSocket->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;

    m_listenSocket->readDatagram(datagram.data(), datagram.size(), &sender,
                                 &senderPort);

    quint32 senderIp = sender.toIPv4Address();
    DnsQuery query;
    DnsHeader header = parseDnsHeader(datagram);
    DnsQuestion question = parseDnsQuestion(datagram);

    query.header = header;
    query.question = question;

    FragmentKey key{header.id, senderIp, senderPort};

    QByteArray output;
    if (m_fragCollector->addFragment(key, question.QName, output)) {
      if (m_sessions.contains(senderIp)) {
        m_sessions[senderIp]->sendData(output);
        m_sessions[senderIp]->addQuery(query);
      } else {
        ClientSession *newSession =
            new ClientSession(sender, senderPort, SERV_ADDR, SERV_PORT);

        connect(newSession, &ClientSession::s_connectionClosed, this,
                &Server::onSessionTimeout);

        newSession->sendData(output);
        newSession->addQuery(query);
        m_sessions[senderIp] = newSession;
      }
    }
  }
}

void Server::onSessionTimeout(const quint32 clientIp) {
  m_sessions.remove(clientIp);
}

DnsHeader Server::parseDnsHeader(QByteArray &input) {
  DnsHeader header;

  header.id =
      static_cast<quint8>(input[0]) << 8 | static_cast<quint8>(input[1]);
  input.remove(0, 2);
  header.flags =
      static_cast<quint8>(input[0]) << 8 | static_cast<quint8>(input[1]);
  input.remove(0, 2);
  header.QDCount =
      static_cast<quint8>(input[0]) << 8 | static_cast<quint8>(input[1]);
  input.remove(0, 2);
  header.ANCount =
      static_cast<quint8>(input[0]) << 8 | static_cast<quint8>(input[1]);
  input.remove(0, 2);
  header.NSCount =
      static_cast<quint8>(input[0]) << 8 | static_cast<quint8>(input[1]);
  input.remove(0, 2);
  header.ARCount =
      static_cast<quint8>(input[0]) << 8 | static_cast<quint8>(input[1]);
  input.remove(0, 2);

  return header;
}

DnsQuestion Server::parseDnsQuestion(QByteArray &input) {
  DnsQuestion question;

  question.QName = getQName(input);

  question.QType =
      static_cast<quint8>(input[0]) << 8 | static_cast<quint8>(input[1]);
  input.remove(0, 2);
  question.QClass =
      static_cast<quint8>(input[0]) << 8 | static_cast<quint8>(input[1]);
  input.remove(0, 2);

  return question;
}

QByteArray Server::getQName(QByteArray &input) {
  QByteArray name;
  quint8 length;
  while (input[0] != 0x00) {
    length = static_cast<quint8>(input[0]);
    name.append(input.sliced(0, length + 1));
    input.remove(0, length + 1);
  }
  name.append(input[0]);
  input.removeFirst();

  return name;
}
