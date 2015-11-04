#include <src/DownloadManager.hpp>
#include <QTimer>
#include <QUuid>
#include <QDebug>
#include <bb/data/JsonDataAccess.hpp>

namespace Purple
{
    DownloadManager::DownloadManager( QObject *parent ): QObject( parent )
    {
        setupDownloadManager();
    }

    DownloadManager::~DownloadManager()
    {

    }

    void DownloadManager::startDownload( QString const & url )
    {
        if( !isValidUrl( url ) ){
            qDebug() << url << " is invalid.";
            emit status( url, "Error", "Invalid URL", "" );
            return;
        }
        startDownloadImpl( url );
        writeDownloadToFile( url );
    }

    void DownloadManager::writeDownloadToFile( QString const & url )
    {
        bb::data::JsonDataAccess jda;
        QVariantList list = jda.load( "app/data/assets/download_queue.json" ).toList();
        if( !jda.hasError() ){
            Downloads item = m_downloadHash[ m_urlHash[url] ];
            QVariantMap newDownload;
            newDownload[ "title" ] = item.m_file->fileName();
            newDownload[ "url"] = item.m_url;
            newDownload["thumbnail"] = QString( "app/data/assets/downloads_icon.png" );
            newDownload["status"] = "incomplete";
            newDownload["percentage"] = 0;

            list.append( newDownload );
            jda.save( list, "app/data/assets/download_queue.json" );
            if( jda.hasError() ){
                qDebug() << "Unable to save new JSON file";
            }
            emit newDownloadAdded();
        }
    }

    void DownloadManager::startDownloadImpl( QString const & url, QString const & path )
    {
        bool fileExist = false;
        bool tempExist = false;
        QString fileName = "";
        QString filePath = saveFilename( path == "" ? url : path, fileExist, fileName, tempExist, path == "" );

        if ( fileExist && m_existDownloadPolicy == CancelDownloadFile) {
            emit status( url, "Cancel", "File already exist", filePath );
            return;
        }

        Downloads item;
        item.m_url = url;
        item.m_key = fileName;
        item.m_destFile = filePath;
        item.m_tempFile = filePath + ".part";
        item.m_tempExists = tempExist;

        if ( m_downloadQueue.isEmpty() )
            QTimer::singleShot( 0, this, SLOT( startNextDownload() ) );

        m_downloadQueue.enqueue(item);
    }

    bool DownloadManager::isValidUrl( QString const & url )
    {
        return QUrl( url, QUrl::StrictMode ).isValid();
    }

    QQueue<Downloads> DownloadManager::downloads() const { return m_downloadQueue; }
    void DownloadManager::stopDownload( QString const & url )
    {
        stopDownloadImpl( url, false );
    }

    void DownloadManager::stopDownloadImpl( QString const & url, bool pause )
    {
        QNetworkReply *reply = m_urlHash[url];

        if( reply ) {
            QObject::disconnect( reply, SIGNAL( downloadProgress( qint64, qint64 ) ), this, SLOT( downloadProgress( qint64, qint64 )));
            QObject::disconnect( reply, SIGNAL( finished() ), this, SLOT( downloadFinished() ));
            QObject::disconnect( reply, SIGNAL( readyRead() ), this, SLOT( downloadReadyRead() ) );
            QObject::disconnect( reply, SIGNAL( error( QNetworkReply::NetworkError )), this, SLOT( downloadError( QNetworkReply::NetworkError )));

            Downloads item = m_downloadHash[ reply ];
            reply->abort();
            item.m_file->write( reply->readAll() );
            item.m_file->close();

            if ( !pause ) {
                QFile::remove( item.m_tempFile );
            }

            m_downloadHash.remove( reply );
            m_urlHash.remove( url );
            startNextDownload();

            reply->deleteLater();
        }
    }

    void DownloadManager::pauseDownload( QString const & url )
    {
        stopDownloadImpl( url, true );
    }

    void DownloadManager::resumeDownload( QString const & url, QString const & path )
    {
        startDownloadImpl( url, path );
    }

    void DownloadManager::setupDownloadManager()
    {
        m_userAgent = "PurpleMobile/1.0";
        m_downloadQueueSize = 2;
        m_filePath = "/accounts/1000/shared/downloads";
        m_partialDownloadPolicy = ContinueDownload;
    }

    void DownloadManager::startNextDownload()
    {
        if( m_downloadQueue.isEmpty() ) {
            emit downloadQueueEmpty();
            return;
        }

        if( m_downloadHash.size() < m_downloadQueueSize ){
            Downloads nextItem = m_downloadQueue.dequeue();

            QNetworkRequest request;
            request.setUrl( nextItem.m_url );
            request.setRawHeader( "USER-AGENT", m_userAgent.toUtf8() );

            if( m_partialDownloadPolicy == ContinueDownload && nextItem.m_tempExists ){
                nextItem.m_file = new QFile( nextItem.m_tempFile );
                if( !nextItem.m_file->open( QIODevice::ReadWrite ) ){
                    emit status( nextItem.m_url, "Error", nextItem.m_file->errorString(), nextItem.m_destFile );
                    startNextDownload();
                    return;
                }
                nextItem.m_file->seek( nextItem.m_file->size() );
                nextItem.m_tempSize = nextItem.m_file->size();
                request.setRawHeader( "Range", QByteArray( "bytes=" + QByteArray::number( nextItem.m_tempSize ) + "-" ) );
            } else {
                if( nextItem.m_tempExists ){
                    QFile::remove( nextItem.m_tempFile );
                }
                nextItem.m_file = new QFile( nextItem.m_tempFile );
                if( !nextItem.m_file->open( QIODevice::ReadWrite ) ){
                    emit status( nextItem.m_url, "Error", nextItem.m_file->errorString(), nextItem.m_destFile );
                    startNextDownload();
                    return;
                }
                nextItem.m_tempSize = 0;
            }
            QNetworkReply *reply = startDownloadRequest( request );
            emit status( nextItem.m_url, "Download started", "Download started successfully", nextItem.m_destFile );
            nextItem.m_time.start();
            m_downloadHash[reply] = nextItem;
            m_urlHash[ nextItem.m_url ] = reply;

            startNextDownload();
        }
    }

    QNetworkReply* DownloadManager::startDownloadRequest( QNetworkRequest const & request )
    {
        QNetworkReply *reply = m_networkManager.get( request );
        QObject::connect( reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64) ));
        QObject::connect( reply, SIGNAL( finished() ), this, SLOT( downloadFinished() ) );
        QObject::connect( reply, SIGNAL( readyRead() ), this, SLOT( downloadReadyRead() ) );
        QObject::connect( reply, SIGNAL( error(QNetworkReply::NetworkError)), this, SLOT(downloadError(QNetworkReply::NetworkError) ));

        return reply;
    }

    void DownloadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
    {
        QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

        if (reply->error() == QNetworkReply::NoError) {

            Downloads item = m_downloadHash[reply];

            qint64 actualReceived = item.m_tempSize + bytesReceived;
            qint64 actualTotal = item.m_tempSize + bytesTotal;
            double speed = actualReceived * 1000.0 / item.m_time.elapsed();
            QString unit;
            if (speed < 1024) {
                unit = "bytes/sec";
            } else if (speed < 1024*1024) {
                speed /= 1024;
                unit = "kB/s";
            } else {
                speed /= 1024*1024;
                unit = "MB/s";
            }
            int percent = actualReceived * 100 / actualTotal;

            emit progress( item.m_url, actualReceived, actualTotal, percent, speed, unit );
        }
    }

    void DownloadManager::downloadReadyRead()
    {
        QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
        if( !hasRedirect( reply ) ){
            Downloads item = m_downloadHash[reply];
            item.m_file->write( reply->readAll() );
        }
    }

    bool DownloadManager::hasRedirect( QNetworkReply *reply )
    {
        if( reply->error() == QNetworkReply::NoError ){
            QVariant redirect = reply->attribute( QNetworkRequest::RedirectionTargetAttribute );
            if( !redirect.isNull() ){
                QUrl originalUrl = reply->request().url();
                QString newUrl = originalUrl.resolved( redirect.toUrl() ).toString();
                stopDownload( originalUrl.toString() );
                startDownload( newUrl );
                return true;
            } else {
                return false;
            }
        }
        return false;
    }

    void DownloadManager::downloadFinished()
    {
        QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
        if( hasRedirect( reply ) ) return;

        if (reply->error() == QNetworkReply::NoError) {

            Downloads item = m_downloadHash[reply];

            item.m_file->close();
            item.m_file->deleteLater();

            if ( QFile::exists( item.m_destFile))
                QFile::remove( item.m_destFile );
            QFile::rename( item.m_tempFile, item.m_destFile );
            m_completedList.append(item);

            emit status(item.m_url, "Complete", "Download file completed", item.m_url);
            emit finished( item.m_url, item.m_destFile );

            m_downloadHash.remove( reply );
            m_urlHash.remove( item.m_url );

            startNextDownload();
        }

        reply->deleteLater();
    }

    void DownloadManager::downloadError(QNetworkReply::NetworkError)
    {
        QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
        Downloads item = m_downloadHash[reply];

        emit status( item.m_url, "Error", reply->errorString(), item.m_url );
        emit progress( item.m_url, 0, 0, 0, 0, "bytes/sec" );

        // remove download when error
        m_downloadHash.remove(reply);
        m_urlHash.remove(item.m_url);

        startNextDownload();
    }

    QString DownloadManager::saveFilename(const QString &url, bool &exist, QString &fileName, bool &tempExist, bool noFilePathSpecified )
    {
        QFileInfo fileInfo = QFileInfo( url );
        QString   baseName = fileInfo.baseName();
        QString   filenameSuffix = fileInfo.completeSuffix();

        if( baseName.isEmpty() ){
            baseName = QUuid::createUuid();
        }
        if( filenameSuffix.isEmpty() ){
            filenameSuffix = "nil";
        }

        QString filePath = url;
        fileName = fileInfo.fileName();

        if( noFilePathSpecified ){
            filePath = m_filePath + QString( "/%1.%2" ).arg( baseName ).arg( filenameSuffix );
            fileName = baseName + "." + filenameSuffix;
        }

        if( QFile::exists( filePath ) ){
            exist = true;
            if( RenameFile == m_existDownloadPolicy ){
                baseName += "(";
                int i = 0;
                while( QFile::exists( m_filePath + "/" + baseName + "(" + QString::number(i) + ")" + filenameSuffix )){
                    ++i;
                }

                baseName += QString::number( i ) + ")";
                filePath = m_filePath + "/" + baseName + "." + filenameSuffix;
            } else if ( m_existDownloadPolicy == OverwriteFile ){
                QFile::remove( filePath );
            }
        }

        QString filePart = filePath + ".part";
        if( QFile::exists( filePart ) ){
            tempExist = true;
        }
        return filePath;
    }
}
