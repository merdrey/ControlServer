#ifndef FRAGMENTCOLLECTOR_H
#define FRAGMENTCOLLECTOR_H

#include <QHash>
#include <QObject>
#include <QTimer>

#include "structs.h"

class FragmentCollector : public QObject {
  Q_OBJECT
public:
  explicit FragmentCollector(QObject *parent = nullptr);

  bool addFragment(const FragmentKey &key, QByteArray &input,
                   QByteArray &output);

signals:

private slots:
  void cleanup();
  void readQName(QByteArray &input);

private:
  QHash<FragmentKey, FragmentBuffer> m_buffers;
  QTimer *m_cleanUpTimer{nullptr};

  int m_timeout = 5 * 10000; // 50 seconds
};

#endif // FRAGMENTCOLLECTOR_H
