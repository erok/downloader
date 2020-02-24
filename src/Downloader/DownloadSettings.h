#pragma once

#include <QString>
#include <QUrl>

class DownloadSettings {
public:
    DownloadSettings(const QUrl & source, const QString & dest, int32_t threadCount)
        : m_source     (source     )
        , m_destination(dest       )
        , m_threadCount(threadCount)
    {
        if (threadCount < 1)
            throw std::invalid_argument("incorrect thread count value");
    }

    DownloadSettings() = delete;
    ~DownloadSettings() = default;
    DownloadSettings(const DownloadSettings &) = default;
    DownloadSettings(DownloadSettings &&     ) = default;
    DownloadSettings & operator=(const DownloadSettings &) = default;
    DownloadSettings & operator=(DownloadSettings &&     ) = default;

    [[nodiscard]] QUrl    getUrl        () const { return m_source     ; }
    [[nodiscard]] QString getDestination() const { return m_destination; }
    [[nodiscard]] int32_t getThreadCount() const { return m_threadCount; }

private:
    QUrl    m_source     ;
    QString m_destination;
    int32_t m_threadCount;
};
