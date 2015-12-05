/*
 * YTDownloadExtractor.hpp
 *
 *  Created on: Dec 5, 2015
 *      Author: Joshua
 */

#ifndef YTDOWNLOADEXTRACTOR_HPP_
#define YTDOWNLOADEXTRACTOR_HPP_

#include "Stream.hpp"
#include <QStringList>

namespace Purple
{
    class YTDownloadExtractor: public QObject
    {
        Q_OBJECT

        Q_PROPERTY( QString title READ title )
        Q_PROPERTY( QString time READ time )
    public:
        YTDownloadExtractor( QObject *parent = NULL );
        virtual ~YTDownloadExtractor();

        QString         title();
        QString         time();
    private:
        QList<Stream> m_downloadStreams;
        QString       m_title;
        QString       m_timeFrame;
    signals:
//        void error( QString const & what );
        void url( QString const & videoUrl );
        void finished( QStringList const & streams );
    public slots:
        Q_INVOKABLE void getDownloadInfo( QString const & url );
        Q_INVOKABLE void downloadVideoWithER( QString const & url );
        void onFinished( );
    };
} //Purple
#endif /* YTDOWNLOADEXTRACTOR_HPP_ */
