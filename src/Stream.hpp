/*
 * UrlExtractor.hpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua
 */

#ifndef STREAM_HPP_
#define STREAM_HPP_

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Purple
{
    struct Mapping
    {
        typedef QString StringType;

        StringType dimension;
        StringType type;
        StringType quality;
        StringType additional_info;
    };

    class Constants
    {
    public:
        static int         lifespan;
        static QString     user_agent;
        static QString     uefsm;
        static QString     af;
        static QString     jsplayer;
        static QMap<QString, Mapping> itags;
        static QVariantMap urls;

        static void init();
    };

    namespace HelperFunctions
    {
        QByteArray      fetchDecode( QString const & url );
        QVariantMap     parseQueryString( QString const & data );
        QString         extractVideoID( QString const & url );
        QVariantList    extractSmap( QString const & key, QVariantMap const & data );
        QVariantList    extractDash( QString const & dashUrl );
        QVariantMap     getVideoInfo( QString const & videoID );
    }

    class Stream
    {
    public:

        static bool sortByBitRate( Stream const & a, Stream const & b );
        typedef QPair<int, int> IntPair;
        Stream( QVariantMap const & sm, QString const & title );

        bool threed() const;
        int rawBitRate() const;
        QString const & rawUrl() const;
        QString const & bitRate() const;
        QString const & itag() const;
        QString const & resolution() const;
        QString const & quality() const;
        QString const & title() const;
        QString const & extension() const;
        QString const & mediaType() const;
        QString const & filename() const;
        IntPair         dimension() const;
        QString const & url() const;
        QString         url_https() const;
        void            download();

    private:
        void init( QVariantMap const & sm );
        QString generateFilename();
        QString m_title ,
                m_itag,
                m_mediaType,
                m_resolution,
                m_quality,
                m_rawUrl,
                m_url,
                m_sig,
                m_filename ,
                m_note ,
                m_extension,
                m_videoFormat,
                m_bitRate;
        int     m_fsize,
                m_rawBitRate;
        IntPair m_dimension;
        bool    m_isDash,
                m_active,
                m_threed,
                m_encrypted;
    };

    class UrlFinder
    {
    public:
        explicit        UrlFinder( QString const & videoUrl, bool start = false );
        void            startUrlExtraction();
        QString const & getTitle()const;
        QList<Stream>   getVideoStreams() const;
        QList<Stream>   getAudioStreams() const;
        QList<Stream>   getAllStreams() const;
        int             getVideoStreamLength() const;
    private:
        void            initFunctions();
        void            fetchBasic();
        void            fetchBasicDelegate();
        void            processStreams( QVariantList const & dash);
    private:
        QString         m_videoUrl,
                        m_title,
                        m_dashUrl,
                        m_author,
                        m_videoRating,
                        m_length,
                        m_viewCount,
                        m_thumb;
        QString         m_videoID,
                        m_keyword,
                        m_bigThumb,
                        m_bigThumbHD,
                        m_cipherTag,
                        m_watchVideoUrl;
        QVariantList    m_formats;
        QVariantList    m_sm;
        QVariantList    m_asm;
        QList<Stream>   m_streams;
        QList<Stream>   m_audioStreams;
        QList<Stream>   m_videoStreams;
        bool            m_hasBasic;
    };
} /* namespace Purple */

#endif /* STREAM_HPP_ */
