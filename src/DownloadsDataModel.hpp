/*
 * DownloadsDataModel.hpp
 *
 *  Created on: Oct 31, 2015
 *      Author: Joshua
 */

#ifndef DOWNLOADSDATAMODEL_HPP_
#define DOWNLOADSDATAMODEL_HPP_

#include <bb/cascades/DataModel>
#include <bb/data/JsonDataAccess>
#include <QStringList>
#include "DownloadManager.hpp"

namespace Purple
{
    class DownloadsDataModel: public bb::cascades::DataModel
    {
        Q_OBJECT

    public:
        DownloadsDataModel( QObject *parent = 0 );
        virtual ~DownloadsDataModel();

        Q_INVOKABLE void            startDownload( QString const & url );
        Q_INVOKABLE void            stopDownload( QString const & url );
        Q_INVOKABLE void            pauseDownload( QString const & url );
        Q_INVOKABLE void            resumeDownload( QString const & url, QString const & path = QString() );

    private:
        void                    load( QString const & filename );

        DownloadManager        *m_downloadManager;
        QVariantList            m_downloadList;
//        QString                 m_url;
    public slots:
        QVariantList            downloadsList();
        Q_INVOKABLE int         childCount( QVariantList const & indexPath );
        Q_INVOKABLE bool        hasChildren( QVariantList const & indexPath );
        Q_INVOKABLE QVariant    data( QVariantList const & indexPath );
        Q_INVOKABLE void        removeItem( QVariantList const & indexPath );

        void onStatus( QString const & url, QString const & title, QString const &message, QString const &data );
        void onFinished( QString const & url, QString const & destination );
        void onNewDownloadAdded();

    signals:
        void    error( QString const & errorMessage );
        void    status( QString const & url, QString const & message );
    };
} /* namespace Purple */

#endif /* DOWNLOADSDATAMODEL_HPP_ */
