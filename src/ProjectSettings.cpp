/*
 * Settings.cpp
 *
 *  Created on: Oct 23, 2015
 *      Author: Joshua
 */

#include <bb/data/JsonDataAccess>
#include <QDir>
#include <src/ProjectSettings.hpp>

namespace Purple
{
    ProjectSettings::ProjectSettings( QObject *parent ):
            QObject( parent ),
            m_projectFileHandler( new Purple::ProjectFile )
    {
        setProjectFile( "asset:///project_file.json" );
    }

    ProjectSettings::~ProjectSettings()
    {
        //
    }

    int     ProjectSettings::maxResult()       { return m_projectFileHandler->apiInfo().maxResults(); }

    QString ProjectSettings::youtubeUrl()      { return m_projectFileHandler->apiInfo().youtubeUrl(); }

    QString ProjectSettings::apiKey()          { return m_projectFileHandler->apiInfo().apiKey(); }

    QString ProjectSettings::safeSearch() { return m_projectFileHandler->appSettings().safeSearch(); }

    QString ProjectSettings::thumbnailsQuality() { return m_projectFileHandler->appSettings().thumbnailsQuality(); }

    void ProjectSettings::setAppTheme( QString const & app_theme ){ m_projectFileHandler->appSettings().setAppTheme( app_theme ); }

    QString ProjectSettings::appTheme() { return m_projectFileHandler->appSettings().appTheme(); }

    void ProjectSettings::setSafeSearch( QString const & safe_search )
    {
        if( safe_search != m_projectFileHandler->appSettings().safeSearch() ){
            m_projectFileHandler->appSettings().setSafeSearch( safe_search );
            emit safeSearchChanged( safe_search );
        }
    }


    void ProjectSettings::setThumbnailsQuality( QString const & thumbnails_quality )
    {
        if( thumbnails_quality != m_projectFileHandler->appSettings().thumbnailsQuality() ){
            m_projectFileHandler->appSettings().setThumbnailsQuality( thumbnails_quality );
            emit thumbnailsQualityChanged( thumbnails_quality );
        }
    }

    void ProjectSettings::setProjectFile( QString const & location )
    {
        bb::data::JsonDataAccess jda;
        QVariantMap keyMap = jda.load( location ).toMap();

        if( jda.hasError() ){
            emit errorOccurred( "Unable to load API Key from current location" );
        } else {
            m_projectFileHandler->apiInfo().setApiKey( keyMap["apiKey"].toString() );
            m_projectFileHandler->apiInfo().setMaxResults( keyMap["maxResults"].toInt() );
            m_projectFileHandler->apiInfo().setYoutubeUrl( keyMap["youtube_url"].toString() );
            m_projectFileHandler->appSettings().setAppTheme( keyMap["appTheme"].toString() );
            m_projectFileHandler->appSettings().setSafeSearch( keyMap["safeSearch"].toString() );
            m_projectFileHandler->appSettings().setThumbnailsQuality( keyMap["thumbnailsQuality"].toString() );
        }
    }
} /* namespace Purple */