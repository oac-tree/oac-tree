#include "ViewModel.h"

#include "model/PrimitiveItems.h"

namespace Sequencer {

namespace ViewModel {

TreeViewModel::TreeViewModel(Model::Item *root_item)
    : __root_item(root_item)
{

}

int TreeViewModel::rowCount(const QModelIndex & parent) const
{
    if (!parent.isValid())
        return __root_item->GetChildrenCount();
    return 0;
}

int TreeViewModel::columnCount(const QModelIndex & parent) const
{
    if (!parent.isValid())
        return 2;
    return 0;
}

QVariant TreeViewModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    if (index.isValid() && row < rowCount() && role == Qt::DisplayRole)
    {
        auto children = __root_item->GetChildren();
        auto item = children[row];
        switch (col)
        {
        case 0:
        {
            return QString::fromStdString(item->GetDisplayName());
        }
        case 1:
        {
            auto stringitem = dynamic_cast<Model::StringItem *>(item);
            if (stringitem)
                return QString::fromStdString(stringitem->GetData());
            return QString("Unknown");
        }
        default:
            return QVariant();
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
        auto children = __root_item->GetChildren();
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
    if (!parent.isValid() && row < rowCount() && column < columnCount())
    {
        return createIndex(row, column);
    }
    return QModelIndex();
}

QModelIndex TreeViewModel::parent(const QModelIndex & /*index*/) const
{
    return QModelIndex();
}

}  // namespace ViewModel

}  // namespace Sequencer
