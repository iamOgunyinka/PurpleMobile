#ifndef YT_MANAGER_HPP_
#define YT_MANAGER_HPP_

#include <QSharedPointer>
#include <bb/cascades/DataModel>
#include "SyncNetworkManager.hpp"
#include "ProjectFile.hpp"
#include "MyIndexMapper.hpp"

namespace Purple
{
    class YTManager: public QObject
    {
        Q_OBJECT

    public:
        YTManager( QObject *parent = 0 );
        virtual ~YTManager();
        QSharedPointer<ProjectFile> projectHandler() { return m_projectFileHandler; }
        void search( QString const & searchString );
        void setProjectFile( QString const & location );

    private:
        SyncNetworkManager*                m_networkManager;
        QSharedPointer<ProjectFile>        m_projectFileHandler;
    private:
        void saveSettings();

    private slots:
        void onError( QString const & err );
        void onFinished( QString const & data );
    signals:
        void error( QString const & what );
        void finished( QString const & data );
        void sourceChanged( QString const & );
    };

    class YTDataManager: public bb::cascades::DataModel
    {
        Q_OBJECT
        Q_PROPERTY( int maxResult READ maxResult WRITE setMaxResult )
        Q_PROPERTY( QString appTheme READ appTheme WRITE setAppTheme )
        Q_PROPERTY( QString safeSearch READ safeSearch WRITE setSafeSearch )
        Q_PROPERTY( QString fileExistPolicy READ fileExistPolicy WRITE setFileExistPolicy )
        Q_PROPERTY( QString thumbnailsQuality READ thumbnailsQuality WRITE setThumbnailsQuality )

    public:
        YTDataManager( QObject* parent = 0 );
        ~YTDataManager();

        Q_INVOKABLE void search( QString const & searchString );
        Q_INVOKABLE void setProjectFile( QString const & location );

    private:
        int maxResult();
        void setMaxResult( int value );

        QString appTheme();
        void setAppTheme( QString const & theme );

        QString safeSearch();
        void setSafeSearch( QString const & safe_search );

        QString fileExistPolicy();
        void    setFileExistPolicy( QString const & filePolicy );

        QString thumbnailsQuality();
        void setThumbnailsQuality( QString const & thumbnails_quality );

    public slots:
        Q_INVOKABLE bool hasChildren( QVariantList const & indexPath );
        Q_INVOKABLE int childCount( QVariantList const & indexPath );
        Q_INVOKABLE QVariant data( QVariantList const & indexPath );
        Q_INVOKABLE void removeItem( QVariantList const & indexPath );

    private slots:
        void load( QString const & data );
        void onFinished( QString const & data );
        void onError( QString const & message );
    private:
        YTManager    *m_ytManager;
        QVariantList  m_videoResultList;
        QString       m_data;
    signals:
        void error( QString const & message );
    };
} /* namespace Purple */

#endif /* YT_MANAGER_HPP_ */
