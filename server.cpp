#include "server.h"
#include <QDebug>
#include <QVariant>
#include <QColor>

Server::Server(QObject *parent)
    : QObject{parent}
{}

void Server::initSocket(const QHostAddress &addr, quint16 port)
{
    m_pudp = new QUdpSocket(this);

    m_pudp->bind(addr, port);

    connect(m_pudp, &QUdpSocket::readyRead, this, &Server::onReadyRead);
}

void Server::sendCommand(const QVariant &data, const Enums::Commands command)
{
    QByteArray packet;

    switch (command) {
    case Enums::Commands::ComFillScreen:
    {
        QColor color = QColor(data.toString());
        quint16 rgb = rgbToRgb565(color.red(), color.green(), color.blue());

        char colorData[2];
        colorData[0] = (rgb >> 8) & 0xFF;
        colorData[1] = rgb & 0xFF;

        packet.append(Enums::Commands::ComFillScreen);
        packet.append(sizeof(colorData));
        packet.append(colorData, 2);

        m_pudp->writeDatagram(packet, IP_ADDR, UDP_PORT);

        emit sendMessage("Сервер отправил команду заливки на " + IP_ADDR.toString(), Enums::Messages::Send);
        break;
    }
    case Enums::Commands::ComSendText:
    {
        QByteArray text = data.toString().toLatin1();

        packet.append(Enums::Commands::ComSendText);
        packet.append(text.size());
        packet.append(text.data());

        m_pudp->writeDatagram(packet, IP_ADDR, UDP_PORT);
        emit sendMessage("Сервер отправил данные на " + IP_ADDR.toString(), Enums::Messages::Send);
        break;
    }
    default:
        break;
    }
}

void Server::onReadyRead()
{
    while (m_pudp->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_pudp->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_pudp->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        qDebug() << datagram << datagram.size();

        if (datagram[0] == Enums::Commands::ComAnswer) { // пришел ответ
            datagram.removeFirst();

            qDebug() << datagram << datagram.size();
            quint8 ack = quint8(datagram[0]);
            qDebug() << ack;
            if (ack == 0x01) { // данные приняты
                datagram.removeFirst();


                qDebug() << datagram << datagram.size();
                char dataSize = datagram[0];
                datagram.removeFirst();

                qDebug() << datagram << datagram.size();
                if (dataSize >= datagram.size()) { // размер данных удовлетворяет требованиям
                    emit sendMessage(datagram.data(), Enums::Messages::Recieve);
                } else {
                    emit sendMessage("Данные от " + sender.toString() + " неверны", Enums::Messages::Error);
                }
            } else {
                emit sendMessage("Данные не были приняты " + sender.toString(), Enums::Messages::Error);
            }
        } else {
            emit sendMessage("Ответ не пришел от " + sender.toString(), Enums::Messages::Error);
        }
    }
}

quint16 Server::rgbToRgb565(const char r, const char g, const char b)
{
    return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
}
