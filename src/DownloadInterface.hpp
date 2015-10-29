#ifndef DOWNLOADINTERFACE_HPP_
#define DOWNLOADINTERFACE_HPP_

#include <QString>

namespace Purple
{
    class DownloadInterface: public QObject
    {
        Q_OBJECT

    public:
        DownloadInterface( QObject *parent = 0 ): QObject( parent ), m_filePath(), m_userAgent(), m_downloadQueueSize(2) {}
        virtual ~DownloadInterface() {}
        virtual void            startDownload( QString const & url ) = 0;
        virtual void            stopDownload( QString const & url ) = 0;
        virtual void            pauseDownload( QString const & url ) = 0;
        virtual void            resumeDownload( QString const & url, QString const & path = QString() ) = 0;
        virtual inline void     setDownloadQueueSize( int size ){ m_downloadQueueSize = size; }
        virtual inline int      downloadQueueSize() const { return m_downloadQueueSize; }
        virtual inline void     setFilePath( QString const & file_path ){ m_filePath = file_path; }
        virtual inline QString  filePath() const { return m_filePath; }

    protected:
        QString m_filePath;
        QByteArray m_userAgent;
        int m_downloadQueueSize;
    };

} /* namespace Purple */

#endif /* DOWNLOADINTERFACE_HPP_ */
