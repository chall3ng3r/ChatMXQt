#ifndef QTCHATMX_H
#define QTCHATMX_H

#include <QtGui/QMainWindow>
//#include "homeview.h"
#include "chatview.h"
#include "loginview.h"
#include "networkapi.h"

class QtChatMX : public QMainWindow
{
    Q_OBJECT

public:
    QtChatMX(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~QtChatMX();

public slots:
    void showHomeView(QString *nick);
    void showChatView(QString *nick);

private:
    //HomeView *homeView;
    LoginView *homeView;
    ChatView *chatView;

    NetworkApi *networkApi;
   

};

#endif // QTCHATMX_H
