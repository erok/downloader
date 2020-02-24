#pragma once

#include <QFile>

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

    void writeChunck(const QByteArray & chunk);

signals:
    void finished(FinishCode code);

private:
    QFile m_file;
};
