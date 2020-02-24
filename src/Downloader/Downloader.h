#pragma once

#include <QNetworkAccessManager>
#include <QThread>

#include "DownloadSettings.h"

class Writer;

class Downloader : public QObject {
    Q_OBJECT
public:
    Downloader(QObject * parent = nullptr);
    ~Downloader() override;

    void download(const DownloadSettings & settings);

signals:
    void progress (qint64, qint64);
    void finished (              );

private:
    void downloadInternal(QNetworkReply * head, const DownloadSettings & settings);
    void getImpl(const QNetworkRequest &request, int64_t pos, const DownloadSettings & settings);

private:
    QNetworkAccessManager  m_manager   ;
    QScopedPointer<Writer> m_fileWriter;
    QThread                m_writerThread;
    int m_counter {0};
    int64_t m_cLength {0};

};
