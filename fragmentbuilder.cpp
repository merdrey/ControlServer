#include "fragmentbuilder.h"
#include <QByteArray>

FragmentBuilder::FragmentBuilder(QObject *parent) : QObject{parent} {}

QVector<QByteArray> FragmentBuilder::build(DnsQuery &query, QByteArray &data) {
  QVector<QByteArray> fragments;

  int QNAME_SIZE = query.question.QName.size();
  int fragSize = qMin(DNS_MAX_SIZE - DNS_HDR_SIZE - DNS_QRY_SIZE - QNAME_SIZE -
                          DNS_ANS_SIZE,
                      DNS_DATA_SIZE);

  query.header.ANCount += 1;
  query.header.flags |= 0x8000;
  query.answer.QName = QByteArray::fromHex("c00c");
  query.answer.QType = query.question.QType;
  query.answer.QClass = query.question.QClass;
  query.answer.ttl = 0x0000545f;

  if (data.size() > fragSize) {
    int dataSize = data.size();
    int totalFragments = (dataSize + fragSize - 1) / fragSize;

    qDebug() << dataSize << totalFragments;

    for (int i = 0; i < totalFragments; i++) {
      int offset = i * fragSize;
      int size = qMin(fragSize, dataSize - offset);

      QByteArray payload;
      QByteArray mainData = data.mid(offset, size);

      FragmentHeader fragHdr;
      fragHdr.id = query.header.id;
      fragHdr.offset = offset;
      fragHdr.totalLen = dataSize;

      payload.append(1, char(sizeof(fragHdr)));
      payload.append(reinterpret_cast<char *>(&fragHdr), sizeof(fragHdr));
      payload.append(1, char(mainData.size()));
      payload.append(mainData);

      query.answer.txt = payload;
      query.answer.dataLength = payload.size();

      fragments.append(dnsQueryToByteArray(query));
    }

  } else {
    FragmentHeader fragHdr;
    fragHdr.id = query.header.id;
    fragHdr.offset = 0;
    fragHdr.totalLen = data.size();

    QByteArray payload;

    payload.append(1, char(sizeof(fragHdr)));
    payload.append(reinterpret_cast<char *>(&fragHdr), sizeof(fragHdr));
    payload.append(1, char(data.size()));
    payload.append(data);

    query.answer.txt = payload;
    query.answer.dataLength = payload.size();

    fragments.append(dnsQueryToByteArray(query));
  }

  return fragments;
}
