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
#ifndef PAGINGWIDGET_H
#define PAGINGWIDGET_H

#include <DIconButton>
#include <DLineEdit>

#include "CustomControl/CustomWidget.h"
#include "CustomControl/CustomClickLabel.h"

/**
 * @brief The ThumbnailItemWidget class
 * @brief   显示的数字 比 实际的文档 多 1
 * @brief   跳转页窗体
 */

class PagingWidget : public CustomWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(PagingWidget)

public:
    explicit PagingWidget(CustomWidget *parent = nullptr);
    ~PagingWidget() Q_DECL_OVERRIDE;

signals:
    void sigDocFilePageChange(const QString &);
    void sigDocFileOpenOk();

    // IObserver interface
public:
    int dealWithData(const int &, const QString &) Q_DECL_OVERRIDE;

private slots:
    void slotPrePageBtnClicked();
    void slotNextPageBtnClicked();
    void slotUpdateTheme();
    void SlotDocFilePageChange(const QString &);
    void SlotDocFileOpenOk();
    void SlotJumpPageLineEditReturnPressed();

private:
    void initWidget() Q_DECL_OVERRIDE;
    void initConnections();
    void __SetBtnState(const int &currntPage, const int &totalPage);

    void __NormalChangePage();
    void __PageNumberJump();

private:
    DLabel              *m_pTotalPagesLab = nullptr;        // 当前文档总页数标签
    DLabel              *m_pCurrantPageLab = nullptr;       // 当前文档当前页码
    DIconButton         *m_pPrePageBtn = nullptr;           // 按钮 前一页
    DIconButton         *m_pNextPageBtn = nullptr;          // 按钮 后一页
    DLineEdit           *m_pJumpPageLineEdit = nullptr;     // 输入框 跳转页码
};

#endif // PAGINGWIDGET_H