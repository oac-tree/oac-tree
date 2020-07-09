#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QAbstractItemModel>

#include "model/Item.h"

namespace Sequencer {

namespace ViewModel {

/**
 * @brief Abstract interface for all model items
 * @details Item is the base class for all model items that
 * need to be represented, created and edited. These include:
 * sequences, primitive values, variable definitions, etc.
 *
 * @note The design is based on the composite pattern to compose
 * tree structures by assembling primitive or composite items into
 * increasingly more complex structures.
 */
class TreeViewModel : public QAbstractItemModel
{
private:
    Model::Item * __root_item;

    Model::Item * ItemFromIndex(const QModelIndex & index) const;

    int RowForItem(Model::Item * item) const;

protected:

public:
    /**
     * @brief Constructor
     * @param root_item Root of the Item tree to represent
     */
    TreeViewModel(Model::Item * root_item);

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;

    int columnCount(const QModelIndex & parent = QModelIndex()) const override;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex & index, const QVariant & value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex & index) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex & index) const override;
};

}  // namespace ViewModel

}  // namespace Sequencer

#endif // VIEWMODEL_H
