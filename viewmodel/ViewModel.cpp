#include "ViewModel.h"

#include "model/PrimitiveItems.h"
#include "model/UserTypes.h"

#include <iostream>

namespace Sequencer {

namespace ViewModel {

Model::Item * TreeViewModel::ItemFromIndex(const QModelIndex & index) const
{
    return index.isValid() ? static_cast<Model::Item *>(index.internalPointer()) : nullptr;
}

int TreeViewModel::RowForItem(Model::Item * item) const
{
    auto parent_item = item->GetParent();
    if (!parent_item && item == __root_item)
    {
        return 0;
    }
    else
    {
        auto all_items = parent_item->GetAllItems();
        for (int i=0; i<all_items.size(); ++i)
        {
            if (item == all_items[i])
            {
                return i;
            }
        }
    }
    return -1;
}

TreeViewModel::TreeViewModel(Model::Item * root_item)
    : __root_item(root_item)
{}

int TreeViewModel::rowCount(const QModelIndex & parent) const
{
    if (!parent.isValid())
        return __root_item == nullptr ? 0 : 1;
    return ItemFromIndex(parent)->GetTotalItemCount();
}

int TreeViewModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant TreeViewModel::data(const QModelIndex & index, int role) const
{
    int col = index.column();
    if (index.isValid() && role == Qt::DisplayRole)
    {
        auto item = ItemFromIndex(index);
        switch (col)
        {
        case 0:
        {
            return QString::fromStdString(item->GetDisplayName());
        }
        case 1:
        {
            if (item->HasData())
            {
                return Model::Utils::ToQtVariant(item->GetDataVariant());
            }
            break;
        }
        default:
            break;
        }
    }
    return QVariant();
}

bool TreeViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    int col = index.column();
    if (index.isValid() && row < rowCount() && role == Qt::EditRole)
    {
        auto children = __root_item->GetAllItems();
        auto item = children[row];
        switch (col)
        {
        case 0:
        {
            return false;
        }
        case 1:
        {
            auto stringitem = dynamic_cast<Model::StringItem *>(item);
            if (stringitem)
            {
                auto str = value.toString();
                stringitem->SetData(str.toStdString());
                emit dataChanged(index, index);
                return true;
            }
            break;
        }
        default:
            return false;
        }
    }
    return false;
}

Qt::ItemFlags TreeViewModel::flags(const QModelIndex & index) const
{
    int col = index.column();
    if (col == 1)
    {
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    }
    return QAbstractItemModel::flags(index);
}

QVariant TreeViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Type");
        case 1:
            return QString("Value");
        }
    }
    return QVariant();
}

QModelIndex TreeViewModel::index(int row, int column, const QModelIndex & parent) const
{
    if (column < 0 || column >= columnCount(parent))
    {
        return QModelIndex();
    }
    if (!parent.isValid())
    {
        return createIndex(row, column, __root_item);
    }
    else
    {
        auto parent_item = ItemFromIndex(parent);
        auto child_items = parent_item->GetAllItems();
        return createIndex(row, column, child_items[row]);
    }
    return QModelIndex();
}

QModelIndex TreeViewModel::parent(const QModelIndex & index) const
{
    auto parent_item = ItemFromIndex(index)->GetParent();
    if (parent_item)
    {
        int row = RowForItem(parent_item);
        return createIndex(row, 0, parent_item);
    }
    return QModelIndex();
}

}  // namespace ViewModel

}  // namespace Sequencer
