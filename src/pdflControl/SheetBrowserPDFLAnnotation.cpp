#include "SheetBrowserPDFLAnnotation.h"
#include "SheetBrowserPDFLItem.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

SheetBrowserPDFLAnnotation::SheetBrowserPDFLAnnotation(QGraphicsItem *parent, QRectF rect, deepin_reader::Annotation *annotation) : QGraphicsItem(parent),
    m_annotation(annotation), m_rect(rect), m_parent(parent)
{

}

void SheetBrowserPDFLAnnotation::setScaleFactorAndRotation(Dr::Rotation rotation)
{
    m_rotation = rotation;
    update();
}

int SheetBrowserPDFLAnnotation::annotationType()
{
    return m_annotation->type();
}

QString SheetBrowserPDFLAnnotation::annotationText()
{
    return m_annotation->contents();
}

QRectF SheetBrowserPDFLAnnotation::boundingRect() const
{
    return QRectF(m_rect.x() * m_parent->boundingRect().width(),
                  m_rect.y() * m_parent->boundingRect().height(),
                  m_rect.width() * m_parent->boundingRect().width(),
                  m_rect.height() * m_parent->boundingRect().height());
}

void SheetBrowserPDFLAnnotation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setBrush(QBrush(Qt::red));
    painter->setPen(Qt::NoPen);
    painter->setOpacity(0.5);
    painter->drawRect(option->rect.x(), option->rect.y(), option->rect.width(), option->rect.height());
}

deepin_reader::Annotation *SheetBrowserPDFLAnnotation::annotation()
{
    return m_annotation;
}

void SheetBrowserPDFLAnnotation::deleteMe()
{
    SheetBrowserPDFLItem *item = static_cast<SheetBrowserPDFLItem *>(m_parent);
    if (nullptr == item)
        return;

    item->removeAnnotation(m_annotation);
}