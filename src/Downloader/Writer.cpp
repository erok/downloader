#include "Writer.h"

void Writer::writeChunck(const QByteArray &chunk) {
    if (m_file.write(chunk) == -1)
        emit finished(FinishCode::FAILED);
}
