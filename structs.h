#ifndef STRUCTS_H
#define STRUCTS_H

#include <QDebug>
#include <QDataStream>
#include <QIODevice>

struct DnsHeader
{
    quint16 id;
    quint16 flags;
    quint16 QDCount;
    quint16 ANCount;
    quint16 NSCount;
    quint16 ARCount;

    friend QDebug operator<<(QDebug debug, const DnsHeader &header);
};

QByteArray dnsHeaderToByteArray(const DnsHeader& header);

struct DnsQuestion
{
    QByteArray QName;
    quint16 QType;
    quint16 QClass;

    friend QDebug operator<<(QDebug debug, const DnsQuestion &question);
};

QByteArray dnsQuestionToByteArray(const DnsQuestion& question);

struct DnsAnswer
{
    QByteArray QName; //pointer to DnsQuestion QName
    quint16 QType;
    quint16 QClass;
    quint32 ttl;
    quint16 dataLength;
    QByteArray txt;

    friend QDebug operator<<(QDebug debug, const DnsAnswer &answer);
};

QByteArray dnsAnswerToByteArray(const DnsAnswer& answer);

#endif // STRUCTS_H
