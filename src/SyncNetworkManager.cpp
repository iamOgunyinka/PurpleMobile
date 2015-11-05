#include <src/SyncNetworkManager.hpp>

namespace Purple
{
    SyncNetworkManager::SyncNetworkManager( QObject *parent ):
                    QObject( parent ), m_networkManager()
    {
        QObject::connect( &m_networkManager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
                this, SLOT(onError( QNetworkAccessManager::NetworkAccessibility )));
    }

    SyncNetworkManager::~SyncNetworkManager()
    {
    }

    void SyncNetworkManager::sendRequest( QString const & url )
    {
        QNetworkRequest request;
        request.setUrl( QUrl( url ) );
        request.setRawHeader( "Agent", "Wiikast Purple" );

        QNetworkReply *reply = m_networkManager.get( request );
        QObject::connect( reply, SIGNAL( finished()), this, SLOT( onFinished() ) );
        QObject::connect( reply, SIGNAL( error( QNetworkReply::NetworkError )), this, SLOT( onError( QNetworkReply::NetworkError )));

        m_requestQueue.push_back( reply );
    }

    void SyncNetworkManager::cancelLastRequest()
    {
        if( !m_requestQueue.empty() ){
            m_requestQueue.front()->abort();
            m_requestQueue.pop_front();
            emit errorOccurred( "Current request cancelled successfully" );
        }
    }

    void SyncNetworkManager::onFinished()
    {
        QNetworkReply *response = qobject_cast<QNetworkReply*>( sender() );
        if( response->error() != QNetworkReply::NoError ){
            emit errorOccurred( response->errorString() );
        } else {
            emit finished( response->readAll() );
        }
        m_requestQueue.removeOne( response );
        response->deleteLater();
    }

    QString SyncNetworkManager::toPercentageEncoding( QString const & text )
    {
        return QUrl::toPercentEncoding( text );
    }

    //Blackberry NDK 2.0 refuses namespace( fully ) qualified conditionals like QNetworkReply::NetwokError::RemostHostClosedError
    void SyncNetworkManager::onError( QNetworkReply::NetworkError error )
    {
        switch( error )
        {
            case 1:
                emit networkError( "Connection Refused by Host" );
                break;
            case 2:
                emit networkError( "Remote Host closed connection" );
                break;
            case 3:
                emit networkError( "Host not found" );
                break;
            case 4:
                emit networkError( "Timed out" );
                break;
            case 5:
                emit networkError( "Operation Cancelled" );
                break;
            case 6:
                emit networkError( "SSL Handshake error" );
                break;
            case 7:
                emit networkError( "Temporary Network Failure" );
                break;
            default:
                emit networkError( "Unknown Network Error" );
                break;
        }
    }

    void SyncNetworkManager::onError( QNetworkAccessManager::NetworkAccessibility err )
    {
        switch( err )
        {
            case -1: case 0:
                emit networkError( "Network Inaccessible" );
                break;
            case 1: default:
                break;
        }
    }
} /* namespace Purple */
