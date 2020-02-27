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
#include "SearchItemWidget.h"
#include <DApplication>
#include <DApplicationHelper>
#include <QClipboard>
#include <QTextLayout>

#include "utils/utils.h"

SearchItemWidget::SearchItemWidget(DWidget *parent)
    : CustomItemWidget(QString("SearchItemWidget"), parent)
{
    initWidget();

    connect(this, SIGNAL(sigUpdateTheme()), this, SLOT(slotUpdateTheme()));

    dApp->m_pModelService->addObserver(this);
}

SearchItemWidget::~SearchItemWidget()
{
    dApp->m_pModelService->removeObserver(this);
}

//  搜索显示内容
void SearchItemWidget::setTextEditText(const QString &contant)
{
    m_strNote = contant;
    if (m_pTextLab) {
        m_pTextLab->clear();
        m_pTextLab->setText(contant);
    }
}

//  搜索結果
void SearchItemWidget::setSerchResultText(const QString &result)
{
    if (m_pSearchResultNum) {
        m_pSearchResultNum->setText(result);
    }
}

bool SearchItemWidget::bSelect()
{
    if (m_pPicture) {
        return m_pPicture->bSelect();
    }
    return false;
}

void SearchItemWidget::setBSelect(const bool &paint)
{
    if (m_pPicture) {
        m_pPicture->setSelect(paint);
    }
    m_bPaint = paint;
    update();
}

void SearchItemWidget::slotUpdateTheme()
{
    if (m_pPageNumber) {
        m_pPageNumber->setForegroundRole(DPalette::TextTitle);
    }
    if (m_pTextLab) {
        m_pTextLab->setForegroundRole(QPalette::BrightText);
    }
    if (m_pSearchResultNum) {
        m_pSearchResultNum->setForegroundRole(DPalette::TextTips);
    }
}

void SearchItemWidget::initWidget()
{
    auto t_vLayoutPicture = new QVBoxLayout;
    t_vLayoutPicture->setContentsMargins(0, 3, 0, 0);
    m_pPicture = new ImageLabel(this);
    m_pPicture->setFixedSize(QSize(48, 68));
    m_pPicture->setAlignment(Qt::AlignCenter);
    t_vLayoutPicture->addWidget(m_pPicture);
    t_vLayoutPicture->addStretch(1);

    m_pPageNumber = new PageNumberLabel(this);
    m_pPageNumber->setMinimumWidth(31);
    m_pPageNumber->setFixedHeight(18);
    m_pPageNumber->setForegroundRole(DPalette::WindowText);
    DFontSizeManager::instance()->bind(m_pPageNumber, DFontSizeManager::T8);

    m_pSearchResultNum = new DLabel(this);
    m_pSearchResultNum->setMinimumWidth(31);
    m_pSearchResultNum->setFixedHeight(18);
    m_pSearchResultNum->setForegroundRole(DPalette::TextTips);
    DFontSizeManager::instance()->bind(m_pSearchResultNum, DFontSizeManager::T10);

    m_pTextLab = new DLabel(this);
    m_pTextLab->setTextFormat(Qt::PlainText);
    m_pTextLab->setFixedHeight(54);
    m_pTextLab->setMinimumWidth(80);
    //    m_pTextLab->setMaximumWidth(349);
    m_pTextLab->setFrameStyle(QFrame::NoFrame);
    m_pTextLab->setWordWrap(true);
    m_pTextLab->setAlignment(Qt::AlignLeft);
    m_pTextLab->setAlignment(Qt::AlignTop);
    m_pTextLab->setForegroundRole(DPalette::BrightText);
    DFontSizeManager::instance()->bind(m_pTextLab, DFontSizeManager::T9);

    auto hLine = new DHorizontalLine(this);

    auto t_hLayout = new QHBoxLayout;
    t_hLayout->setContentsMargins(0, 0, 0, 0);
    t_hLayout->setSpacing(0);
    t_hLayout->addWidget(m_pPageNumber);
    t_hLayout->addWidget(m_pSearchResultNum);

    auto t_vLayout = new QVBoxLayout;
    t_vLayout->setContentsMargins(15, 0, 10, 0);
    t_vLayout->setSpacing(0);
    t_vLayout->addItem(t_hLayout);
    t_vLayout->addWidget(m_pTextLab);
    t_vLayout->addStretch(1);
    t_vLayout->addWidget(hLine);

    auto m_pHLayout = new QHBoxLayout;

    m_pHLayout->setSpacing(1);
    m_pHLayout->setContentsMargins(0, 0, 10, 0);
//    m_pHLayout->addWidget(m_pPicture);
    m_pHLayout->addItem(t_vLayoutPicture);
    m_pHLayout->addItem(t_vLayout);
    m_pHLayout->setSpacing(1);

    this->setLayout(m_pHLayout);
}

int SearchItemWidget::dealWithData(const int &msgType, const QString &)
{
    if (msgType == MSG_OPERATION_UPDATE_THEME) {
        emit sigUpdateTheme();
    }
    return 0;
}

void SearchItemWidget::paintEvent(QPaintEvent *e)
{
    CustomItemWidget::paintEvent(e);

    if (m_pTextLab) {
        QString note = m_strNote;

        note.replace(QChar('\n'), QString(""));
        note.replace(QChar('\t'), QString(""));
//        note.replace(QChar(' '), QString(""));
        m_pTextLab->setText(calcText(m_pTextLab->font(), note, m_pTextLab->size()));
    }

    //  涉及到 主题颜色
    if (m_bPaint) {
        m_pPicture->setForegroundRole(DPalette::Highlight);
    } else {
        m_pPicture->setForegroundRole(QPalette::Shadow);
    }
}

QString SearchItemWidget::calcText(const QFont &font, const QString &note,
                                   const QSize &size /*const int MaxWidth*/)
{
#if 0
    QString text = note;

    QFontMetrics fontWidth(font);
    int width = fontWidth.width(note);  //计算字符串宽度
    if (width >= size.width()) { //当字符串宽度大于最大宽度时进行转换
        text = fontWidth.elidedText(text, Qt::ElideRight, size.width()); //右部显示省略号
    }
    return text;   //返回处理后的字符串
#endif

#if 1
    QString text;
    QString tempText;
    int totalHeight = size.height();
    int lineWidth = size.width() - 12;

    QFontMetrics fm(font);

    int calcHeight = 0;
    int lineHeight = fm.height();
    int lineSpace = 0;
    int lineCount = (totalHeight - lineSpace) / lineHeight;
    int prevLineCharIndex = 0;
    for (int charIndex = 0; charIndex < note.size() && lineCount >= 0; ++charIndex) {
        int fmWidth = fm.horizontalAdvance(tempText);
        if (fmWidth > lineWidth) {
            calcHeight += lineHeight /*+3*/;
            if (calcHeight + lineHeight > totalHeight) {
                QString endString = note.mid(prevLineCharIndex);
                const QString &endText = fm.elidedText(endString, Qt::ElideRight, size.width());
                text += endText;

                lineCount = 0;
                break;
            }

            QChar currChar = tempText.at(tempText.length() - 1);
            QChar nextChar = note.at(note.indexOf(tempText) + tempText.length());
            if (currChar.isLetter() && nextChar.isLetter()) {
                //                tempText += '-';
            }
            fmWidth = fm.horizontalAdvance(tempText);
            if (fmWidth > size.width()) {
                --charIndex;
                --prevLineCharIndex;
                tempText = tempText.remove(tempText.length() - 2, 1);
            }
            text += tempText;

            --lineCount;
            if (lineCount > 0) {
                text += "\n";
            }
            tempText = note.at(charIndex);

            prevLineCharIndex = charIndex;
        } else {
            tempText += note.at(charIndex);
        }
    }

    if (lineCount > 0) {
        text += tempText;
    }

    return text;
#endif
}