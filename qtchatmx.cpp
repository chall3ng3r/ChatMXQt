#include "qtchatmx.h"
#include <QtCore>
#include <QtGui>
#include <QPropertyAnimation>


QtChatMX::QtChatMX(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    homeView = new LoginView(this);
    chatView = new ChatView(this);
    networkApi = new NetworkApi(this);

    homeView->resize(640, 360);
	chatView->resize(640, 360);

    chatView->hide();

    connect(homeView, SIGNAL(loginComplete(QString*)), this, SLOT(showChatView(QString*)));
    connect(chatView, SIGNAL(logoutEvent(QString*)), this, SLOT(showHomeView(QString*)));

	chatView->init(networkApi);
	homeView->init(networkApi);

    homeView->show();
}


// switch to main chat view once the login in done
void QtChatMX::showChatView(QString *nick)
{
    chatView->setNick(nick);

	homeView->hide();
	chatView->show();
}

// switch to home view if the logout is pressed
void QtChatMX::showHomeView(QString *nick)
{
    //
	chatView->resetUI();
	chatView->hide();

	homeView->resetUI();
	homeView->show();
}

QtChatMX::~QtChatMX()
{

}
