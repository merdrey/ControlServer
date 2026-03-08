#include "server.h"
#include <QDebug>
#include <QVariant>
#include <QColor>
#include <QtEndian>

#include "structs.h"

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

        qDebug() << datagram << sender << senderPort;

        // if (!datagram.isEmpty()) {
        //     QByteArray answer = resolveDnsAns(datagram);

        //     qDebug() << answer;

        //     if (!answer.isEmpty()) {
        //         m_pudp->writeDatagram(answer, IP_ADDR, UDP_PORT);
        //     }
        // }
    }
}

quint16 Server::rgbToRgb565(const char r, const char g, const char b)
{
    return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
}

QByteArray Server::resolveDnsAns(QByteArray &query)
{
    QByteArray response;
    DnsHeader header;
    DnsQuestion question;
    DnsAnswer answer;

    // parsing DNS header

    header.id = static_cast<quint8>(query[0]) << 8 | static_cast<quint8>(query[1]);
    query.remove(0, 2);
    header.flags = static_cast<quint8>(query[0]) << 8 | static_cast<quint8>(query[1]);
    query.remove(0, 2);
    header.QDCount = static_cast<quint8>(query[0]) << 8 | static_cast<quint8>(query[1]);
    query.remove(0, 2);
    header.ANCount = static_cast<quint8>(query[0]) << 8 | static_cast<quint8>(query[1]);
    query.remove(0, 2);
    header.NSCount = static_cast<quint8>(query[0]) << 8 | static_cast<quint8>(query[1]);
    query.remove(0, 2);
    header.ARCount = static_cast<quint8>(query[0]) << 8 | static_cast<quint8>(query[1]);
    query.remove(0, 2);

    // parsing DNS question

    question.QName = getQName(query);

    question.QType = static_cast<quint8>(query[0]) << 8 | static_cast<quint8>(query[1]);
    query.remove(0, 2);
    question.QClass = static_cast<quint8>(query[0]) << 8 | static_cast<quint8>(query[1]);
    query.remove(0, 2);

    // making DNS response

    header.ANCount += 1;
    header.flags |= 0x8000;

    answer.QName = QByteArray::fromHex("c00c");
    answer.QType = question.QType;
    answer.QClass = question.QClass;
    answer.ttl = 0x0000545f;

    switch (question.QType) {
    case 0x0001: { // A record type
        quint32 addr = QHostAddress("10.0.0.1").toIPv4Address();
        quint32 addrBe = qToBigEndian(addr);
        answer.txt = QByteArray(reinterpret_cast<const char*>(&addrBe), 4);
        answer.dataLength = answer.txt.size();
        break;
    }
    case 0x0010: { // TXT record type
        QByteArray text = QByteArray("HELLO!");
        answer.txt = QByteArray(reinterpret_cast<const char*>(text.size()), 1).append(text);
        answer.dataLength = answer.txt.size();
        break;
    }
    default: {
        qDebug() << "Неизвестный тип записи: " << question.QType;
        return QByteArray();
    }
    }

    response.append(dnsHeaderToByteArray(header));
    response.append(dnsQuestionToByteArray(question));
    qDebug() << answer;
    qDebug() << dnsAnswerToByteArray(answer);
    response.append(dnsAnswerToByteArray(answer));

    return response;
}

QByteArray Server::getQName(QByteArray &query)
{
    QByteArray name;
    quint8 length;
    while(query[0] != 0x00) {
        length = static_cast<quint8>(query[0]);
        name.append(query.sliced(0, length + 1));
        query.remove(0, length + 1);
    }
    name.append(query[0]);
    query.removeFirst();
    return name;
}
