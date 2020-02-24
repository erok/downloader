#include "Downloader.h"

#include <QNetworkReply>
#include <iostream>

#include "Writer.h"
#include "ReplyWrapper.h"

Downloader::Downloader(QObject *parent)
    : QObject(parent)
{
}

Downloader::~Downloader() {
    m_writerThread.quit();
    m_writerThread.wait();
}

void Downloader::download(const DownloadSettings &settings) {
    m_fileWriter.reset(new Writer(settings.getDestination()));
    m_writerThread.moveToThread(&m_writerThread);
    m_writerThread.start();

    connect(m_fileWriter.get(), &Writer::finished, this, [this](Writer::FinishCode code) {
        assert(code == Writer::FinishCode::SUCCESS);
        emit finished();
    });

    if (!m_fileWriter->isInitialized())
        throw std::logic_error("Unable to open file for writing");

    QNetworkRequest request(settings.getUrl());
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    auto head = m_manager.head(request);
    connect(head, &QNetworkReply::metaDataChanged, this, [this, &settings]() {
        downloadInternal(qobject_cast<QNetworkReply*>(sender()), settings);
    });
}

void Downloader::downloadInternal(QNetworkReply * head, const DownloadSettings & settings)
{
    m_cLength = head->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    if (head->rawHeader("Accept-Ranges") != "" && settings.getThreadCount() > 1) {

        int64_t r = (m_cLength + 1) / settings.getThreadCount();
        for (int64_t i = 0 ; i < m_cLength; i += r) {
            QNetworkRequest request(settings.getUrl());
            request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
            request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
            QString bytes = "bytes=" + QString::number(i) + "-" + (i + r <= m_cLength ? QString::number(i + r - 1) : "");
            request.setRawHeader("Range", bytes.toUtf8());
            getImpl(request, i, settings);
        }
    } else {
        QNetworkRequest request(settings.getUrl());
        request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
        getImpl(request, 0, settings);
    }
}

void Downloader::getImpl(const QNetworkRequest &request, int64_t pos, const DownloadSettings & settings) {
    ++ m_counter;
    auto * readWrapper = new ReadWrapper(this, pos);
    auto * progressWrapper = new ProgressWrapper(this, m_cLength);
    auto reply = m_manager.get(request);
    connect(reply, &QNetworkReply::finished, this, [this] {
        --m_counter;
        if (!m_counter)
            m_fileWriter->close();
    });
    connect(reply, &QNetworkReply::downloadProgress, progressWrapper, &ProgressWrapper::downloadProgress);
    connect(progressWrapper, &ProgressWrapper::progress, this, &Downloader::progress);
    connect(reply, &QNetworkReply::readyRead, readWrapper, &ReadWrapper::dataReceived);
    connect(readWrapper, &ReadWrapper::writeChunk, m_fileWriter.get(), &Writer::writeChunck);
}
