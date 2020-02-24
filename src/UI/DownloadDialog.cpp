#include "DownloadDialog.h"
#include "ui_DownloadDialog.h"
#include <QDebug>
#include <QTime>

DownloadDialog::DownloadDialog(QWidget *parent)
    : QWidget{ parent }
    , m_ui   { std::make_unique<Ui::DownloadDialog>() }
{
    m_ui->setupUi(this);
}

void DownloadDialog::onProgress(qint64 received, quint64 total, quint64 timeElapsed) {
    auto r = static_cast<double>(received);
    auto t = static_cast<double>(total   );
    m_ui->progressBar->setValue(static_cast<int>(r / t * 100));
    const double speed = r / timeElapsed;
    const double msLeftD = (t - r) / speed;
    const int msLeft = static_cast<int>(msLeftD);
    QTime timeLeft(0, 0);
    timeLeft = timeLeft.addMSecs(msLeft);
    QString timeStr = timeLeft.hour() > 0
            ? QString("%1 hours %2 minutes %3 seconds remaining").arg(timeLeft.hour()).arg(timeLeft.minute()).arg(timeLeft.second())
            : timeLeft.minute() > 0
                ? QString("%1 minutes %2 seconds remaining").arg(timeLeft.minute()).arg(timeLeft.second())
                : QString("%1 seconds remaining").arg(timeLeft.second())
            ;
    m_ui->progressLabel->setText(timeStr);
}

DownloadDialog::~DownloadDialog() = default;
