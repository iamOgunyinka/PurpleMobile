/*
 * DownloadsDataModel.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: Joshua
 */

#include <src/DownloadsDataModel.hpp>
#include "MyIndexMapper.hpp"
//#include <c++/4.6.3/thread>
//#include <c++/4.6.3/condition_variable>

namespace Purple
{

    DownloadsDataModel::DownloadsDataModel( QObject *parent ): bb::cascades::DataModel( parent ),
            m_downloadManager( new DownloadManager( this ) )
    {
        QObject::connect( m_downloadManager, SIGNAL(status( QString, QString, QString, QString )),
                this, SLOT(onStatus( QString, QString, QString, QString )) );
        QObject::connect( m_downloadManager, SIGNAL( newDownloadAdded() ), this, SLOT( onNewDownloadAdded() ) );
        QObject::connect( m_downloadManager, SIGNAL( finished(QString, QString) ), this, SLOT( onFinished(QString, QString) ) ) ;
        QObject::connect( m_downloadManager, SIGNAL( progress(QString, qint64, qint64, int, double, QString )),
                this, SLOT( onNewDownloadAdded() ) );
    }

    DownloadsDataModel::~DownloadsDataModel()
    {
        QFile file( DownloadManager::downloads_path );
        if( file.open( QIODevice::ReadOnly ) ){
            QTextStream text( &file );
            qDebug() << text.readAll();
            file.close();
        }
    }

    void DownloadsDataModel::onNewDownloadAdded()
    {
        load( DownloadManager::downloads_path );
    }

    void DownloadsDataModel::startDownload( QString const & url )
    {
//        std::thread foreign_thread( startDownload, m_downloadManager, url );
        m_downloadManager->startDownload( url );
    }

    void DownloadsDataModel::stopDownload( QString const & url )
    {
        m_downloadManager->stopDownload( url );
    }

    void DownloadsDataModel::resumeDownload( QString const & url, QString const & path )
    {
        m_downloadManager->resumeDownload( url, path );
    }

    void DownloadsDataModel::pauseDownload( QString const & url )
    {
        m_downloadManager->pauseDownload( url );
    }

    void DownloadsDataModel::onStatus( QString const & url, QString const & title, QString const & message, QString const & )
    {
        if( title == "Error" || title == "Cancel" ){
            emit error( message );
        } else {
            emit status( url, message );
        }
    }

    void DownloadsDataModel::onFinished( QString const& url, QString const & )
    {
        emit status( "Completed", url );
    }

    void DownloadsDataModel::load( QString const & sourceFile )
    {
        bb::data::JsonDataAccess jda;
        QVariantList list = jda.load( sourceFile ).toList();

        if( jda.hasError() ){
            emit error( jda.error().errorMessage() );
            return;
        }
        qDebug() << "Loaded new file: " << sourceFile;
        for( int i = 0; i != list.size(); ++i ){
            m_downloadList.append( list[i].toMap() );
        }
        emit itemsChanged( bb::cascades::DataModelChangeType::AddRemove, QSharedPointer<bb::cascades::DataModel::IndexMapper>(
                new MyIndexMapper( m_downloadList.size(), m_downloadList.size(), false )));
    }

    bool DownloadsDataModel::hasChildren( QVariantList const & indexPath )
    {
        if( indexPath.size() == 0 || indexPath.size() == 1 ){
            return true;
        }
        return false;
    }

    int DownloadsDataModel::childCount( QVariantList const & indexPath )
    {
        if( indexPath.size() == 0 ){ //root hierarchy, all downloads
            return 1;
        } else {
            if( indexPath.size() == 1 ){
                if( indexPath.at( 0 ).toInt() == 0 ){
                    return m_downloadList.size();
                }
            } else {
                return 0;
            }
        }
        return 0;
    }

    QVariant DownloadsDataModel::data( QVariantList const & indexPath )
    {
        if( indexPath.size() == 1 ){
            if( indexPath.at( 0 ).toInt() == 0 ){
                QVariantMap header;
                header["value"] = "All Downloads";
                return header;
            }
        } else if( indexPath.size() == 2 ) {
            return m_downloadList.at( indexPath.at( 1 ).toInt() );
        }
        return QVariant();
    }

    QVariantList DownloadsDataModel::downloadsList() { return m_downloadList; }

    void DownloadsDataModel::removeItem( QVariantList const & indexPath )
    {
        if( indexPath.size() == 2 ){
            m_downloadList.removeAt( indexPath.at(1).toInt() );
            emit itemRemoved( indexPath );
        }
    }
} /* namespace Purple */
