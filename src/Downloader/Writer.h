#pragma once

#include <QObject>
#include <QFile>
#include <QNetworkReply>

class Writer : public QObject
{
    Q_OBJECT
public:
    enum class FinishCode {
          SUCCESS
        , FAILED
    };
    Q_ENUM(FinishCode)

    Writer(const QString & filepath, QObject * parent = nullptr) : QObject(parent), m_file(filepath) {
        qRegisterMetaType<Writer::FinishCode>("Writer::FinishCode");
        m_file.open(QFile::WriteOnly);
    }
    ~Writer() override { close(); }

    void close() {
        m_file.close();
        emit finished(FinishCode::SUCCESS);
    }

    [[nodiscard]] bool isInitialized() const { return m_file.isOpen(); }
    [[nodiscard]] int64_t getSize() const { return m_file.size(); }

    void writeChunck(const QByteArray & chunk, int64_t pos);

signals:
    void finished(FinishCode code);

private:
    QFile m_file;
};

