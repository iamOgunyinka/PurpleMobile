/*
 * MyIndexMapper.hpp
 *
 *  Created on: Nov 8, 2015
 *      Author: Joshua
 */

#ifndef MYINDEXMAPPER_HPP_
#define MYINDEXMAPPER_HPP_

#include <bb/cascades/DataModel>

namespace Purple
{
    class MyIndexMapper: public bb::cascades::DataModel::IndexMapper
    {
    public:
        MyIndexMapper( int index, int count, bool deleted );
        bool newIndexPath( QVariantList *pNewIndexPath, int *pReplacementIndex, const QVariantList &oldIndexPath ) const;
    private:
        int m_index;
        int m_count;
        bool m_deleted;
    };

} /* namespace Purple */

#endif /* MYINDEXMAPPER_HPP_ */
