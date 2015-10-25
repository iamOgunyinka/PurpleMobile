#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include <QObject>
#include "ProjectFile.hpp"

namespace data
{
    class JsonDataAccess;
}

namespace Purple
{
    class Settings: public QObject
    {
        Q_OBJECT

        Q_PROPERTY( QString apiKey READ apiKey )
        Q_PROPERTY( QString youtubeUrl READ youtubeUrl )
        Q_PROPERTY( int maxResult READ maxResult )
        Q_PROPERTY( QString appTheme READ appTheme WRITE setAppTheme )
        Q_PROPERTY( QString safeSearch READ safeSearch WRITE setSafeSearch NOTIFY safeSearchChanged )
        Q_PROPERTY( QString thumbnailsQuality READ thumbnailsQuality WRITE setThumbnailsQuality NOTIFY thumbnailsQualityChanged )
    public:
        Settings( QObject *parent = 0 );
        virtual ~Settings();

        QString apiKey();
        QString youtubeUrl();
        int maxResult();

        QString appTheme();
        void setAppTheme( QString const & theme );

        QString safeSearch();
        void setSafeSearch( QString const & safe_search );

        QString thumbnailsQuality();
        void setThumbnailsQuality( QString const & thumbnails_quality );
    signals:
        void errorOccurred( QString const & error );
        void thumbnailsQualityChanged( QString const & newThumbnailsQuality );
        void safeSearchChanged( QString const & newSafeSearch );
    private:
        Purple::ProjectFile         *m_projectFileHandler;
        void    settings();
    };

} /* namespace Purple */

#endif /* SETTINGS_HPP_ */
