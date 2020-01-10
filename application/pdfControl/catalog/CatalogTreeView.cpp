/*
 * Copyright (C) 2019 ~ 2020 UOS Technology Co., Ltd.
 *
 * Author:     wangzhxiaun
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
#include "CatalogTreeView.h"

#include <QHeaderView>
#include <DFontSizeManager>

#include "subjectObserver/MsgHeader.h"
#include "controller/NotifySubject.h"

#include "frame/DocummentFileHelper.h"

#include "CatalogDelegate.h"

#include <QDebug>

CatalogTreeView::CatalogTreeView(DWidget *parent)
    : DTreeView(parent)
{
    setFrameShape(QFrame::NoFrame);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    this->header()->setHidden(true);
    this->viewport()->setAutoFillBackground(false);

    this->setContentsMargins(0, 0, 0, 0);

    this->setItemDelegate(new CatalogDelegate(this));   //  代理进行绘制

    initConnections();

    DFontSizeManager::instance()->bind(this, DFontSizeManager::T8);

    m_pSubjectThread = g_NotifySubject::getInstance();
    if (m_pSubjectThread) {
        m_pSubjectThread->addObserver(this);
    }
}

CatalogTreeView::~CatalogTreeView()
{
    if (m_pSubjectThread) {
        m_pSubjectThread->removeObserver(this);
    }
}

int CatalogTreeView::dealWithData(const int &msgType, const QString &)
{
    if (msgType == MSG_OPERATION_OPEN_FILE_OK) {
        emit sigOpenFileOk();
    }
    return 0;
}

void CatalogTreeView::sendMsg(const int &, const QString &)
{

}

void CatalogTreeView::notifyMsg(const int &, const QString &)
{

}

void CatalogTreeView::initConnections()
{
    connect(this, SIGNAL(sigOpenFileOk()), SLOT(SlotOpenFileOk()));
    connect(this, SIGNAL(clicked(const QModelIndex &)), SLOT(SlotClicked(const QModelIndex &)));
}

void CatalogTreeView::parseCatalogData(const Section &ol, QStandardItem *parentItem)
{
    foreach (auto s, ol.children) {

        auto itemList = getItemList(s.title, s.link.page);

        parentItem->appendRow(itemList);

        parseCatalogData(s, itemList.at(0));
    }
}

//  获取 一行的 三列数据
QList<QStandardItem *> CatalogTreeView::getItemList(const QString &title, const int &page)
{
    auto item = new QStandardItem(title);
    item->setData(page);

    item->setTextAlignment(Qt::AlignLeft);

    auto item1 = new QStandardItem();
    item1->setData(page);

    auto item2 = new QStandardItem(QString::number(page));
    item2->setData(page);

    item2->setTextAlignment(Qt::AlignRight);

    return QList<QStandardItem *>() << item << item1 << item2;
}

void CatalogTreeView::SlotOpenFileOk()
{
    auto model = reinterpret_cast<QStandardItemModel *>(this->model());
    if (model) {
        model->clear();

        Outline ol = DocummentFileHelper::instance()->outline();
        foreach (const Section &s, ol) {
            auto itemList = getItemList(s.title, s.link.page);

            model->appendRow(itemList);

            parseCatalogData(s, itemList.at(0));
        }
    }
}

//  点击 任一行, 实现 跳转页面
void CatalogTreeView::SlotClicked(const QModelIndex &index)
{
    int nPage = index.data(Qt::UserRole + 1).toInt();
    int nCurPage = DocummentFileHelper::instance()->currentPageNo();
    if (nPage != nCurPage) {
        DocummentFileHelper::instance()->pageJump(nPage);
    }
}
