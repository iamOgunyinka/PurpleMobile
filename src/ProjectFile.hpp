#ifndef PROJECTFILE_HPP_
#define PROJECTFILE_HPP_

#include <QString>

namespace Purple
{
    class ApiInfo
    {
        QString         m_apiKey;
        QString         m_youtubeUrl;
        int             m_maxResults;
    public:
        ApiInfo( QString const & api_key = QString(), QString const & youtube_url = QString(), int const & max_results = 40 );
        QString         youtubeUrl();
        QString         apiKey();
        int             maxResults();

        void setYoutubeUrl( QString const & youtube_url );
        void setApiKey( QString const & api_key );
        void setMaxResults( int max_results );
    };

    struct AppSettings
    {
        QString         m_appTheme;
        QString         m_safeSearch;
        QString         m_thumbnailsQuality;
        QString         m_fileExistencePolicy;

        AppSettings( QString const & app_theme = QString(), QString const & safe_search = QString(), QString const & thumbnailsQuality = QString() );

        void setSafeSearch( QString const & safe_search );
        QString safeSearch() const;
        void setThumbnailsQuality( QString const & quality );
        QString thumbnailsQuality() const;
        void setAppTheme( QString const & app_theme );
        QString appTheme() const;
        void setFileExistencePolicy( QString const & f );
        QString fileExistencePolicy();
    };

    class ProjectFile
    {
    public:
        ProjectFile();
        virtual ~ProjectFile();

        AppSettings&        appSettings();
        ApiInfo&            apiInfo();
    private:
        ApiInfo             m_apiInfo;
        AppSettings         m_appSettings;
    };

} /* namespace Purple */

#endif /* PROJECTFILE_HPP_ */
