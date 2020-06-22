/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     zhangsong<zhangsong@uniontech.com>
*
* Maintainer: zhangsong<zhangsong@uniontech.com>
*
* Central(NaviPage ViewPage)
*
* CentralNavPage(openfile)
*
* CentralDocPage(DocTabbar DocSheets)
*
* DocSheet(SheetSidebar SheetBrowser document)
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
#ifndef RenderThreadPDFL_H
#define RenderThreadPDFL_H

#include <QThread>
#include <QMutex>
#include <QStack>
#include <QImage>

#include "Global.h"

class SheetBrowserPDFLItem;
struct RenderTaskPDFL {
    SheetBrowserPDFLItem *item = nullptr;
    double scaleFactor = 1.0;
    Dr::Rotation rotation = Dr::RotateBy0;
};

class RenderThreadPDFL : public QThread
{
    Q_OBJECT
public:
    explicit RenderThreadPDFL(QObject *parent = nullptr);

    ~RenderThreadPDFL();

    static void clearTask(SheetBrowserPDFLItem *item);

    static void appendTask(SheetBrowserPDFLItem *item, double scaleFactor, Dr::Rotation rotation);

    static void destroy();

    void run();

signals:
    void sigTaskFinished(SheetBrowserPDFLItem *item, QImage image, double scaleFactor, int rotation, QRect rect);

private slots:
    void onTaskFinished(SheetBrowserPDFLItem *item, QImage image, double scaleFactor, int rotation, QRect rect);

private:
    RenderTaskPDFL m_curTask;
    QStack<RenderTaskPDFL> m_tasks;
    QMutex m_mutex;
    bool m_quit = false;
    bool m_quitRender = false;
    static RenderThreadPDFL *instance;
};

#endif // RenderThreadPDFL_H
