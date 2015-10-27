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
        settings();
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

    void ProjectSettings::settings()
    {
        bb::data::JsonDataAccess jda;
        QVariant keyMap = jda.load( "asset:///project_file.json" );

        if( jda.hasError() ){
            emit errorOccurred( "Unable to load API Key from current location" );
        } else {
            QVariantMap api_info = keyMap.toMap()["api_info"].toMap();
            m_projectFileHandler->apiInfo().setApiKey( api_info["apiKey"].toString() );
            m_projectFileHandler->apiInfo().setMaxResults( api_info["maxResults"].toInt() );
            m_projectFileHandler->apiInfo().setYoutubeUrl( api_info["youtube_url"].toString() );

            QVariantMap app_settings = keyMap.toMap()["settings"].toMap();
            m_projectFileHandler->appSettings().setAppTheme( app_settings["appTheme"].toString() );
            m_projectFileHandler->appSettings().setSafeSearch( app_settings["safeSearch"].toString() );
            m_projectFileHandler->appSettings().setThumbnailsQuality( app_settings["thumbnailsQuality"].toString() );
        }
    }
} /* namespace Purple */
