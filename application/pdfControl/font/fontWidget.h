﻿#ifndef FONTWIDGET_H
#define FONTWIDGET_H

#include <DLabel>
#include <DSlider>

#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QPixmap>
#include <QPainter>
#include <QRectF>
#include <QtMath>

#include "translator/Frame.h"
#include "subjectObserver/CustomWidget.h"
#include "docview/docummentproxy.h"

class MenuLab : public DLabel
{
    Q_OBJECT
public:
    MenuLab(QWidget *parent = nullptr);

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

class FontWidget : public CustomWidget
{
    Q_OBJECT
public:
    FontWidget(CustomWidget *parent = nullptr);

signals:
    void sigWidgetHide();

public:
    int dealWithData(const int &, const QString &) override;

protected:
    void initWidget() override;

protected:
    void  paintEvent(QPaintEvent *e) override;
    void  hideEvent(QHideEvent *event) override;

private:
    void rotateFileView(bool isRight = true);
    void scaleAndRotate(int);
    void setShowSuitHIcon();
    void setShowSuitWIcon();

private slots:
    void slotSetChangeVal(int);
    void slotSetDoubPageViewCheckIcon();
    void slotSetSuitHCheckIcon();
    void slotSetSuitWCheckIcon();
    void slotSetRotateLeftCheckIcon();
    void slotSetRotateRightCheckIcon();

private:
    DLabel *m_pEnlargeLab = nullptr;
    DSlider *m_pEnlargeSlider = nullptr;

    MenuLab *m_pDoubPageViewLb = nullptr;
    MenuLab *m_pSuitHLb = nullptr;
    MenuLab *m_pSuitWLb = nullptr;
    MenuLab *m_pRotateLeftLb = nullptr;
    MenuLab *m_pRotateRightLb = nullptr;

    DLabel *m_pDoubPageViewLab = nullptr;
    DLabel *m_pSuitHLab = nullptr;
    DLabel *m_pSuitWLab = nullptr;
    DLabel *m_pRotateLeftLab = nullptr;
    DLabel *m_pRotateRightLab = nullptr;
    int m_rotate = 0;  // 旋转角度
    int m_rotateType = RotateType_Normal;
    bool m_bSuitH = false;
    bool m_bSuitW = false;
};

#endif // FONTWIDGET_H
