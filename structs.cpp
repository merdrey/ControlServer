#include "structs.h"


QDebug operator<<(QDebug debug, const DnsHeader &header)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << QString::number(header.id, 16) << QString::number(header.flags, 16)
                    << QString::number(header.QDCount, 16) << QString::number(header.ANCount, 16)
                    << QString::number(header.NSCount, 16) << QString::number(header.ARCount, 16);
    return debug;
}

QDebug operator<<(QDebug debug, const DnsQuestion &question)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << question.QName
                    << question.QType
                    << question.QClass;
    return debug;
}

QDebug operator<<(QDebug debug, const DnsAnswer &answer)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << answer.QName
                    << answer.QType
                    << answer.QClass
                    << answer.ttl
                    << answer.dataLength
                    << answer.txt;
    return debug;
}


QByteArray dnsHeaderToByteArray(const DnsHeader &header)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    stream << header.id
           << header.flags
           << header.QDCount
           << header.ANCount
           << header.NSCount
           << header.ARCount;

    return data;
}

QByteArray dnsQuestionToByteArray(const DnsQuestion &question)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    stream << question.QType
           << question.QClass;

    return data.prepend(question.QName);
}

QByteArray dnsAnswerToByteArray(const DnsAnswer &answer)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    stream << answer.QType
           << answer.QClass
           << answer.ttl
           << answer.dataLength;

    return data.prepend(answer.QName).append(answer.txt);
}
