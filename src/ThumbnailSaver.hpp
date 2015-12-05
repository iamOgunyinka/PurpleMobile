/*
 * ThumbnailSaver.hpp
 *
 *  Created on: Dec 5, 2015
 *      Author: Joshua
 */

#ifndef THUMBNAILSAVER_HPP_
#define THUMBNAILSAVER_HPP_

#include <QObject>
namespace Purple
{

    class ThumbnailSaver: public QObject
    {
        Q_OBJECT
    public:

        static QString thumbnailPath;
        ThumbnailSaver( QObject *parent = NULL );
        virtual ~ThumbnailSaver();
        Q_INVOKABLE void fetchImage( QString const & url );

    private slots:
        void onFinished();
    signals:
        void imageFetched( QString const & location );
        void error( QString const & what );
    };

} /* namespace Purple */

#endif /* THUMBNAILSAVER_HPP_ */
