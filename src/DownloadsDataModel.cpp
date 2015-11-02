/*
 * DownloadsDataModel.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: Joshua
 */

#include <src/DownloadsDataModel.hpp>

namespace Purple
{

    DownloadsDataModel::DownloadsDataModel( QObject *parent ): bb::cascades::DataModel( parent )
    {

    }

    DownloadsDataModel::~DownloadsDataModel()
    {
    }

    QString DownloadsDataModel::source() const { return m_source; }

    void DownloadsDataModel::setSource( QString const & newSource ) {
        if( newSource != m_source ){
            m_source = newSource;
            load( newSource );
            emit sourceChanged( newSource );
        }
    }

    void DownloadsDataModel::load( QString const & sourceFile )
    {
        bb::data::JsonDataAccess jda;
        QVariantList list = jda.load( QDir::currentPath() + "/app/data/asset/" + sourceFile ).toList();

        if( jda.hasError() ){
            emit error( jda.error().errorMessage() );
            return;
        }
        for( int i = 0; i != list.size(); ++i ){
            m_downloadList.append( list[i].toMap() );
        }
    }

    bool DownloadsDataModel::hasChildren( QVariantList const & indexPath )
    {
        if( indexPath.size() == 0 || indexPath.size() == 1 ){
            return true;
        }
        return false;
    }

    int DownloadsDataModel::childCount( QVariantList const & indexPath )
    {
        if( indexPath.size() == 0 ){ //root hierarchy, all downloads
            return 1;
        } else {
            if( indexPath.size() == 1 ){
                if( indexPath.at( 0 ).toInt() == 0 ){
                    return m_downloadList.size();
                }
            } else {
                return 0;
            }
        }
        return 0;
    }

    QVariant DownloadsDataModel::data( QVariantList const & indexPath )
    {
        if( indexPath.size() == 1 ){
            if( indexPath.at( 0 ).toInt() == 0 ){
                QVariantMap header;
                header["value"] = "All Downloads";
                return header;
            }
        } else if( indexPath.size() == 2 ) {
            return m_downloadList.at( indexPath.at( 1 ).toInt() );
        }
        return QVariant();
    }

    QString DownloadsDataModel::itemType( QVariantList const & )
    {
        return QString( "all" );
    }

    QVariantList DownloadsDataModel::downloads() { return m_downloadList; }
    // TODO
    void DownloadsDataModel::removeItem( QVariantList const & )
    {

    }
} /* namespace Purple */
