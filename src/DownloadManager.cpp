/*
 * DownloadManager.cpp
 *
 *  Created on: Oct 29, 2015
 *      Author: Joshua
 */

#include <src/DownloadManager.hpp>
#include <QTimer>

namespace Purple
{

    DownloadManager::DownloadManager( QObject *parent ): DownloadInterface( parent )
    {
        setupDownloadManager();
    }

    DownloadManager::~DownloadManager()
    {
    }

    void DownloadManager::startDownload( QString const & url )
    {
        startDownloadImpl( url );
    }

    void DownloadManager::startDownloadImpl( QString const & url, QString const & path )
    {
        bool fileExist = false;
        bool tempExist = false;
        QString fileName = "";
        QString filePath = saveFilename( path == "" ? url : path, fileExist, fileName, tempExist, path == "" );

        /*
        if (fileExist && m_existPolicy == DownloadInterface::ExistThenCancel) {
            qDebug() << fileName << "exist. Cancel download";
            emit status(url, "Cancel", "File already exist", filePath);
            return;
        }
        */

        Downloads item;
        item.m_url = url;
        item.m_key = fileName;
        item.m_destFolder = filePath;
        item.m_tempFolder = filePath + ".part";
        item.m_tempExists = tempExist;

        if ( m_downloadQueue.isEmpty() )
            QTimer::singleShot(0, this, SLOT(startNextDownload()));

        m_downloadQueue.enqueue(item);
    }

    void DownloadManager::stopDownload( QString const & url )
    {
        stopDownloadImpl( url, false );
    }

    void DownloadManager::stopDownloadImpl( QString const & url, bool pause )
    {

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
        m_filePath = "file:///accounts/1000/shared/downloads/";
    }

    void DownloadManager::startNextDownload()
    {
        if( m_downloadQueue.isEmpty() ) {
            emit emptyDownloadQueue();
            return;
        }

        Downloads nextItem = m_downloadQueue.dequeue();
    }

    QString DownloadManager::saveFilename( QString const & url, bool &fileExists, QString & filename, bool &tempExists, bool isUrl )
    {

    }
} /* namespace Purple */
