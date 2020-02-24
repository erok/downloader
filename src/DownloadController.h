#pragma once

#include <memory>
#include <QObject>
#include <QElapsedTimer>

#include "src/Downloader/DownloadSettings.h"

class Downloader;
class DownloadDialog;

class DownloadController : public QObject {
public:
    DownloadController(QObject * parent = nullptr);
    ~DownloadController();

    void download();

private:
    std::unique_ptr<Downloader>       m_downloader;
    std::unique_ptr<DownloadSettings> m_settings  ;
    std::unique_ptr<DownloadDialog>   m_dialog    ;
    QElapsedTimer                     m_timer     ;

};
