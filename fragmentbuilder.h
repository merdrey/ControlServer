#ifndef FRAGMENTBUILDER_H
#define FRAGMENTBUILDER_H

#include <QObject>

#include "structs.h"

#define DNS_MAX_SIZE 512
#define DNS_HDR_SIZE 12
#define DNS_QRY_SIZE 4
#define DNS_ANS_SIZE 8
#define DNS_DATA_SIZE 240

class FragmentBuilder : public QObject {
  Q_OBJECT
public:
  explicit FragmentBuilder(QObject *parent = nullptr);

  static QVector<QByteArray> build(DnsQuery &query, QByteArray &data);

signals:
};

#endif // FRAGMENTBUILDER_H
