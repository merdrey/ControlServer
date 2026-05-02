#include "fragmentcollector.h"

FragmentCollector::FragmentCollector(QObject *parent) : QObject{parent} {
  m_cleanUpTimer = new QTimer();

  connect(m_cleanUpTimer, &QTimer::timeout, this, &FragmentCollector::cleanup);
}

bool FragmentCollector::addFragment(const FragmentKey &key, QByteArray &input,
                                    QByteArray &output) {
  m_cleanUpTimer->start(60 * 1000); // 60 seconds

  readQName(input);

  quint16 packetId =
      static_cast<quint8>(input[0]) << 8 | static_cast<quint8>(input[1]);
  input.remove(0, 2);
  quint8 seqIndex = static_cast<quint8>(input[0]);
  input.removeFirst();
  quint8 totalSeq = static_cast<quint8>(input[0]);
  input.removeFirst();

  if (key.id != packetId)
    return false;

  if (!m_buffers.contains(key)) {
    FragmentBuffer buf;
    buf.totalFrag = totalSeq;
    buf.received.resize(totalSeq);
    buf.received.fill(false);
    buf.fragments.resize(totalSeq);
    buf.lastUpdate = QDateTime::currentDateTime();

    m_buffers.insert(key, buf);
  }

  FragmentBuffer &buf = m_buffers[key];

  if (seqIndex > buf.totalFrag)
    return false;

  if (!buf.received[seqIndex]) {
    buf.fragments[seqIndex] = input;
    buf.received[seqIndex] = true;
    buf.receivedCount++;
    buf.lastUpdate = QDateTime::currentDateTime();
  }

  if (buf.receivedCount == buf.totalFrag) {
    for (int i = 0; i < buf.totalFrag; i++) {
      output.append(buf.fragments[i]);
    }
    m_buffers.remove(key);
    return true;
  }

  return false;
}

void FragmentCollector::cleanup() {
  auto now = QDateTime::currentDateTime();

  for (auto it = m_buffers.begin(); it != m_buffers.end();) {
    if (it.value().lastUpdate.msecsTo(now) > m_timeout) {
      it = m_buffers.erase(it);
    } else {
      ++it;
    }
  }
}

void FragmentCollector::readQName(QByteArray &input) {
  quint8 size = static_cast<quint8>(input[0]);
  input.removeFirst();
  while (input[size] != 0x00) {
    quint8 step = static_cast<quint8>(input[size]);
    input.remove(size, 1);
    size += step;
  }
  input.removeLast();
}
