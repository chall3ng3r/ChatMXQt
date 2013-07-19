#include "chatview.h"
#include "ui_chatview.h"
#include <QTime>
#include <QScrollBar>

ChatView::ChatView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatView)
{
    ui->setupUi(this);

    apiTimer = new QTimer(this);
    apiTimer->setInterval(1000);

    // bind signals
    connect(ui->pushButtonLogout, SIGNAL(clicked()), this, SLOT(logoutUser()));
    connect(ui->pushButtonSend, SIGNAL(clicked()), this, SLOT(sendMessage()));
    connect(ui->lineEditChatBox, SIGNAL(returnPressed()), this, SLOT(sendMessage()));

    connect(apiTimer, SIGNAL(timeout()), this, SLOT(timerEvent()));

}

void ChatView::init(NetworkApi *netApi)
{
    networkApi = netApi;

    connect(networkApi, SIGNAL(messagesReceived(QString*)), this, SLOT(loadMessages(QString*)));
    connect(networkApi, SIGNAL(usersReceived(QString*)), this, SLOT(loadUsers(QString*)));
    connect(networkApi, SIGNAL(messagePosted(QString*)), this, SLOT(loadMessages(QString*)));
}

// beats every sec
void ChatView::timerEvent()
{
    int sec = QTime::currentTime().second();

    // poll messages every 5 secs
    if(sec % 5 == 0)
    {
//        if(lastTime.isEmpty())
//			lastTime = "";

        networkApi->getMessages(&lastTime);
    }
    if(sec % 10 == 0)
    {
        networkApi->getUsers();
    }
}


// set user's nick
void ChatView::setNick(QString *nick)
{
    Nick = *nick;
    ui->labelTitle->setText(*nick);
}

void ChatView::sendMessage()
{
    if(ui->lineEditChatBox->text().trimmed().length() > 0)
    {
		// stop until response arives
		apiTimer->stop();

        networkApi->postMessage(&Nick, &ui->lineEditChatBox->text());

        ui->lineEditChatBox->clear();
        ui->lineEditChatBox->setFocus();
    }
}


void ChatView::loadMessages(QString *xmlData)
{
    qDebug(QTime::currentTime().toString().append(" | loadMessages called").toUtf8());

	if(!apiTimer->isActive())
	{
		// resume timer
		apiTimer->start();
	}

    if(xmlData->isNull())
    {
        qDebug("loadMessages: no data");
        networkApi->getMessages(&lastTime);
        return;
    }

    // process XML
    QDomDocument xml("ChatMX_Response");
    xml.setContent(*xmlData);

    QDomElement rootNode = xml.documentElement();

    if(rootNode.tagName() == "Messages")
    {
        if(rootNode.firstChild().childNodes().count() == 0)
            return;

        QDomNode node = rootNode.firstChild().firstChild();
        QString strMessages;
        while(!node.isNull())
        {
            strMessages.append(node.attributes().namedItem("nick").nodeValue()).append(": ");
            strMessages.append(node.attributes().namedItem("message").nodeValue()).append("\n");
            lastTime = node.attributes().namedItem("date_time").nodeValue();
            node = node.nextSibling();
        }

        ui->textEditMessages->append(strMessages.left(strMessages.length() - 1));

        QScrollBar *sb = ui->textEditMessages->verticalScrollBar();
        sb->setValue(sb->maximum());
    }
}

void ChatView::loadUsers(QString *xmlData)
{
    // process XML
    QDomDocument xml("ChatMX_Response");
    xml.setContent(*xmlData);

    QDomElement rootNode = xml.documentElement();

    if(rootNode.tagName() == "Users")
    {
        QDomNode node = rootNode.firstChild().firstChild();
        QString strUsers;
        while(!node.isNull())
        {
            //qDebug(QString("node: ").append(node.nodeName()).toAscii());
            strUsers.append("::").append(node.attributes().namedItem("nick").nodeValue()).append("\n");
            node = node.nextSibling();
        }

        ui->textEditUsers->setText(strUsers);
    }
}

void ChatView::logoutUser()
{
    networkApi->logoutUser(&Nick);

    emit logoutEvent(&Nick);
    apiTimer->stop();
}


void ChatView::resetUI()
{
	lastTime = "";
    ui->textEditMessages->clear();
    ui->textEditUsers->clear();
    ui->lineEditChatBox->clear();
    ui->labelTitle->clear();
}

void ChatView::showEvent(QShowEvent *)
{
    apiTimer->start();
    ui->lineEditChatBox->setFocus();
}

ChatView::~ChatView()
{
    delete ui;
}
