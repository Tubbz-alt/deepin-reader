/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     duanxiaohui
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "FontMenu.h"
#include "DocSheet.h"
#include "ModuleHeader.h"
#include "MsgHeader.h"
#include "pdfControl/docview/docummentproxy.h"

FontMenu::FontMenu(DWidget *parent):
    CustomMenu(parent)
{
    initActions();
}

void FontMenu::readCurDocParam(DocSheet *sheet)
{
    m_sheet = sheet;
    if (m_sheet.isNull())
        return;

    m_pTwoPageAction->setChecked(m_sheet->operation().layoutMode == Dr::TwoPagesMode);
    m_pFiteWAction->setChecked(m_sheet->operation().scaleMode == Dr::FitToPageWidthMode);
    m_pFiteHAction->setChecked(m_sheet->operation().scaleMode == Dr::FitToPageHeightMode);

    m_bDoubPage = (m_sheet->operation().layoutMode == Dr::TwoPagesMode);

    int adaptat = m_sheet->operation().scaleMode;

    if (adaptat == Dr::FitToPageWidthMode) {
        m_bFiteW = true;
        m_bFiteH = false;
    } else if (adaptat == Dr::FitToPageHeightMode) {
        m_bFiteH = true;
        m_bFiteW = false;
    } else {
        m_bFiteW = false;
        m_bFiteH = false;
    }
}

/**
 * @brief FontMenu::slotTwoPage
 * 双页显示
 */
void FontMenu::slotTwoPage()
{
    if (m_sheet.isNull())
        return;

    m_bDoubPage = !m_bDoubPage;

    if (m_bDoubPage)
        m_sheet->setLayoutMode(Dr::TwoPagesMode);
    else
        m_sheet->setLayoutMode(Dr::SinglePageMode);

    if (!m_bDoubPage) {//如果切成单页，需要自适应宽度
        m_sheet->setScaleMode(Dr::FitToPageWidthMode);
    } else if (m_bDoubPage && m_sheet->operation().sidebarVisible) {                //从单页切双页，如果缩略图开着,则适应宽度
        m_sheet->setScaleMode(Dr::FitToPageWidthMode);
    } else if (qFuzzyCompare(m_sheet->operation().scaleFactor, 1.00)) {             //如果没有人为调整过，则适应
        m_sheet->setScaleMode(Dr::FitToPageWidthMode);
    } else if (m_bDoubPage && m_sheet->operation().scaleFactor > 1.01) {            //从单页切双页，原来比例大于100,则适应宽度
        m_sheet->setScaleMode(Dr::FitToPageWidthMode);
    } else if (Dr::FitToPageWidthMode == m_sheet->operation().scaleMode) {          //从单页切双页，原来比例大于100,则适应宽度
        m_sheet->setScaleMode(Dr::FitToPageWidthMode);
    } else
        m_sheet->setScaleMode(Dr::ScaleFactorMode);
}

/**
 * @brief FontMenu::slotFiteH
 *自适应高
 */
void FontMenu::slotFiteH()
{
    m_pFiteWAction->setChecked(false);
    m_bFiteW = false;
    m_bFiteH = !m_bFiteH;
    m_pFiteHAction->setChecked(m_bFiteH);

    setAppSetFiteHAndW();
}

/**
 * @brief FontMenu::slotFiteW
 * 自适应宽
 */
void FontMenu::slotFiteW()
{
    m_pFiteHAction->setChecked(false);
    m_bFiteH = false;
    m_bFiteW = !m_bFiteW;
    m_pFiteWAction->setChecked(m_bFiteW);

    setAppSetFiteHAndW();
}

/**
 * @brief FontMenu::slotRotateL
 * 左旋转
 */
void FontMenu::slotRotateL()
{
    if (m_sheet.isNull())
        return;

    m_sheet->rotateLeft();
}

/**
 * @brief FontMenu::slotRotateR
 * 右旋转
 */
void FontMenu::slotRotateR()
{
    if (m_sheet.isNull())
        return;

    m_sheet->rotateRight();
}

/**
 * @brief FontMenu::initMenu
 * 初始化Menu
 */
void FontMenu::initActions()
{
    m_pTwoPageAction = createAction(tr("Two-Page View"), SLOT(slotTwoPage()), true);
    m_pFiteHAction = createAction(tr("Fit Height"), SLOT(slotFiteH()), true);
    m_pFiteWAction = createAction(tr("Fit Width"), SLOT(slotFiteW()), true);
    this->addSeparator();
    m_pRotateLAction = createAction(tr("Rotate Left"), SLOT(slotRotateL()));
    m_pRotateRAction = createAction(tr("Rotate Right"), SLOT(slotRotateR()));
}

/**
 * @brief FontMenu::createAction   创建菜单action
 * @param objName                  action名称
 * @param member                   action所关联的响应函数
 * @param checked                  action是否有选中状态
 * @return                         当前action指针
 */
QAction *FontMenu::createAction(const QString &objName, const char *member, bool checked)
{
    auto action = new QAction(objName, this);
    if (action) {
        action->setObjectName(objName);
        action->setCheckable(checked);

        connect(action, SIGNAL(triggered()), this, member);

        this->addAction(action);

        return action;
    }
    return nullptr;
}

/**
 * @brief FontMenu::setAppSetFiteHAndW
 * 记录文档属性,下次加载时使用
 * 0:都不自适应  1:自适应宽  10:自适应高
 */
void FontMenu::setAppSetFiteHAndW()
{
    if (m_sheet.isNull())
        return;

    if (m_bFiteW) {
        m_sheet->setScaleMode(Dr::FitToPageWidthMode);
    } else if (m_bFiteH) {
        m_sheet->setScaleMode(Dr::FitToPageHeightMode);
    } else
        m_sheet->setScaleMode(Dr::ScaleFactorMode);
}
