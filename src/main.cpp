#include <QApplication>

#include "DownloadController.h"

int main(int argc, char * argv[]) {
    QApplication app(argc, argv);

    DownloadController controller(&app);
    controller.download();

    return QApplication::exec();
}
