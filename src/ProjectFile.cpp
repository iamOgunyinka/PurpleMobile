/*
 * ProjectFile.cpp
 *
 *  Created on: Oct 22, 2015
 *      Author: Joshua
 */

#include <src/ProjectFile.hpp>

namespace Purple
{
    ApiInfo::ApiInfo( QString const & api_key, QString const & youtube_url, int const & max_results ):
        m_apiKey( api_key ), m_youtubeUrl( youtube_url ), m_maxResults( max_results )
    {

    }
    QString ApiInfo::youtubeUrl() { return m_youtubeUrl; }
    QString ApiInfo::apiKey() { return m_apiKey; }
    int     ApiInfo::maxResults() { return m_maxResults; }

    void ApiInfo::setYoutubeUrl( QString const & youtube_url ) { m_youtubeUrl = youtube_url; }
    void ApiInfo::setApiKey( QString const & api_key ) { m_apiKey = api_key; }
    void ApiInfo::setMaxResults( int max_results ) { m_maxResults = max_results; }

    AppSettings::AppSettings( QString const & app_theme, QString const & safe_search, QString const & thumbnailsQuality ):
            m_appTheme( app_theme ), m_safeSearch( safe_search ), m_thumbnailsQuality( thumbnailsQuality )
    {

    }

    void AppSettings::setSafeSearch( QString const & safe_search ) { m_safeSearch = safe_search; }
    QString AppSettings::safeSearch() const { return m_safeSearch; }
    void AppSettings::setThumbnailsQuality( QString const & quality ) { m_thumbnailsQuality = quality; }
    QString AppSettings::thumbnailsQuality() const { return m_thumbnailsQuality;}
    void AppSettings::setAppTheme( QString const & app_theme ) { m_appTheme = app_theme; }
    QString AppSettings::appTheme() const { return m_appTheme; }

    ProjectFile::ProjectFile():
            m_apiInfo(), m_appSettings()
    {
    }

    ProjectFile::~ProjectFile()
    {
    }

    AppSettings&   ProjectFile::appSettings() { return m_appSettings; }
    ApiInfo&       ProjectFile::apiInfo() { return m_apiInfo; }
} /* namespace Purple */
