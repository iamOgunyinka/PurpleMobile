/*
 * UrlExtractor.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: Joshua
 */

#include <src/UrlExtractor.hpp>
#include <QUrl>

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

        itags["5"] = Mapping{  "320x240", "flv", "normal", "" };
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

            return reply->readAll();
        }

        inline QVariantMap parseQueryString( QByteArray const & data )
        {
            QUrlQuery urlQuery( data );
            QList<QPair<QString, QString>> list = urlQuery.queryItems();

            QVariantMap map{};
            if( !list.isEmpty() ){
                for( auto const & m: list ){
                    map.insert( m.first, m.second );
                }
            }
            return map;
        }

    }
    UrlExtractor::UrlExtractor()
    {
        // TODO Auto-generated constructor stub

    }

    UrlExtractor::~UrlExtractor()
    {
        // TODO Auto-generated destructor stub
    }

} /* namespace Purple */
