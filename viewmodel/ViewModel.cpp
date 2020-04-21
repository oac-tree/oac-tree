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
