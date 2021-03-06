/*
 * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     duanxiaohui<duanxiaohui@uniontech.com>
 *
 * Maintainer: duanxiaohui<duanxiaohui@uniontech.com>
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
#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include "Application.h"
#include "CustomMenu.h"
#include "Utils.h"

#include <DFontSizeManager>
#include <DGuiApplicationHelper>
#include <DPalette>
#include <DWidget>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

DWIDGET_USE_NAMESPACE

/**
 * @brief The CustomWidget class
 * @brief   封装 DWidget 和 观察者， 便于 继承 DWidget
 *          实现 相应的消息处理， 更关注于 对应的业务处理
 */

const int FIRST_LOAD_PAGES = 20;
const int LEFTMINWIDTH = 266;//226  >>  266
const int LEFTMAXWIDTH = 380;
const int LEFTNORMALWIDTH = 266;//226  >>  266

class CustomWidget : public DWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(CustomWidget)

public:
    explicit CustomWidget(DWidget *parent = nullptr);
    virtual ~CustomWidget();

public:
    /**
     * @brief CustomWidget::adaptWindowSize
     * 缩略图列表自适应视窗大小
     * @param scale  缩放因子 大于0的数
     */
    virtual void adaptWindowSize(const double &);

    /**
     * @brief CustomWidget::updateThumbnail
     * 高亮操作之后要跟换相应的缩略图
     * @param index 页码数，从0开始
     */
    virtual void updateThumbnail(const int &);

    /**
     * @brief CustomWidget::showMenu
     * 显示菜单,响应键盘菜单键
     */
    virtual void showMenu();

protected:
    /**
     * @brief initWidget
     * 初始化控件
     */
    virtual void initWidget();

    /**
     * @brief updateWidgetTheme
     * 主题变换更新
     */
    void updateWidgetTheme();

protected:
    bool bIshandOpenSuccess = false;
};

#endif  // CUSTOMWIDGET_H
