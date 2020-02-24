#include "ReplyWrapper.h"

void ReadWrapper::dataReceived()  {
    QByteArray chunk = qobject_cast<QNetworkReply*>(sender())->readAll();
    emit writeChunk(chunk, m_initialShift + m_addShift);
    m_addShift += chunk.size();
}
