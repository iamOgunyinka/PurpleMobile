/*
 *
 *  Created on: Oct 25, 2015
 *      Author: Joshua
 */

#include <src/YTManager.hpp>
#include <QFile>
#include <bb/data/JsonDataAccess>

namespace Purple
{
    YTManager::YTManager( QObject *parent ): QObject( parent ),
            m_networkManager( new SyncNetworkManager( this ) ),
            m_projectFileHandler( new ProjectFile )
    {
        QObject::connect( m_networkManager, SIGNAL(networkError(QString)), this, SLOT( onError(QString) ) );
        QObject::connect( m_networkManager, SIGNAL(errorOccurred(QString)), this, SLOT( onError(QString) ) );
        QObject::connect( m_networkManager, SIGNAL(finished(QString)), this, SLOT( onFinished(QString) ) );
    }

    YTManager::~YTManager()
    {
        saveSettings();
    }

    void YTManager::onFinished( QString const & data )
    {
        emit finished( data );
    }

    void YTManager::onError( QString const & err )
    {
        emit error( err );
    }

    void YTManager::saveSettings()
    {
        QFile textFile( "app/data/assets/settings.json" );
        if( textFile.exists() )
        {
            textFile.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate );
            QTextStream out( &textFile );

            out << "[\n" << "{\n"
                    << "\"apiKey\": \"AIzaSyBhl_zBnEEv_xiIukkMpz8ayoiwT1UdfQk\",\n"
                    << "\"maxResult\": " << m_projectFileHandler->apiInfo().maxResults() << ",\n"
                    << "\"youtube_url\": \"https://www.googleapis.com/youtube/v3/search/?part=snippet\",\n"
                    << "\"appTheme\": \"" << m_projectFileHandler->appSettings().appTheme().toLower() << "\",\n"
                    << "\"safeSearch\": \"" << m_projectFileHandler->appSettings().safeSearch().toLower() << "\",\n"
                    << "\"thumbnailsQuality\": \"" << m_projectFileHandler->appSettings().thumbnailsQuality().toLower() << "\",\n"
                    << "\"existsAction\": \"" << m_projectFileHandler->appSettings().fileExistencePolicy() << "\"\n"
                    << "}\n" << "]\n";
            textFile.close();
            textFile.open( QIODevice::ReadOnly );
            QTextStream in( &textFile );
            qDebug() << in.readAll();
            textFile.close();
        }
    }

    void YTManager::search( QString const & searchString )
    {
        QString youtube_query = m_projectFileHandler->apiInfo().youtubeUrl();
        youtube_query += "&q=" + QUrl::toPercentEncoding( searchString );
        youtube_query += "&maxResults=" + QString::number( m_projectFileHandler->apiInfo().maxResults() );
        youtube_query += "&key=" + m_projectFileHandler->apiInfo().apiKey();

        qDebug() << "Youtube query is " << youtube_query;
        m_networkManager->sendRequest( youtube_query );
    }

    int     YTManager::maxResult()  { return m_projectFileHandler->apiInfo().maxResults(); }

    QString YTManager::safeSearch() { return m_projectFileHandler->appSettings().safeSearch(); }

    QString YTManager::thumbnailsQuality() { return m_projectFileHandler->appSettings().thumbnailsQuality(); }

    void YTManager::setMaxResult( int value ) { m_projectFileHandler->apiInfo().setMaxResults( value ); }

    void YTManager::setAppTheme( QString const & app_theme ){ m_projectFileHandler->appSettings().setAppTheme( app_theme ); }

    QString YTManager::appTheme() { return m_projectFileHandler->appSettings().appTheme(); }

    void YTManager::setSafeSearch( QString const & safe_search )
    {
        if( safe_search != m_projectFileHandler->appSettings().safeSearch() ){
            m_projectFileHandler->appSettings().setSafeSearch( safe_search );
        }
    }

    void YTManager::setFileExistPolicy( QString const & filePolicy ) { m_projectFileHandler->appSettings().setFileExistencePolicy( filePolicy ); }
    QString YTManager::fileExistPolicy() { return m_projectFileHandler->appSettings().fileExistencePolicy(); }

    void YTManager::setThumbnailsQuality( QString const & thumbnails_quality )
    {
        if( thumbnails_quality != m_projectFileHandler->appSettings().thumbnailsQuality() ){
            m_projectFileHandler->appSettings().setThumbnailsQuality( thumbnails_quality );
        }
    }

    void YTManager::setProjectFile( QString const & location )
    {
        bb::data::JsonDataAccess jda;

        QVariantList keyList = jda.load( location ).toList();

        if( jda.hasError() ){
            emit error( jda.error().errorMessage() );
        } else {
            QVariantMap keyMap = keyList[0].toMap();
            m_projectFileHandler->apiInfo().setApiKey( keyMap["apiKey"].toString() );
            m_projectFileHandler->apiInfo().setMaxResults( keyMap["maxResult"].toInt() );
            m_projectFileHandler->apiInfo().setYoutubeUrl( keyMap["youtube_url"].toString() );
            m_projectFileHandler->appSettings().setAppTheme( keyMap["appTheme"].toString() );
            m_projectFileHandler->appSettings().setSafeSearch( keyMap["safeSearch"].toString() );
            m_projectFileHandler->appSettings().setThumbnailsQuality( keyMap["thumbnailsQuality"].toString() );
        }
    }

    YTDataManager::YTDataManager( QObject *parent ): bb::cascades::DataModel( parent )
    {
    }

    YTDataManager::~YTDataManager(){}

    void YTDataManager::setSource( QString const & buffer )
    {
        qDebug() << buffer;
        bb::data::JsonDataAccess jda;
        QVariantMap rootMap = jda.loadFromBuffer( buffer ).toMap();
        if( rootMap["kind"].toString() == QString( "error" ) ){
            emit error( "Unable to process request" );
        } else {
            QVariantList results = rootMap["items"].toList();
            for( int i = 0; i != results.size(); ++i ){
                QVariantMap eachItem = results[i].toMap();
                QVariantMap item;
                item["id"] = eachItem["id"].toMap()["videoId"].toString();
                item["title"] = eachItem["title"].toString();
                item["thumbnails"] = eachItem["thumbnails"].toMap()
                                                             [ "default" ].toMap()
                                                             ["url"].toString();
                item["owner"] = eachItem["channelTitle"].toString();
                m_videoResultList.append( item );
            }
        }
        emit sourceChanged( source() );
    }

    QString YTDataManager::source(){ return QString(); }
    QVariantList YTDataManager::videoResultList(){ return m_videoResultList; }

    bool YTDataManager::hasChildren( QVariantList const & indexPath )
    {
        if( indexPath.size() == 0 || indexPath.size() == 1 ){
            return true;
        }
        return false;
    }

    int YTDataManager::childCount( QVariantList const & indexPath )
    {
        if( indexPath.size() == 0 ){
            return 1;
        } else {
            if( indexPath.size() == 1 ){
                return m_videoResultList.size();
            }
        }
        return 0;
    }

    void YTDataManager::removeItem( QVariantList const & indexPath )
    {
        Q_UNUSED(indexPath);
    }

    QString YTDataManager::itemType( QVariantList const & )
    {
        return QString();
    }

    QVariant YTDataManager::data( QVariantList const & indexPath )
    {
        if( indexPath.size() == 1 ){
            if( indexPath.at(0).toInt() == 0 ){
                QVariantMap header;
                header["value"] = "Result";
                return header;
            }
        } else if( indexPath.size() == 2 ) {
            return m_videoResultList.at( indexPath.at( 1 ).toInt() );
        }
        return QVariant();
    }
} /* namespace Purple */
