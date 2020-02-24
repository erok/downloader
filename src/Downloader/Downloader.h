#pragma once

#include <QNetworkAccessManager>

#include "DownloadSettings.h"

class Writer;

class Downloader : public QObject {
    Q_OBJECT
public:
    Downloader(QObject * parent = nullptr);
    ~Downloader() override;

    void download(const DownloadSettings & settings);
    void cancel();

signals:
    void progress (qint64, qint64);
    void finished (              );
    void writeData(QByteArray    );

private:
    QNetworkAccessManager  m_manager   ;
    QScopedPointer<Writer> m_fileWriter;

};
