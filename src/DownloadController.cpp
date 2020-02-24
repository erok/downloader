#include "DownloadController.h"

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QStringList>
#include <QDir>
#include <iostream>

#include "src/Downloader/DownloadSettings.h"
#include "Downloader/Downloader.h"
#include "UI/DownloadDialog.h"

DownloadController::DownloadController(QObject * parent)
    : QObject(parent)
    , m_downloader{ std::make_unique<Downloader    >() }
    , m_dialog    { std::make_unique<DownloadDialog>() }
{
    connect(m_downloader.get(), &Downloader::finished, m_dialog.get(), &DownloadDialog::close     );
    connect(m_downloader.get(), &Downloader::progress, this, [this](qint64 received, qint64 total){
        m_dialog->onProgress(received, total, m_timer.elapsed());
    });
    connect(m_downloader.get(), &Downloader::finished, QCoreApplication::instance(), &QCoreApplication::quit);

    QCommandLineParser cmdParser;
    cmdParser.setApplicationDescription("Downloader");
    cmdParser.addHelpOption();
    QCommandLineOption sourceOption     (QStringList() << "s" << "source"     , "download url"                      , "source"     );
    QCommandLineOption destinationOption(QStringList() << "d" << "destination", "Destination file"                  , "destination");
    QCommandLineOption threadCountOption(QStringList() << "c" << "count"      , "Thread count for parallel download", "count"      );
    cmdParser.addOptions({ sourceOption, destinationOption, threadCountOption });

    cmdParser.process(*QCoreApplication::instance());
    if (!cmdParser.isSet(sourceOption))
        cmdParser.showHelp(EXIT_FAILURE);

    const QUrl sourceUrl(cmdParser.value(sourceOption));
    QString dest = QDir::currentPath() + "/" + sourceUrl.fileName();
    if (cmdParser.isSet(destinationOption))
        dest = cmdParser.value(destinationOption);

    int32_t threadCount = 1;
    if (cmdParser.isSet(threadCountOption))
        threadCount = cmdParser.value(threadCountOption).toInt();

    try {
        m_settings = std::make_unique<DownloadSettings>(sourceUrl, dest, threadCount);
    }
    catch(std::invalid_argument & ex) {
        std::cerr << ex.what() << std::endl;
        cmdParser.showHelp(EXIT_FAILURE);
    }
}

void DownloadController::download() {
    m_dialog->show();
    try {
        m_downloader->download(*m_settings);
        m_timer.start();
    } catch (std::logic_error & ex) {
        std::cerr << "Failed to download " << m_settings->getUrl().toString().toStdString() << std::endl;
        std::cerr << ex.what() << std::endl;
        QCoreApplication::quit();
    }
}

DownloadController::~DownloadController() = default;
