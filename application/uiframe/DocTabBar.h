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
#ifndef MAINTABBAR_H
#define MAINTABBAR_H

#include <DTabBar>
#include <DTabWidget>
#include <QPointer>

#include "application.h"
#include "IObserver.h"

DWIDGET_USE_NAMESPACE

class CentralDocPage;
class DocTabBar : public DTabBar, public IObserver
{
    Q_OBJECT
    Q_DISABLE_COPY(DocTabBar)

public:
    explicit DocTabBar(CentralDocPage *parent);

    ~DocTabBar() override;

    int indexOfFilePath(const QString &filePath);

protected:
    QMimeData *createMimeDataFromTab(int index, const QStyleOptionTab &option) const;

    void insertFromMimeDataOnDragEnter(int index, const QMimeData *source); //只是先生成一个tab,结束后自动删除

    void insertFromMimeData(int index, const QMimeData *source);            //完全DROP 需要添加tab并打开对应的文档

    bool canInsertFromMimeData(int index, const QMimeData *source) const;

    void handleDragActionChanged(Qt::DropAction action);

private slots:
    void handleTabReleased(int index);                                  //方法测试结果为当tab从bar移除释放

    void handleTabDroped(int index, Qt::DropAction da, QObject *target);//方法测试结果为当tab添加到其他的bar里释放

    void onDroped();

signals:
    void sigTabBarIndexChange(const QString &);

    void sigAddTab(const QString &);

    void sigCloseTab(const QString &);

    void sigFileCountChanged(int);

public:
    int dealWithData(const int &, const QString &) override;

    void notifyMsg(const int &, const QString &s = "") override;

private:
    void AddFileTab(const QString &, int index = -1);

    QString getFileName(const QString &strFilePath);

private slots:
    void SlotCurrentChanged(int);

    void SlotTabAddRequested();

    void SlotTabCloseRequested(int index);

    void SlotRemoveFileTab(const QString &);

    void SlotOpenFileResult(const QString &, const bool &);

private:
    QList <int> m_pMsgList;

    QPointer<CentralDocPage> m_docPage;
};

#endif // MAINTABWIDGET_H