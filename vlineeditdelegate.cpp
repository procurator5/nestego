#include "vlineeditdelegate.h"
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>

#include "vchoosedirectory.h"
#include "vproject.h"

extern vProject *currentProject;

vLineEditDelegate::vLineEditDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}


QWidget *	vLineEditDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const{

    if(index.column() == 0)
        return NULL;

    if (index.parent().row()==0 && index.row() ==1)
        return new QLineEdit(parent);

    if (index.parent().row()==0 && index.row() ==2)
        return new QCheckBox(parent);

    if(index.parent().row()==2){
        QSpinBox *sb =new QSpinBox(parent);
        if(index.row()!=2 )
            sb->setMaximum(10000);
        return sb;
    }

    return new vChooseDirectory(parent);
}

void vLineEditDelegate::destroyEditor(QWidget * editor, const QModelIndex & index) const{
    delete editor;
}

void vLineEditDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const{

    QString keyIndex = model->index( index.row(), 2, index.parent()).data().toString();

    if (index.parent().row()==0 && index.row() ==1){
        QLineEdit *le = (QLineEdit*) editor;
        model->setData(index, QVariant(le->text()));
        currentProject->setSourceParam(keyIndex, le->text());
        return;
    }
    if (index.parent().row()==0 && index.row() ==2){
        QCheckBox *cb = (QCheckBox*) editor;
        model->setData(index, QVariant(cb->isCheckable()));
        return;
    }
    if(index.parent().row() == 2){
        QSpinBox *sb = (QSpinBox*) editor;
        model->setData(index, QVariant(sb->value()));
        currentProject->setSourceParam(keyIndex, QString::number(sb->value()));
        return;
    }

    vChooseDirectory* vcd = (vChooseDirectory*) editor;
    model->setData(index, QVariant(vcd->getText()));

    //Записыаем соответствующее значение в БД
    currentProject->setSourceParam(keyIndex, vcd->getText());

}

void vLineEditDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const{
    if (index.parent().row()==0 && index.row() ==1){
        QLineEdit *le = (QLineEdit*) editor;
        le->setText(index.data().toString());
        return;
    }

    if (index.parent().row()==0 && index.row() ==2){
        QCheckBox *cb = (QCheckBox*) editor;
        cb->setCheckable(index.data().toBool());
        return;
    }
    if(index.parent().row() == 2){
        QSpinBox *sb = (QSpinBox*) editor;
        sb->setValue(index.data().toInt());
        return;
    }

    vChooseDirectory* vcd = (vChooseDirectory*) editor;
    vcd->setText(index.data().toString());

}

