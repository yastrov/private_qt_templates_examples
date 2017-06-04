/*
* Copyright (C) 2011 Stephane 'sOuSiX' C.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB. If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
*
* Has been modified by Yuriy Astrov.
*/

#include "sqltablemodelcheckable.h"

SqlTableModelCheckable::SqlTableModelCheckable(int primaryKeyColumn, QObject *parent, QSqlDatabase db)
                        : QSqlTableModel(parent, db), m_idSet(), m_fontBold(), m_fontNormal()
{
    m_fontBold.setBold(true);
    m_primaryKeyColumn = primaryKeyColumn;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Qt::ItemFlags SqlTableModelCheckable::flags(const QModelIndex &index) const
{
    return ( (QSqlTableModel::flags(index) | Qt::ItemIsUserCheckable ) & ~Qt::ItemIsEditable );
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int SqlTableModelCheckable::primaryKey(int row) const
{
    return index(row, m_primaryKeyColumn).data().toInt();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

QVariant SqlTableModelCheckable::data(const QModelIndex& index, int role) const
{
    if(index.column() == 1) {
        if(role == Qt::CheckStateRole) {
            if(m_idSet.contains(primaryKey(index.row())))
                return Qt::Checked;
            else
                return Qt::Unchecked;
        }
        if(role == Qt::FontRole) {
            if(m_idSet.contains(primaryKey(index.row())))
                return m_fontBold;
            else
                return m_fontNormal;
        }
    }
    return QSqlTableModel::data(index, role);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool SqlTableModelCheckable::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(role == Qt::CheckStateRole && index.column() == 1)
    {
        if ( value == Qt::Checked )
            m_idSet.insert(primaryKey(index.row()));
        else
            m_idSet.remove(primaryKey(index.row()));

        emit dataChanged( index, index );
        return true;
    }

    return QSqlTableModel::setData( index, value, role );
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool SqlTableModelCheckable::removeRows(int row, int count, const QModelIndex &parent)
{
    if(m_idSet.count()>0) {
        const int temp = row+count;
        for (int i = row; i < temp && m_idSet.count()>0; ++i) {
            m_idSet.remove(primaryKey(i));
        }
    }
    return QSqlTableModel::removeRows( row, count, parent );
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool SqlTableModelCheckable::hasBoxChecked() const
{
    return ( m_idSet.count() > 0 );
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int SqlTableModelCheckable::countBoxChecked() const
{
    return m_idSet.count();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SqlTableModelCheckable::clearBox()
{
    m_idSet.clear();
    emit dataChanged( this->index(0, 1), this->index(this->rowCount()-1, 1) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////

QString SqlTableModelCheckable::filter()
{
    if( QSqlTableModel::filter().isEmpty() )
        return "1";

    return QSqlTableModel::filter();
}
