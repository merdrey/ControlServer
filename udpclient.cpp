#include "udpclient.h"
#include "structs.h"
#include <QColor>
#include <QImage>
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

    char dataSize[2];
    dataSize[0] = (sizeof(colorData) >> 8) & 0xFF;
    dataSize[1] = sizeof(colorData) & 0xFF;

    packet.append(Enums::Commands::ComFillScreen);
    packet.append(dataSize, 2);
    packet.append(colorData, 2);

    m_udpSocket->writeDatagram(packet, m_clientAddr, UDP_PORT);

    emit sendMessage("Сервер отправил команду заливки на " +
                         m_clientAddr.toString(),
                     Enums::Messages::Send);

    break;
  }
  case Enums::Commands::ComSendText: {
    QByteArray text = data.toString().toLatin1();

    char dataSize[2];
    dataSize[0] = (text.size() >> 8) & 0xFF;
    dataSize[1] = text.size() & 0xFF;

    packet.append(Enums::Commands::ComSendText);
    packet.append(dataSize, 2);
    packet.append(text.data());

    m_udpSocket->writeDatagram(packet, m_clientAddr, UDP_PORT);
    emit sendMessage("Сервер отправил данные на " + m_clientAddr.toString(),
                     Enums::Messages::Send);
    break;
  }
  case Enums::Commands::ComSendImg: {
    QImage img = QImage(":/qr-code.gif");

    QByteArray resImg = convertImgToRGB565(img);

    ImageHeader hdr;

    hdr.width = img.width();
    hdr.height = img.height();
    hdr.size = resImg.size();

    int size = resImg.size() + sizeof(ImageHeader);

    char dataSize[2];
    dataSize[0] = (size >> 8) & 0xFF;
    dataSize[1] = size & 0xFF;

    packet.append(Enums::Commands::ComSendImg);
    packet.append(dataSize, 2);
    packet.append(reinterpret_cast<char *>(&hdr), sizeof(ImageHeader));
    packet.append(resImg);

    qDebug() << packet.size() << resImg.size();

    m_udpSocket->writeDatagram(packet, m_clientAddr, UDP_PORT);
    emit sendMessage("Сервер отправил изображение на " +
                         m_clientAddr.toString(),
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

        quint16 dataSize = static_cast<quint8>(datagram[0]) << 8 |
                           static_cast<quint8>(datagram[1]);
        ;
        datagram.remove(0, 2);

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

QByteArray UdpClient::convertImgToRGB565(const QImage &img) {
  QImage rgb = img.convertToFormat(QImage::Format_RGB888);

  QByteArray out;
  out.resize(rgb.width() * rgb.height() * 2);

  quint8 *dst = reinterpret_cast<quint8 *>(out.data());

  for (int y = 0; y < rgb.height(); y++) {
    const uchar *line = rgb.constScanLine(y);

    for (int x = 0; x < rgb.width(); x++) {
      char r = line[x * 3 + 0];
      char g = line[x * 3 + 1];
      char b = line[x * 3 + 2];

      quint16 c = rgbToRgb565(r, g, b);

      *dst++ = c & 0xFF;
      *dst++ = (c >> 8) & 0xFF;
    }
  }

  return out;
}
