#include "TitleWidget.h"
#include <QHBoxLayout>

#include <QWidgetAction>

TitleWidget::TitleWidget(CustomWidget *parent) :
    CustomWidget("TitleWidget", parent)
{
    initWidget();
    initConnections();
}

TitleWidget::~TitleWidget()
{

}

//  主题变了
void TitleWidget::slotUpdateTheme()
{
    auto btnList = this->findChildren<DIconButton *>();
    foreach (auto btn, btnList) {
        QString objName = btn->objectName();
        if (objName != "") {
            QString sPixmap = PF::getImagePath(objName, Pri::g_frame);
            btn->setIcon(QIcon( sPixmap));
        }
    }

    if (m_pHandleAction) {
        QString sPixmap = PF::getImagePath("handleShape_small", Pri::g_frame);
        m_pHandleAction->setIcon(QIcon( sPixmap));
    }

    if (m_pDefaultAction) {
        QString sPixmap = PF::getImagePath("defaultShape_small", Pri::g_frame);
        m_pDefaultAction->setIcon(QIcon(sPixmap));
    }
}

//  打开文件成功
void TitleWidget::slotOpenFileOk()
{
    m_pThumbnailBtn->setDisabled(false);
    m_pSettingBtn->setDisabled(false);
    m_pHandleShapeBtn->setDisabled(false);
    m_pMagnifierBtn->setDisabled(false);
}

// 应用全屏显示
void TitleWidget::slotAppFullScreen()
{
    m_pThumbnailBtn->setChecked(false);

    //  侧边栏 隐藏
    sendMsgToSubject(MSG_SLIDER_SHOW_STATE, "0");
}

//  退出放大鏡
void TitleWidget::slotMagnifierCancel()
{
    m_pMagnifierBtn->setChecked(false);
    sendMsgToSubject(MSG_MAGNIFYING, "0");
}

void TitleWidget::initWidget()
{
    initBtns();

    auto m_layout = new QHBoxLayout();
    m_layout->setContentsMargins(5, 0, 0, 0);
    m_layout->setSpacing(10);
    setLayout(m_layout);

    m_layout->addWidget(m_pThumbnailBtn);
    m_layout->addWidget(m_pSettingBtn);
    m_layout->addWidget(m_pHandleShapeBtn);
    m_layout->addWidget(m_pMagnifierBtn);
    m_layout->addStretch(1);
}

void TitleWidget::initConnections()
{
    connect(this, SIGNAL(sigOpenFileOk()), this, SLOT(slotOpenFileOk()));
    connect(this, SIGNAL(sigMagnifierCancel()), this, SLOT(slotMagnifierCancel()));
    connect(this, SIGNAL(sigAppFullScreen()), this, SLOT(slotAppFullScreen()));
    connect(this, &TitleWidget::sigUpdateTheme, &TitleWidget::slotUpdateTheme);
}

//  缩略图 显示
void TitleWidget::on_thumbnailBtn_clicked()
{
    bool rl = m_pThumbnailBtn->isChecked();
    sendMsgToSubject(MSG_SLIDER_SHOW_STATE, QString::number(rl));
}

//  字体
void TitleWidget::on_settingBtn_clicked()
{
    QPoint point = m_pSettingBtn->mapToGlobal(QPoint(0, 0));
    int nOldY = point.y();

    int nHeight = m_pSettingBtn->height();
    point.setY(nHeight + nOldY + 2);

    if (m_pSettingMenu == nullptr) {
        m_pSettingMenu = new DMenu(this);

        auto action = new QWidgetAction(this);

        auto scaleWidget = new FontWidget(this);
        action->setDefaultWidget(scaleWidget);

        m_pSettingMenu->addAction(action);
    }
    m_pSettingMenu->exec(point);
}

//  手型点击
void TitleWidget::on_handleShapeBtn_clicked()
{
    int nHeight = m_pHandleShapeBtn->height();

    QPoint point = m_pHandleShapeBtn->mapToGlobal(QPoint(0, 0));
    int nOldY = point.y();

    point.setY(nHeight + nOldY + 2);

    if (m_pHandleMenu == nullptr) {
        m_pHandleMenu = new DMenu(this);
        {
            m_pHandleAction = new QAction(tr("handleShape"), this);
            m_pHandleAction->setObjectName("handleShape");
            m_pHandleAction->setCheckable(true);
            QString sPixmap = PF::getImagePath("handleShape_small", Pri::g_frame);
            m_pHandleAction->setIcon(QIcon(sPixmap));
            m_pHandleMenu->addAction(m_pHandleAction);
        }
        {
            m_pDefaultAction = new QAction(tr("defaultShape"), this);
            m_pDefaultAction->setObjectName("defaultShape");
            m_pDefaultAction->setCheckable(true);
            m_pDefaultAction->setChecked(true);
            QString sPixmap = PF::getImagePath("defaultShape_small", Pri::g_frame);
            m_pDefaultAction->setIcon(QIcon(sPixmap));
            m_pHandleMenu->addAction(m_pDefaultAction);
        }
        QActionGroup *actionGroup = new QActionGroup(this);
        actionGroup->addAction(m_pHandleAction);
        actionGroup->addAction(m_pDefaultAction);

        connect(actionGroup, SIGNAL(triggered(QAction *)), this, SLOT(slotActionTrigger(QAction *)));
    }
    m_pHandleMenu->exec(point);
}

//  放大镜 功能
void TitleWidget::on_magnifyingBtn_clicked()
{
    bool bCheck = m_pMagnifierBtn->isChecked();
    sendMsgToSubject(MSG_MAGNIFYING, QString::number(bCheck));
}

void TitleWidget::slotActionTrigger(QAction *action)
{
    QString btnName = "";
    int nCurrentState = -1;

    QString sAction = action->objectName();
    if (sAction == "defaultShape") {
        nCurrentState = 0;
        btnName = "defaultShape";
    } else {
        nCurrentState = 1;
        btnName = "handleShape";
    }

    QString normalPic = PF::getImagePath(btnName, Pri::g_frame);
    m_pHandleShapeBtn->setIcon(QIcon(normalPic));

    sendMsgToSubject(MSG_HANDLESHAPE, QString::number(nCurrentState));
}

void TitleWidget::initBtns()
{
    m_pThumbnailBtn = createBtn("thumbnails", true);
    connect(m_pThumbnailBtn, SIGNAL(clicked()), this, SLOT(on_thumbnailBtn_clicked()));

    m_pSettingBtn = createBtn("setting");
    connect(m_pSettingBtn, SIGNAL(clicked()), this, SLOT(on_settingBtn_clicked()));

    m_pHandleShapeBtn = createBtn("defaultShape");
    m_pHandleShapeBtn->setFixedSize(QSize(42, 36));
    m_pHandleShapeBtn->setIconSize(QSize(42, 36));
    connect(m_pHandleShapeBtn, SIGNAL(clicked()), this, SLOT(on_handleShapeBtn_clicked()));

    m_pMagnifierBtn = createBtn("magnifier", true);
    connect(m_pMagnifierBtn, SIGNAL(clicked()), this, SLOT(on_magnifyingBtn_clicked()));
}

DIconButton *TitleWidget::createBtn(const QString &btnName, bool bCheckable)
{
    DIconButton *btn = new  DIconButton(this);
    btn->setObjectName(btnName);
    btn->setFixedSize(QSize(36, 36));
    btn->setIconSize(QSize(36, 36));  

    QString sPixmap = PF::getImagePath(btnName, Pri::g_frame);
    btn->setIcon(QIcon(sPixmap));

    btn->setToolTip(btnName);
    btn->setCheckable(bCheckable);

    if (bCheckable) {
        btn->setChecked(false);
    }

    btn->setDisabled(true);

    return btn;
}

void TitleWidget::sendMsgToSubject(const int &msgType, const QString &msgCotent)
{
    sendMsg(msgType, msgCotent);
}

//  处理 推送消息
int TitleWidget::dealWithData(const int &msgType, const QString &msgContent)
{
    if (msgType == MSG_OPERATION_OPEN_FILE_OK) {
        emit sigOpenFileOk();
    } else if (msgType == MSG_OPERATION_FULLSCREEN || msgType == MSG_OPERATION_SLIDE) {
        emit sigAppFullScreen();
    } else if (msgType == MSG_NOTIFY_KEY_MSG) {
        if (msgContent == "Esc") {      //  退出放大镜模式
            emit sigMagnifierCancel();
        }
    } else if (msgType == MSG_OPERATION_UPDATE_THEME) {
        emit sigUpdateTheme();
    }
    return 0;
}
