#pragma once

#include <QNetworkReply>

class ReadWrapper : public QObject {
    Q_OBJECT
public:
    ReadWrapper(QObject * parent, int64_t initialShift)
            : QObject(parent)
            , m_initialShift(initialShift)
            , m_addShift(0)
    {}

    void dataReceived();

signals:
    void writeChunk(const QByteArray & chunk, int64_t pos);

private:
    int64_t m_initialShift;
    int64_t m_addShift;

};

class ProgressWrapper : public QObject {
    Q_OBJECT
public:
    ProgressWrapper(QObject * parent, int64_t cLength) : QObject(parent), m_cLength(cLength) {}

    void downloadProgress(qint64 received, qint64 ) {
        emit progress(received - m_lastValue, m_cLength);
        m_lastValue = received;
    }

signals:
    void progress(qint64, qint64);

private:
    int64_t m_lastValue {0};
    int64_t m_cLength {0};
};
