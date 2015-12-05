/*
 * YTDownloadExtractor.hpp
 *
 *  Created on: Dec 5, 2015
 *      Author: Joshua
 */

#ifndef YTDOWNLOADEXTRACTOR_HPP_
#define YTDOWNLOADEXTRACTOR_HPP_

#include "Stream.hpp"
namespace Purple
{
    class YTDownloadExtractor: public QObject
    {
        Q_OBJECT

        Q_PROPERTY( QList<Stream> streams READ streams CONSTANT )
        Q_PROPERTY( QString title READ title  )
    public:
        YTDownloadExtractor( QString const & url, QObject *parent = NULL );
        virtual ~YTDownloadExtractor();

        QList<Stream> streams() const;
        QString         title() const;
    private:
        QList<Stream> m_downloadStreams;
        QString       m_title;
        UrlFinder    *m_urlFinder;

    signals:
        void error( QString const & what );
        void status( QString const & message );
    };
} //Purple
#endif /* YTDOWNLOADEXTRACTOR_HPP_ */
