/*
 * YTDownloadExtractor.cpp
 *
 *  Created on: Dec 5, 2015
 *      Author: Joshua
 */

#include <src/YTDownloadExtractor.hpp>

namespace Purple
{
    YTDownloadExtractor::YTDownloadExtractor( QString const & url, QObject *parent ): QObject( parent ),
            m_title(), m_urlFinder( new UrlFinder( url ))
    {
        try {
            m_urlFinder->startUrlExtraction();
        } catch ( QString const & e) {
            emit error( e );
        }
        emit status( "Success" );
    }

    YTDownloadExtractor::~YTDownloadExtractor(){}

    QList<Stream> YTDownloadExtractor::streams() const { return m_downloadStreams; }
    QString YTDownloadExtractor::title() const{ return m_urlFinder->getTitle(); }
}
