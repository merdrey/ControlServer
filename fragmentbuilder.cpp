#include "fragmentbuilder.h"
#include <QByteArray>

FragmentBuilder::FragmentBuilder(QObject *parent) : QObject{parent} {}

QVector<QByteArray> FragmentBuilder::build(DnsQuery &query, QByteArray &data) {
  QVector<QByteArray> fragments;

  quint16 QNAME_SIZE = query.question.QName.size();

  if (data.size() >
      DNS_MAX_SIZE - DNS_HDR_SIZE - DNS_QRY_SIZE - QNAME_SIZE - DNS_ANS_SIZE) {
    qDebug() << "fragmentation needed\n" << "data size: " << data.size();
  } else {
    query.header.ANCount += 1;
    query.header.flags |= 0x8000;
    query.answer.QName = QByteArray::fromHex("c00c");
    query.answer.QType = query.question.QType;
    query.answer.QClass = query.question.QClass;
    query.answer.ttl = 0x0000545f;
    query.answer.txt = QByteArray(1, char(data.size())).append(data);
    query.answer.dataLength = query.answer.txt.size();

    fragments.append(dnsQueryToByteArray(query));
  }

  return fragments;
}
