/*
 * YTDownloadExtractor.cpp
 *
 *  Created on: Dec 5, 2015
 *      Author: Joshua
 */

#include <src/YTDownloadExtractor.hpp>
#include <QThread>
#include <QDateTime>

namespace Purple
{
    YTDownloadExtractor::YTDownloadExtractor( QObject *parent ): QObject( parent ){}
    YTDownloadExtractor::~YTDownloadExtractor(){}

    QString YTDownloadExtractor::time(){ return m_timeFrame; }
    QString YTDownloadExtractor::title(){ return m_title; }

    void YTDownloadExtractor::getDownloadInfo( QString const & url )
    {
        QThread *new_thread = new QThread;
        UrlFinder *urlFinder = new UrlFinder( url );
        urlFinder->moveToThread( new_thread );

        QObject::connect( new_thread, SIGNAL( started()), urlFinder, SLOT( startUrlExtraction() ) );
        QObject::connect( urlFinder, SIGNAL(finished()), this, SLOT( onFinished() ) );
        QObject::connect( urlFinder, SIGNAL(finished()), new_thread, SLOT(quit()));
        QObject::connect( new_thread, SIGNAL( finished()), new_thread, SLOT( deleteLater() ) );

        new_thread->start();
    }

    void YTDownloadExtractor::downloadVideoWithER( QString const & er )
    {
        for( int i = 0; i != m_downloadStreams.size(); ++i ){
            QString new_er = m_downloadStreams[i].extension() + "( " + m_downloadStreams[i].resolution() + " )";
            if( new_er == er ){
                emit url( m_downloadStreams[i].rawUrl() );
                break;
            }
        }
    }

    void YTDownloadExtractor::onFinished()
    {
        UrlFinder *urlFinder = qobject_cast<UrlFinder*>( sender() );
        if( urlFinder == NULL ) return;

        m_title = urlFinder->getTitle();
        m_downloadStreams = urlFinder->getAllStreams();
        m_timeFrame = QString( "Video Length: %1" ).arg(
                    QDateTime::fromTime_t( urlFinder->getVideoStreamLength() ).toUTC().toString("hh:mm:ss")
                    );

        QStringList list;
        for( int i = 0; i != m_downloadStreams.size(); ++i ){
            list.append( m_downloadStreams[i].extension() + "( " + m_downloadStreams[i].resolution() + " )" );
        }
        emit finished( list );
    }
}
