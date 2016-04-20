#include "wsearchitemdelegate.h"
#include <QDebug>
#include <QPainter>
#include <QAbstractTextDocumentLayout>

wsearchitemdelegate::wsearchitemdelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void wsearchitemdelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);

    painter->save();

    QTextDocument doc;
    doc.setHtml(options.text);

    options.text = "";
    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

    // shift text right to make icon visible
    QSize iconSize = options.icon.actualSize(options.rect.size());
    painter->translate(options.rect.left()+iconSize.width(), options.rect.top());
    QRect clip(0, 0, options.rect.width()+iconSize.width(), options.rect.height());

    //doc.drawContents(painter, clip);

    painter->setClipRect(clip);
    QAbstractTextDocumentLayout::PaintContext ctx;
    // set text color to red for selected item
    if (option.state & QStyle::State_Selected)
       ctx.palette.setColor(QPalette::Text, QColor("red"));
    ctx.clip = clip;
    doc.documentLayout()->draw(painter, ctx);

    painter->restore();

    /*
    if(option.state & QStyle::State_Selected)
    {
        painter->fillRect( option.rect, option.palette.highlight() );
    }

    painter->save();

    QTextDocument document;
    document.setTextWidth(option.rect.width());
    QVariant value = index.data(Qt::DisplayRole);
    if (value.isValid() && !value.isNull())
    {
        document.setHtml(value.toString());
        painter->translate(option.rect.topLeft());
        document.drawContents(painter);
    }

    painter->restore();
    */
}

// TableView need to create an Editor
// Create Editor when we construct MyDelegate
// and return the Editor
QWidget* wsearchitemdelegate::createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    int iCol = index.column();
    if( iCol == 1)
    {
        QTextEdit* editor = new QTextEdit(parent);
        return editor;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

// Then, we set the Editor
// Gets the data from Model and feeds the data to Editor
void wsearchitemdelegate::setEditorData(QWidget *editor,
                               const QModelIndex &index) const
{
    int iCol = index.column();
    if( iCol == 1)
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QTextEdit* te = static_cast<QTextEdit*>(editor);
        if (te)te->setHtml(value);
        return;
    }

    QStyledItemDelegate::setEditorData(editor, index);
}

// When we modify data, this model reflect the change
void wsearchitemdelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                              const QModelIndex &index) const
{
    int iCol = index.column();
    if (iCol == 1)
    {
        QTextEdit *te = static_cast<QTextEdit*>(editor);
        if(te)
            model->setData(index, te->toPlainText(), Qt::EditRole);
    }
}

// Give the SpinBox the info on size and location
void wsearchitemdelegate::updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
