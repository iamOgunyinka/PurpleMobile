#ifndef SYNCNETWORKMANAGER_HPP_
#define SYNCNETWORKMANAGER_HPP_

#include <bb/data/JsonDataAccess>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>
#include "ProjectFile.hpp"

namespace Purple
{
    class SyncNetworkManager: public QObject
    {
        Q_OBJECT

	public:
        SyncNetworkManager( QObject *parent = NULL );
        virtual ~SyncNetworkManager();

        Q_INVOKABLE void sendRequest( QString const & request );
        Q_INVOKABLE void cancelLastRequest();
        Q_INVOKABLE QString toPercentageEncoding( QString const & text );
	private:
        QNetworkAccessManager m_networkManager;
        QList<QNetworkReply*> m_requestQueue;
	public slots:
		Q_SLOT void onFinished();
		Q_SLOT void onError( QNetworkReply::NetworkError );
    signals:
        void networkError( QString const & what );
        void errorOccurred( QString const & what );
        void finished( QString const & message );
    };
} /* namespace Purple */

#endif /* SYNCNETWORKMANAGER_HPP_ */
