#ifndef STRUCTS_H
#define STRUCTS_H

#include <QDataStream>
#include <QDateTime>
#include <QDebug>
#include <QIODevice>

struct DnsHeader {
  quint16 id;
  quint16 flags;
  quint16 QDCount;
  quint16 ANCount;
  quint16 NSCount;
  quint16 ARCount;

  friend QDebug operator<<(QDebug debug, const DnsHeader &header);
};

QByteArray dnsHeaderToByteArray(const DnsHeader &header);

struct DnsQuestion {
  QByteArray QName;
  quint16 QType;
  quint16 QClass;

  friend QDebug operator<<(QDebug debug, const DnsQuestion &question);
};

QByteArray dnsQuestionToByteArray(const DnsQuestion &question);

struct DnsAnswer {
  QByteArray QName; // pointer to DnsQuestion QName
  quint16 QType;
  quint16 QClass;
  quint32 ttl;
  quint16 dataLength;
  QByteArray txt;

  friend QDebug operator<<(QDebug debug, const DnsAnswer &answer);
};

QByteArray dnsAnswerToByteArray(const DnsAnswer &answer);

struct DnsQuery {
  DnsHeader header;
  DnsQuestion question;
  DnsAnswer answer;
};

QByteArray dnsQueryToByteArray(const DnsQuery &query);

struct FragmentKey {
  quint16 id;
  quint32 ip_addr;
  quint16 port;

  bool operator==(const FragmentKey &other) const;
};

inline size_t qHash(const FragmentKey &key, size_t seed) {
  size_t hashValue = qHash(key.id, seed);
  hashValue = qHash(key.ip_addr, hashValue);
  return qHash(key.port, hashValue);
}

struct FragmentBuffer {
  QVector<QByteArray> fragments;
  QVector<bool> received;

  quint16 totalFrag = 0;
  quint16 receivedCount = 0;

  QDateTime lastUpdate;
};

struct FragmentHeader {
  quint16 id;
  quint16 offset;
  quint16 totalLen;
};

struct ImageHeader {
  quint16 width;
  quint16 height;
  quint32 size;
};

#endif // STRUCTS_H
