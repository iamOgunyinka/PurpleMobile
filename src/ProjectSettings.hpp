#ifndef PROJECT_SETTINGS_HPP_
#define PROJECT_SETTINGS_HPP_

#include "ProjectFile.hpp"
#include <QString>

namespace data
{
    class JsonDataAccess;
}

namespace Purple
{
    class ProjectSettings: public QObject
    {
        Q_OBJECT

        Q_PROPERTY( QString apiKey READ apiKey )
        Q_PROPERTY( QString youtubeUrl READ youtubeUrl )
        Q_PROPERTY( int maxResult READ maxResult )
        Q_PROPERTY( QString appTheme READ appTheme WRITE setAppTheme )
        Q_PROPERTY( QString safeSearch READ safeSearch WRITE setSafeSearch NOTIFY safeSearchChanged )
        Q_PROPERTY( QString thumbnailsQuality READ thumbnailsQuality WRITE setThumbnailsQuality NOTIFY thumbnailsQualityChanged )
    public:
        ProjectSettings( QObject *parent = 0 );
        virtual ~ProjectSettings();

        QString apiKey();
        QString youtubeUrl();
        int maxResult();

        Q_INVOKABLE QString appTheme();
        Q_INVOKABLE void setAppTheme( QString const & theme );

        Q_INVOKABLE QString safeSearch();
        Q_INVOKABLE void setSafeSearch( QString const & safe_search );

        Q_INVOKABLE QString thumbnailsQuality();
        Q_INVOKABLE void setThumbnailsQuality( QString const & thumbnails_quality );
    signals:
        void errorOccurred( QString const & error );
        void thumbnailsQualityChanged( QString const & newThumbnailsQuality );
        void safeSearchChanged( QString const & newSafeSearch );
    private:
        Purple::ProjectFile         *m_projectFileHandler;
        void    settings();
    };

} /* namespace Purple */

#endif /* PROJECT_SETTINGS_HPP_ */
