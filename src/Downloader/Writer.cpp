#include "Writer.h"

void Writer::writeChunck(const QByteArray &chunk, int64_t pos) {
    if (!m_file.seek(pos))
        emit finished(FinishCode::FAILED);
    if(m_file.write(chunk, chunk.size()) == -1)
        emit finished(FinishCode::FAILED);
}
