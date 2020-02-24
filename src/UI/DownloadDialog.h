#pragma once

#include <QWidget>
#include <memory>

namespace Ui {
class DownloadDialog;
}

class DownloadDialog : public QWidget
{
    Q_OBJECT
public:
    explicit DownloadDialog(QWidget *parent = nullptr);
    ~DownloadDialog() override;

    void onProgress(qint64 received, quint64 total, quint64 timeElapsed);

private:
    std::unique_ptr<Ui::DownloadDialog>  m_ui;
};
