#include "CustomLabel.h"

CustomLabel::CustomLabel(QWidget *parent) :
    DLabel(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

void CustomLabel::setSelect(const bool select)
{
    m_bSelect = select;
    update();
}

/**
 * @brief CustomLabel::paintEvent
 * 自绘承载缩略图标签
 * @param e
 */
void CustomLabel::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    DLabel::paintEvent(e);
    qreal local = 0;
    qreal width = 0;
    qreal heigh = 0;

    if(m_bSelect){
        local = 3;
        width = this->width() - 6;
        heigh = this->height() - 6;
    }else{
        local = 0;
        width = this->width() - 0;
        heigh = this->height() - 0;
    }
    QRectF rectangle(local, local, width, heigh);

    QPainter painter(this);
    painter.setOpacity(1);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::NoBrush);
//    painter.setPen(QPen(QColor(QString("#0081FF")), 3));
    painter.drawRoundedRect(rectangle, 8, 8);

}
