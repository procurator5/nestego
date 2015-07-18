#ifndef VLINEEDITDELEGATE_H
#define VLINEEDITDELEGATE_H

#include <QItemDelegate>

class vLineEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit vLineEditDelegate(QObject *parent = 0);
    virtual QWidget *createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    virtual void destroyEditor(QWidget * editor, const QModelIndex & index) const;
    virtual void	setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
    virtual void	setEditorData ( QWidget * editor, const QModelIndex & index ) const;
signals:

public slots:

};

#endif // VLINEEDITDELEGATE_H
