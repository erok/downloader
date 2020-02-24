#include "Downloader.h"

#include <QNetworkReply>

#include "Writer.h"

Downloader::Downloader(QObject *parent)
    : QObject(parent)
{
}

Downloader::~Downloader() = default;

void Downloader::download(const DownloadSettings &settings) {
    m_fileWriter.reset(new Writer(settings.getDestination()));

    connect(this, &Downloader::writeData, m_fileWriter.get(), &Writer::writeChunck);
    connect(m_fileWriter.get(), &Writer::finished, this, [this](Writer::FinishCode code) {
        assert(code == Writer::FinishCode::SUCCESS);
        emit finished();
    });

    if (!m_fileWriter->isInitialized())
        throw std::logic_error("Unable to open file for writing");

    QNetworkRequest request(settings.getUrl());
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    auto reply = m_manager.get(request);
    connect(reply, &QNetworkReply::finished        , m_fileWriter.get(), &Writer::close);
    connect(reply, &QNetworkReply::downloadProgress, this, &Downloader::progress);
    connect(reply, &QNetworkReply::readyRead       , this, [this]() {
        auto reply = qobject_cast<QNetworkReply*>(sender());
        if (!reply)
            return;
        emit writeData(reply->readAll());
    });
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), [this, &reply](QNetworkReply::NetworkError code){
        if (code == QNetworkReply::NetworkError::TimeoutError) {
            m_manager.get(QNetworkRequest{ reply->url() });
        }
    });
}
