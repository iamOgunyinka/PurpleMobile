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
                    << "\"appTheme\": \"" << m_projectFileHandler->appSettings().appTheme() << "\",\n"
                    << "\"safeSearch\": \"" << m_projectFileHandler->appSettings().safeSearch() << "\",\n"
                    << "\"thumbnailsQuality\": \"" << m_projectFileHandler->appSettings().thumbnailsQuality() << "\",\n"
                    << "\"existsAction\": \"" << m_projectFileHandler->appSettings().fileExistencePolicy() << "\"\n"
                    << "}\n" << "]\n";
            textFile.close();
        }
    }

    void YTManager::search( QString const & searchString )
    {
        QString youtube_query = m_projectFileHandler->apiInfo().youtubeUrl();
        youtube_query += "&q=" + QUrl::toPercentEncoding( searchString );
        youtube_query += "&maxResults=" + QString::number( m_projectFileHandler->apiInfo().maxResults() );
        youtube_query += "&key=" + m_projectFileHandler->apiInfo().apiKey();

        m_networkManager->sendRequest( youtube_query );
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

    MyIndexMapper::MyIndexMapper( int index, int count, bool deleted ):
            m_index( index ), m_count( count ), m_deleted( deleted )
    {
    }

    bool MyIndexMapper::newIndexPath( QVariantList *pNewIndexPath, int *replacementIndex, QVariantList const & oldIndexPath ) const
    {
        if (oldIndexPath[0].toInt() < m_index) {
            pNewIndexPath->append(oldIndexPath);
            return true;

        // Deleted indices
        } else if (m_deleted && oldIndexPath[0].toInt() <= m_index + m_count) {
            *replacementIndex = m_index;
            return false;

        // Indices after a deletion or addition
        } else {
            if (m_deleted)
                pNewIndexPath->append(oldIndexPath[0].toInt() - m_count);
            else
                pNewIndexPath->append(oldIndexPath[0].toInt() + m_count);
            return true;
        }
    }

    YTDataManager::YTDataManager( QObject *parent ): bb::cascades::DataModel( parent ), m_ytManager( new YTManager( this ))
    {
        QObject::connect( m_ytManager, SIGNAL( error(QString)), this, SLOT(onError(QString)));
        QObject::connect( m_ytManager, SIGNAL( finished(QString)), this, SLOT(onFinished( QString )));
    }

    YTDataManager::~YTDataManager(){}
    void YTDataManager::search( QString const & searchString )
    {
        m_ytManager->search( searchString );
    }

    void YTDataManager::setProjectFile( QString const & location )
    {
        m_ytManager->setProjectFile( location );
    }

    void YTDataManager::onError( QString const & message )
    {
        emit error( message );
    }

    void YTDataManager::onFinished( QString const & data )
    {
        if( data != m_data )
        {
            m_data = data;
            load( m_data );
        }
    }

    void YTDataManager::load( QString const & buffer )
    {
        m_videoResultList.clear();

        QVariantList indexPath;
        indexPath.append( m_videoResultList.size() );
        emit itemRemoved( indexPath );

        bb::data::JsonDataAccess jda;
        QVariantMap rootMap = jda.loadFromBuffer( buffer ).toMap();

        if( jda.hasError() ){
            emit error( jda.error().errorMessage() );
            return;
        }

        if( rootMap["kind"].toString() == QString( "error" ) ){
            emit error( "Unable to process request" );
        } else {
            QVariantList results = rootMap["items"].toList();
            for( int i = 0; i != results.size(); ++i ){
                QVariantMap eachItem = results[i].toMap();
                QVariantMap item;

                item["id"] = eachItem["id"].toMap()["videoId"].toString();

                if( item["id"] == QString("") ){ //we got a playlist here
                    item["id"] = eachItem["id"].toMap()["playlist"].toString();
                }
                QVariantMap map = eachItem["snippet"].toMap();
                item["title"] = map["title"].toString();
                item["owner"] = map["channelTitle"].toString();
                item["details"] = map["description"].toString();
                item["thumbnails"] = map["thumbnails"].toMap()[ "default" ].toMap()["url"].toString();
                m_videoResultList.append( item );
            }
        }

        emit itemsChanged( bb::cascades::DataModelChangeType::AddRemove, QSharedPointer<bb::cascades::DataModel::IndexMapper>(
                new MyIndexMapper( m_videoResultList.size(), m_videoResultList.size(), false )));
    }

    int     YTDataManager::maxResult()  { return m_ytManager->projectHandler()->apiInfo().maxResults(); }

    QString YTDataManager::safeSearch() { return m_ytManager->projectHandler()->appSettings().safeSearch(); }

    QString YTDataManager::thumbnailsQuality() { return m_ytManager->projectHandler()->appSettings().thumbnailsQuality(); }

    void YTDataManager::setMaxResult( int value ) { m_ytManager->projectHandler()->apiInfo().setMaxResults( value ); }

    void YTDataManager::setAppTheme( QString const & app_theme ){ m_ytManager->projectHandler()->appSettings().setAppTheme( app_theme ); }

    QString YTDataManager::appTheme() { return m_ytManager->projectHandler()->appSettings().appTheme(); }

    void YTDataManager::setSafeSearch( QString const & safe_search )
    {
        if( safe_search != m_ytManager->projectHandler()->appSettings().safeSearch() ){
            m_ytManager->projectHandler()->appSettings().setSafeSearch( safe_search );
        }
    }

    void YTDataManager::setFileExistPolicy( QString const & filePolicy )
    {
        m_ytManager->projectHandler()->appSettings().setFileExistencePolicy( filePolicy );
    }

    QString YTDataManager::fileExistPolicy() { return m_ytManager->projectHandler()->appSettings().fileExistencePolicy(); }

    void YTDataManager::setThumbnailsQuality( QString const & thumbnails_quality )
    {
        if( thumbnails_quality != m_ytManager->projectHandler()->appSettings().thumbnailsQuality() ){
            m_ytManager->projectHandler()->appSettings().setThumbnailsQuality( thumbnails_quality );
        }
    }


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
        if( indexPath.size() == 2 ){
            m_videoResultList.removeAt( indexPath.at(1).toInt() );
            emit itemRemoved( indexPath );
        }
    }

    QVariant YTDataManager::data( QVariantList const & indexPath )
    {
        if( indexPath.size() == 2 ) {
            return m_videoResultList.at( indexPath.at( 1 ).toInt() );
        }
        return QVariant();
    }
} /* namespace Purple */
