#include "udpclient.h"
#include <QColor>
#include <QVariant>

UdpClient::UdpClient(QObject *parent) : QObject{parent} {}

UdpClient::~UdpClient() {
  if (m_udpSocket)
    delete m_udpSocket;
}

void UdpClient::initSocket(const QHostAddress &addr, quint16 port) {
  m_udpSocket = new QUdpSocket();

  m_udpSocket->bind(addr, port);

  connect(m_udpSocket, &QUdpSocket::readyRead, this, &UdpClient::onReadyRead);

  m_clientAddr = QHostAddress("10.0.0.10");
}

void UdpClient::sendCommand(const QVariant &data,
                            const Enums::Commands command) {
  QByteArray packet;

  switch (command) {
  case Enums::Commands::ComFillScreen: {
    QColor color = QColor(data.toString());
    quint16 rgb = rgbToRgb565(color.red(), color.green(), color.blue());

    char colorData[2];
    colorData[0] = (rgb >> 8) & 0xFF;
    colorData[1] = rgb & 0xFF;

    packet.append(Enums::Commands::ComFillScreen);
    packet.append(sizeof(colorData));
    packet.append(colorData, 2);

    m_udpSocket->writeDatagram(packet, m_clientAddr, UDP_PORT);

    emit sendMessage("Сервер отправил команду заливки на " +
                         m_clientAddr.toString(),
                     Enums::Messages::Send);

    break;
  }
  case Enums::Commands::ComSendText: {
    QByteArray text = data.toString().toLatin1();

    packet.append(Enums::Commands::ComSendText);
    packet.append(text.size());
    packet.append(text.data());

    m_udpSocket->writeDatagram(packet, m_clientAddr, UDP_PORT);
    emit sendMessage("Сервер отправил данные на " + m_clientAddr.toString(),
                     Enums::Messages::Send);
    break;
  }
  default:
    break;
  }
}

void UdpClient::updateClientAddr(const QString &ip) {
  QHostAddress newIpAddr = QHostAddress(ip);

  if (!m_clientAddr.isEqual(newIpAddr)) {
    m_clientAddr = newIpAddr;
  }
}

void UdpClient::onReadyRead() {
  while (m_udpSocket->hasPendingDatagrams()) {
    QByteArray datagram;
    datagram.resize(m_udpSocket->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;

    m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender,
                              &senderPort);

    if (datagram[0] == Enums::Commands::ComAnswer) { // пришел ответ
      datagram.removeFirst();

      quint8 ack = quint8(datagram[0]);
      if (ack == 0x01) { // данные приняты
        datagram.removeFirst();

        char dataSize = datagram[0];
        datagram.removeFirst();

        if (dataSize >=
            datagram.size()) { // размер данных удовлетворяет требованиям
          emit sendMessage(datagram.data(), Enums::Messages::Recieve);
        } else {
          emit sendMessage("Данные от " + sender.toString() + " неверны",
                           Enums::Messages::Error);
        }
      } else {
        emit sendMessage("Данные не были приняты " + sender.toString(),
                         Enums::Messages::Error);
      }
    } else {
      emit sendMessage("Ответ не пришел от " + sender.toString(),
                       Enums::Messages::Error);
    }
  }
}

quint16 UdpClient::rgbToRgb565(const char r, const char g, const char b) {
  return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
}
