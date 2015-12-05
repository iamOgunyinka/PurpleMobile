/*
 * ThumbnailSaver.cpp
 *
 *  Created on: Dec 5, 2015
 *      Author: Joshua
 */

#include <src/ThumbnailSaver.hpp>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QDir>
#include <QTextStream>

namespace Purple
{

    ThumbnailSaver::ThumbnailSaver( QObject *parent ): QObject( parent )
    {
    }

    ThumbnailSaver::~ThumbnailSaver()
    {
    }

    QString ThumbnailSaver::thumbnailPath = QDir::currentPath() + "/data/thumbs.png";
    void ThumbnailSaver::fetchImage( QString const & url )
    {
        QUrl request_url( url );
        QNetworkRequest request( request_url );
        QNetworkAccessManager network_manager;
        QNetworkReply *reply = network_manager.get( request );

        QObject::connect( reply, SIGNAL(finished()), this, SLOT( onFinished()) );
    }

    void ThumbnailSaver::onFinished()
    {
        QNetworkReply *reply = qobject_cast<QNetworkReply*>( sender() );
        if( reply->error() == QNetworkReply::NoError ){
            QFile file( ThumbnailSaver::thumbnailPath );
            if( file.open( QIODevice::WriteOnly ) ){
                QTextStream text( &file );
                text << reply->readAll();
                file.close();

                emit imageFetched( ThumbnailSaver::thumbnailPath );
                return;
            }
        }
        emit error( "Unable to fetch thumbnail" );
    }
} /* namespace Purple */
