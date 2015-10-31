/*
 * DownloadsDataModel.hpp
 *
 *  Created on: Oct 31, 2015
 *      Author: Joshua
 */

#ifndef DOWNLOADSDATAMODEL_HPP_
#define DOWNLOADSDATAMODEL_HPP_

#include <bb/cascades/DataModel>
#include <bb/data/JsonDataAccess>
#include <QStringList>

namespace Purple
{

    class DownloadsDataModel: public bb::cascades::DataModel
    {
        Q_OBJECT

        Q_PROPERTY( QString source READ source WRITE setSource NOTIFY sourceChanged )
    public:
        DownloadsDataModel( QObject *parent = 0 );
        virtual ~DownloadsDataModel();
    private:
        void                    setSource( QString const & );
        QString                 source() const;
        void                    load( QString const & filename );

        QString                 m_source;
        QVariantList            m_downloadList;
    public slots:
        Q_INVOKABLE int         childCount( QVariantList const & indexPath );
        Q_INVOKABLE bool        hasChildren( QVariantList const & indexPath );
        Q_INVOKABLE QVariant    data( QVariantList const & indexPath );
        Q_INVOKABLE QString     itemType( QVariantList const & indexPath );
        Q_INVOKABLE void        removeItem( QVariantList const & indexPath );
    signals:
        void    sourceChanged( QString const &newSource );
        void    error( QString const & errorMessage );
    };

} /* namespace Purple */

#endif /* DOWNLOADSDATAMODEL_HPP_ */
