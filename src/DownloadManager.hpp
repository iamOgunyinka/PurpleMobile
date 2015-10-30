#ifndef DOWNLOADMANAGER_HPP_
#define DOWNLOADMANAGER_HPP_

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QQueue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTime>
#include <QHash>

namespace Purple
{
    enum PartialDownloadPolicy
    {
        ContinueDownload = 0,
        RemoveDownload
    };

    enum ExistDownloadPolicy
    {
        CancelDownloadFile = 0,
        OverwriteFile,
        RenameFile
    };

    struct Downloads
    {
        QString m_key;
        QString m_url;
        QString m_destFile;
        QString m_tempFile;
        QFile   *m_file;
        QTime   m_time;
        bool    m_tempExists;
        int     m_tempSize;

        Downloads(): m_key(), m_url(), m_destFile(), m_tempFile(), m_file( NULL ), m_time(), m_tempExists(false), m_tempSize(0){}
        ~Downloads() { if( m_file ) delete m_file; }
    };

    class DownloadManager: public QObject
    {
        Q_OBJECT

    public:
        DownloadManager( QObject *parent = 0 );
        virtual ~DownloadManager();

        Q_INVOKABLE void            startDownload( QString const & url );
        Q_INVOKABLE void            stopDownload( QString const & url );
        Q_INVOKABLE void            pauseDownload( QString const & url );
        Q_INVOKABLE void            resumeDownload( QString const & url, QString const & path = QString() );
    private:
        void        setupDownloadManager();
        void        startDownloadImpl( QString const & url, QString const & path = QString() );
        void        stopDownloadImpl( QString const & url, bool pause );
        QString     saveFilename( QString const & url, bool &fileExists, QString & filename, bool &tempExists, bool isUrl );
    private:
        PartialDownloadPolicy           m_partialDownloadPolicy;
        ExistDownloadPolicy             m_existDownloadPolicy;
        qint64                          m_downloadQueueSize;
        QString                         m_filePath;
        QString                         m_userAgent;
        QQueue<Downloads>               m_downloadQueue;
        QNetworkAccessManager           m_networkManager;
        QList<Downloads>                m_completedList;
        QHash<QNetworkReply*, Downloads> m_downloadHash;
        QHash<QString, QNetworkReply*>  m_urlHash;
    private slots:
        void startNextDownload();
        void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
//        void downloadReadyRead();
        void downloadFinished();
        void downloadError(QNetworkReply::NetworkError);
    signals:
        void        status( const QString &url, const QString &title, const QString &message, const QString &data);
        void        progress( QString const &url, qint64, qint64, int, double, QString const & unit );
        void        finished( QString const & url, QString const & destination );
        void        downloadQueueEmpty();
    };

} /* namespace Purple */

#endif /* DOWNLOADMANAGER_HPP_ */