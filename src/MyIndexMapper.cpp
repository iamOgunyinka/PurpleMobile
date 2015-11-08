/*
 * MyIndexMapper.cpp
 *
 *  Created on: Nov 8, 2015
 *      Author: Joshua
 */

#include <src/MyIndexMapper.hpp>

namespace Purple
{
    MyIndexMapper::MyIndexMapper( int index, int count, bool deleted ):
                        m_index( index ), m_count( count ), m_deleted( deleted )
    {
    }

    bool MyIndexMapper::newIndexPath( QVariantList *pNewIndexPath, int *replacementIndex, QVariantList const & oldIndexPath ) const
    {
        if (oldIndexPath[0].toInt() < m_index) {
            pNewIndexPath->append(oldIndexPath);
            return true;

            // Deleted indices
        } else if (m_deleted && oldIndexPath[0].toInt() <= m_index + m_count) {
            *replacementIndex = m_index;
            return false;

            // Indices after a deletion or addition
        } else {
            if (m_deleted)
                pNewIndexPath->append(oldIndexPath[0].toInt() - m_count);
            else
                pNewIndexPath->append(oldIndexPath[0].toInt() + m_count);
            return true;
        }
    }

} /* namespace Purple */
