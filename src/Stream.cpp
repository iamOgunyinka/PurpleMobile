/*
 * Stream.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua
 */

#include <src/Stream.hpp>
#include <QUrl>
#include <QEventLoop>
#include <QRegExp>
#include <QDomDocument>
#include <QStringList>

namespace Purple
{
    QString Constants::uefsm = "url_encoded_fmt_stream_map";
    QString Constants::af = "adaptive_fmts";
    QString Constants::jsplayer  = "(;\\s*ytplayer\\.config\\s*=\\s*(\\s*{.*?}\\s*)\\s*;)";
    QString Constants::user_agent  = "Purple";
    int Constants::lifespan = 60 * 60 * 5;

    QVariantMap Constants::urls;
    QMap<QString, Mapping> Constants::itags;

    void Constants::init()
    {
        urls["gdata"]= "http://gdata.youtube.com/feeds/api/videos/%1?v=2";
        urls["watch_v"] = "http://www.youtube.com/watch?v=%1";
        urls["vidinfo"] = "http://www.youtube.com/get_video_info?video_id=%1&asv=3&el=detailpage&hl=en_US";
        urls["playlist"] = "http://www.youtube.com/list_ajax?style=json&action_get_list=1&list=%1";
        urls["age_vidinfo"] = "http://www.youtube.com/get_video_info?video_id=%1&eurl="
                "https://youtube.googleapis.com/v/%2&sts=1588";

        itags["5"] = Mapping{ "320x240", "flv", "normal", "" };
        itags["17"] = Mapping{  "176x144", "3gp", "normal", "" };
        itags["18"]= Mapping{  "640x360", "mp4", "normal", "" };
        itags["22"] = Mapping{  "1280x720", "mp4", "normal", "" };
        itags["34"] = Mapping{  "640x360", "flv", "normal", "" };
        itags["35"] = Mapping{  "854x480", "flv", "normal", "" };
        itags["36"] = Mapping{  "320x240", "3gp", "normal", "" };
        itags["37"] = Mapping{  "1920x1080", "mp4", "normal", "" };
        itags["38"] = Mapping{  "4096x3072", "mp4", "normal", "4,3 hi-res" };
        itags["43"] = Mapping{  "640x360", "webm", "normal", "" };
        itags["44"] = Mapping{  "854x480", "webm", "normal", "" };
        itags["45"] = Mapping{  "1280x720", "webm", "normal", "" };
        itags["46"] = Mapping{  "1920x1080", "webm", "normal", "" };
        itags["82"] = Mapping{  "640x360-3D", "mp4", "normal", "" };
        itags["83"] = Mapping{  "640x480-3D", "mp4", "normal", "" };
        itags["84"] = Mapping{  "1280x720-3D", "mp4", "normal", "" };
        itags["100"] = Mapping{  "640x360-3D", "webm", "normal", "" };
        itags["102"] = Mapping{  "1280x720-3D", "webm", "normal", "" };
        itags["133"] = Mapping{  "426x240", "m4v", "video", "" };
        itags["134"] = Mapping{  "640x360", "m4v", "video", "" };
        itags["135"] = Mapping{  "854x480", "m4v", "video", ""};
        itags["136"] = Mapping{  "1280x720", "m4v", "video", ""};
        itags["137"] = Mapping{  "1920x1080", "m4v", "video", ""};
        itags["138"] = Mapping{  "4096x3072", "m4v", "video", ""};
        itags["139"] = Mapping{  "48k", "m4a", "audio", ""};
        itags["140"] = Mapping{  "128k", "m4a", "audio", ""};
        itags["141"] = Mapping{  "256k", "m4a", "audio", ""};
        itags["160"] = Mapping{  "256x144", "m4v", "video", ""};
        itags["167"] = Mapping{  "640x480", "webm", "video", ""};
        itags["168"] = Mapping{  "854x480", "webm", "video", ""};
        itags["169"] = Mapping{  "1280x720", "webm", "video", ""};
        itags["170"] = Mapping{  "1920x1080", "webm", "video", ""};
        itags["171"] = Mapping{  "128k", "ogg", "audio", ""};
        itags["172"] = Mapping{  "192k", "ogg", "audio", ""};
        itags["218"] = Mapping{  "854x480", "webm", "video", "VP8"};
        itags["219"] = Mapping{  "854x480", "webm", "video", "VP8"};
        itags["242"] = Mapping{  "360x240", "webm", "video", "VP9"};
        itags["243"] = Mapping{  "480x360", "webm", "video", "VP9"};
        itags["244"] = Mapping{  "640x480", "webm", "video", "VP9 low"};
        itags["245"] = Mapping{  "640x480", "webm", "video", "VP9 med"};
        itags["246"] = Mapping{  "640x480", "webm", "video", "VP9 high"};
        itags["247"] = Mapping{  "720x480", "webm", "video", "VP9"};
        itags["248"] = Mapping{  "1920x1080", "webm", "video", "VP9"};
        itags["249"] = Mapping{  "48k", "ogg", "audio", "Opus"};
        itags["250"] = Mapping{  "56k", "ogg", "audio", "Opus"};
        itags["251"] = Mapping{  "128k", "ogg", "audio", "Opus"};
        itags["256"] = Mapping{  "192k", "m4a", "audio", "6-channel"};
        itags["258"] = Mapping{  "320k", "m4a", "audio", "6-channel"};
        itags["264"] = Mapping{  "2560x1440", "m4v", "video", ""};
        itags["266"] = Mapping{   "3840x2160", "m4v", "video", "AVC"};
        itags["271"] = Mapping{  "1920x1280", "webm", "video", "VP9"};
        itags["272"] = Mapping{  "3414x1080", "webm", "video", "VP9"};
        itags["278"] = Mapping{  "256x144", "webm", "video", "VP9" };
        itags["298"] = Mapping{  "1280x720", "m4v", "video", "60fps" };
        itags["299"] = Mapping{  "1920x1080", "m4v", "video", "60fps" };
        itags["302"] = Mapping{  "1280x720", "webm", "video", "VP9" };
        itags["303"] = Mapping{  "1920x1080", "webm", "video", "VP9" };
    }
    namespace HelperFunctions
    {
        QByteArray fetchDecode( QString const & url_ )
        {
            QString url = url_;
            while( url.contains( '%' ) )
            {
                url = QUrl::fromPercentEncoding( url.toUtf8() );
            }
            QNetworkAccessManager networkManager;
            QNetworkReply *reply = networkManager.get( QNetworkRequest( QUrl( url ) ) );

            QEventLoop loop;

            QObject::connect( reply, SIGNAL(finished()), &loop, SLOT( quit() ) );
            QObject::connect( reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()) );
            loop.exec();

            return reply->readAll();
        }

        inline QVariantMap parseQueryString( QString const & data )
        {
            QUrl urlQuery( data );
            QList<QPair<QByteArray, QByteArray> > list = urlQuery.encodedQueryItems();

            QVariantMap map;
            if( !list.isEmpty() ){
                for( QList<QPair<QByteArray, QByteArray> >::iterator m = list.begin(); m != list.end(); ++m ){
                    map.insert( m->first, m->second );
                }
            }
            return map;
        }

        QString extractVideoID( QString const & url )
        {
            QString const re_string = "(?:^|[^\\w-]+)([\\w-]{11})(?:[^\\w-]+|$)";
            QRegExp re( re_string );

            if( re.indexIn( url ) != -1 ){
                return re.cap(1);
            }
            return QString();
        }

        QVariantList extractSmap( QString const & key, QVariantMap const & data )
        {
            QVariantList list_of_smaps;
            if( data.contains( key ) ){
                QString smap = data.value( key ).toString();
                QStringList splitted_map = smap.split( "," ); //decoded string
                if( splitted_map.size() == 1 ){
                    splitted_map = smap.split( "%2C" ); //encoded string
                }

                for( int i = 0; i != splitted_map.size(); ++i ){
                    list_of_smaps.push_back( parseQueryString( splitted_map[i] ) );
                }
            }
            return list_of_smaps;
        }

        QVariantList extractDash( QString const & dashUrl )
        {
            QByteArray dashdata ( fetchDecode( dashUrl )); //May throw an exception from fetchDecode

            QString ns = "urn:mpeg:DASH:schema:MPD:2011";
            QDomDocument xmlDocument;
            if( !xmlDocument.setContent( dashdata, true ) ) return QVariantList();

            QDomElement root = xmlDocument.documentElement();
            QDomNodeList tlist = root.elementsByTagNameNS( ns, "Representation" );
            QRegExp re ( "(?:\\?|&)mime=([\\w\\d\\/]+)" );
            QVariantList dash_list;

            for( int index = 0; index != tlist.size(); ++index )
            {
                QDomNode node = tlist.item( index );
                if( node.isElement() )
                {
                    QDomElement x = node.toElement();
                    QDomNodeList node_list = x.elementsByTagName( "BaseURL" );
                    QDomElement base_url = node_list.at( 0 ).toElement();
                    QString url = base_url.text();
                    QString size = base_url.attribute( "contentLength", "0" );
                    QString bitrate = x.attribute( "bandwidth" );
                    QString itag = x.attribute( "id" );
                    QString width = x.attribute( "width" );
                    QString height = x.attribute( "height" );
                    QString type = re.indexIn( url ) != -1 ? re.cap(1): QString() ;

                    QVariantMap mapp;
                    mapp["bitrate"] = bitrate;
                    mapp["dash"] = "true";
                    mapp["itag"] = itag;
                    mapp[ "width" ] = width;
                    mapp["height"] = height;
                    mapp["url"] = url;
                    mapp["size"] =  size;
                    mapp["type"] = type;

                    dash_list.push_back( mapp);
                }
            }
            return dash_list;
        }

        QVariantMap getVideoInfo( QString const & videoID )
        {
            QString url = Constants::urls["vidinfo"].toString().arg( videoID );
            QByteArray info = fetchDecode( url ); //May throw an exception from fetchDecode
            if( info.isEmpty() ) return QVariantMap();

            return parseQueryString( info );
        }

    }

    Stream::Stream( QVariantMap const & sm, QString const & title ): m_title( title )
    {
        init( sm );
    }

    void Stream::init( QVariantMap const & sm )
    {
        m_itag = sm.value( "itag" ).toString();
        m_isDash = sm.contains( "dash" );
        if( !Constants::itags.contains( m_itag ) ) throw QString( "No encoding found" );

        m_mediaType = Constants::itags.value( m_itag ).type;
        m_threed = ( sm.contains( "stereo3d" ) && sm.value( "stereo3d" ).toString() == "1" );

        if( m_isDash )
        {
            if ( sm["width"].toString() != QString( "None" ) || !sm["width"].toString().isEmpty() )
            {
                m_quality = m_resolution = QString("%1x%2").arg(sm["width"].toString() ).arg(sm["height"].toString());
                m_dimension = IntPair( sm["width"].toInt(), sm["height"].toInt() );
            } else {
                m_resolution = "0x0";
                m_dimension = IntPair( 0, 0 );
                m_rawBitRate = sm["bitrate"].toInt();
                m_quality = m_bitRate = Constants::itags[m_itag].dimension;
            }
            m_fsize = sm["size"].toInt();
        } else {
            m_quality = m_resolution = Constants::itags[m_itag].dimension;
            m_bitRate = m_rawBitRate = m_fsize = 0;
        }
        m_videoFormat = sm["type"].toString().split(";")[0];
        m_extension = Constants::itags[m_itag].type;
        m_encrypted = sm.contains( "s" );
        m_note = Constants::itags[m_itag].additional_info;
        m_rawUrl = sm["url"].toString();
        m_sig = ( m_encrypted ? sm["s"].toString() : sm["sig"].toString() );
        m_active = false;

        if( m_mediaType == QString( "audio" ) && !m_isDash )
        {
            m_dimension = IntPair( 0, 0 );
            m_quality = m_bitRate = m_resolution;
            m_resolution = "0x0";
            m_rawBitRate = sm["bitrate"].toInt();
        }
    }

    bool Stream::threed() const { return m_threed; }
    int Stream::rawBitRate() const { return m_rawBitRate; }
    QString const & Stream::rawUrl() const { return m_rawUrl; }
    QString const & Stream::bitRate() const { return m_bitRate; }
    QString const & Stream::itag() const { return m_itag; }
    QString const & Stream::resolution() const { return m_resolution; }
    QString const & Stream::quality() const { return m_quality; }
    QString const & Stream::title() const { return m_title; }
    QString const & Stream::extension() const { return m_extension; }
    QString const & Stream::mediaType() const { return m_mediaType; }
    QString const & Stream::filename() const { return m_filename; }
    Stream::IntPair Stream::dimension() const { return m_dimension; }
    QString Stream::url_https() const
    {
        QString temp = m_url;
        return temp.replace( "http", "https" );
    }
    bool Stream::sortByBitRate( Stream const & a, Stream const & b ){ return a.extension() < b.extension(); }

    UrlFinder::UrlFinder( QString const & videoUrl, bool start ): m_videoUrl( videoUrl )
    {
        if( start ){
            startUrlExtraction();
        }
    }

    void UrlFinder::startUrlExtraction()
    {
        if( !m_hasBasic ){
            initFunctions();
        }
    }

    QString const & UrlFinder::getTitle() const { return m_title; }

    void UrlFinder::initFunctions()
    {
        m_videoID = HelperFunctions::extractVideoID( m_videoUrl );

        m_watchVideoUrl = Constants::urls["watch_v"].toString().arg( m_videoID );
        fetchBasic();
    }

    void UrlFinder::fetchBasic()
    {
        if( m_hasBasic ) return;

        m_hasBasic = true;
        fetchBasicDelegate();
        QVariantList dash = HelperFunctions::extractDash( m_dashUrl );
        if( dash.isEmpty() ) return;

        processStreams( dash );
    }

    void UrlFinder::fetchBasicDelegate()
    {
        QVariantMap elem = HelperFunctions::getVideoInfo( m_videoID );
        if( elem.isEmpty() ) throw QString( "No associated information with this video ID" );

        struct GetList {
            QVariantMap & element;
            GetList( QVariantMap & e ): element( e ){}
            QString operator()( QString const & key, QString const & default_ = "unknown" )
            {
                return element.value( key, default_ ).toString();
            }
        };

        GetList get_lst( elem );

        m_title = get_lst( "title" );
        m_dashUrl = get_lst( "dashmpd" );
        m_author = get_lst( "author" );
        m_videoID = get_lst( "video_id" );
        m_videoRating = get_lst( "avg_rating", "0.0" );
        m_length = get_lst( "length_seconds", "0" );
        m_viewCount = get_lst( "view_count", "0" );
        m_thumb = get_lst( "thumbnail_url" );

        QStringList splitFormatByComma = get_lst( "fmt_lst" ).split(",");
        for( int i = 0; i != splitFormatByComma.size(); ++i ) {
            m_formats.push_back( splitFormatByComma[i].split( "/" ) );
        }
        m_keyword = get_lst( "keywords", "" );
        m_bigThumb = get_lst( "iurlsd", "" );
        m_bigThumbHD = get_lst( "iurlsdmaxres", "" );
        m_cipherTag = get_lst( "use_cipher_signature" ) == "True";
        m_sm = HelperFunctions::extractSmap( Constants::uefsm, elem );
        m_asm = HelperFunctions::extractSmap( Constants::af, elem );
    }

    void UrlFinder::processStreams( QVariantList const & dash)
    {
        if( !m_hasBasic ) fetchBasic();

        for( int i = 0; i != m_sm.size(); ++i ){
            Stream stream = Stream( m_sm[i].toMap(), getTitle() );
            if( Constants::itags.contains( stream.itag() ) )
                m_streams.push_back( stream );
        }
        QList<Stream> adptStreams, dashStreams;
        for( int i = 0; i != m_asm.size(); ++i ){
            Stream temp = Stream( m_asm.at( i ).toMap(), getTitle() );
            if( Constants::itags.contains( temp.itag() ) ){
                adptStreams.push_back( temp );
            }
        }

        for( int i = 0; i != dash.size(); ++i ){
            Stream stream = Stream( dash[i].toMap(), getTitle() );
            if( Constants::itags.contains( stream.itag() ) )
                dashStreams.push_back( stream );
        }

        QList<QString> dash_itags;
        for( int i = 0; i != dashStreams.size(); ++i ) dash_itags.push_back( dashStreams[i].itag() );

        for( int i = 0; i != adptStreams.size(); ++i ){
            Stream const stream = adptStreams[i];
            if( stream.bitRate().isEmpty() && !dash_itags.contains( stream.itag() ) ){
                m_videoStreams.push_back( stream );
            } else {
                if( !stream.bitRate().isEmpty() && !dash_itags.contains( stream.itag() )){
                    m_audioStreams.push_back( stream );
                }
            }
        }

        for( int i = 0; i != dashStreams.size(); ++i )
        {
            if( dashStreams[i].mediaType() == QString( "audio" ) ){
                m_audioStreams.push_back( dashStreams[i] );
            } else {
                m_videoStreams.push_back( dashStreams[i] );
            }
        }

        std::sort( m_audioStreams.begin(), m_audioStreams.end(), Stream::sortByBitRate );
        std::sort( m_videoStreams.begin(), m_videoStreams.end(), Stream::sortByBitRate );
    }

    QList<Stream> UrlFinder::getVideoStreams()  const { return m_videoStreams; }
    QList<Stream> UrlFinder::getAudioStreams()  const { return m_audioStreams; }
    QList<Stream> UrlFinder::getAllStreams()    const { return m_streams; }
    int           UrlFinder::getVideoStreamLength() const { return m_length.toInt(); }
}
