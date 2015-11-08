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
    };

    class DownloadManager: public QObject
    {
        Q_OBJECT

    public:
        DownloadManager( QObject *parent = 0 );
        virtual ~DownloadManager();

        void            startDownload( QString const & url );
        void            stopDownload( QString const & url );
        void            pauseDownload( QString const & url );
        void            resumeDownload( QString const & url, QString const & path = QString() );
    private:
        void            setupDownloadManager();
        void            openDownloadList();
        void            startDownloadImpl( QString const & url, QString const & path = QString() );
        void            stopDownloadImpl( QString const & url, bool pause );
        bool            hasRedirect( QNetworkReply *reply );
        void            writeDownloadToFile( QString const & url );
        bool            isValidUrl( QString const & url );
        QNetworkReply*  startDownloadRequest( QNetworkRequest const & request );
        QString         saveFilename( QString const & url, bool &fileExists, QString & filename, bool &tempExists, bool isUrl );
    private:
        PartialDownloadPolicy           m_partialDownloadPolicy;
        ExistDownloadPolicy             m_existDownloadPolicy;
        qint64                          m_downloadQueueSize;
        QString                         m_filePath;
        QString                         m_userAgent;
        QQueue<Downloads>               m_downloadQueue;
        QNetworkAccessManager           m_networkManager;
        QVariantList                    m_downloadList;
        QHash<QNetworkReply*, Downloads> m_downloadHash;
        QHash<QString, QNetworkReply*>  m_urlHash;
    private slots:
        void startNextDownload();
        void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
        void downloadReadyRead();
        void downloadFinished();
        void downloadError(QNetworkReply::NetworkError);
        void updateDownload( QString const &url, qint64 actualReceived, qint64 actualTotal, int percent, double speed, QString const & unit  );
    signals:
        void        status( const QString &url, const QString &title, const QString &message, const QString &data);
        void        progress( QString const &url, qint64, qint64, int, double, QString const & unit );
        void        finished( QString const & url, QString const & destination );
        void        downloadQueueEmpty();
        void        newDownloadAdded();
    };

} /* namespace Purple */

#endif /* DOWNLOADMANAGER_HPP_ */
