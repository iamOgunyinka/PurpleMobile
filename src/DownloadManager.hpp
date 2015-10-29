#ifndef DOWNLOADMANAGER_HPP_
#define DOWNLOADMANAGER_HPP_

#include "DownloadInterface.hpp"
#include <QString>
#include <QFile>
#include <QQueue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTime>

namespace Purple
{

    struct Downloads
    {
        QString m_key, m_url, m_destFolder, m_tempFolder;
        QFile *m_file;
        QTime m_time;
        bool  m_tempExists;
        int   m_tempSize;
    };

    class DownloadManager: public DownloadInterface
    {
        Q_OBJECT
    public:
        DownloadManager( QObject *parent = 0 );
        virtual ~DownloadManager();

        Q_INVOKABLE void            startDownload( QString const & url );
        Q_INVOKABLE void            stopDownload( QString const & url );
        Q_INVOKABLE void            pauseDownload( QString const & url );
        Q_INVOKABLE void            resumeDownload( QString const & url, QString const & path = QString() );
    public slots:
        void        downloadProgress( qint64 downloaded, qint64 total );
        void        downloadFinished();
        void        downloadError( QNetworkReply::NetworkError );
    signals:
        void        emptyDownloadQueue();
    private:
        void        setupDownloadManager();
        void        startDownloadImpl( QString const & url, QString const & path = QString() );
        void        stopDownloadImpl( QString const & url, bool pause );
        QString     saveFilename( QString const & url, bool &fileExists, QString & filename, bool &tempExists, bool isUrl );
        void        startNextDownload();
    private:
        QQueue<Downloads>      m_downloadQueue;
        QNetworkAccessManager  m_networkManager;
        QList<Downloads>       m_completedList;
    };

} /* namespace Purple */

#endif /* DOWNLOADMANAGER_HPP_ */
