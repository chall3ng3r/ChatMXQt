#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
//
#include <QWidget>
#include <QTimer>
#include "networkapi.h"

namespace Ui {
    class ChatView;
}

class ChatView : public QWidget
{
    Q_OBJECT

public:
    explicit ChatView(QWidget *parent = 0);
    ~ChatView();

    void resetUI();
    void setNick(QString *nick);
    void init(NetworkApi *networkApi);

signals:
    void logoutEvent(QString *nick);

public slots:
    void sendMessage();
    void loadMessages(QString *xmlData);
    void logoutUser();
    void loadUsers(QString *xmlData);
    //void networkReply(QNetworkReply*);

private slots:
    void timerEvent();

protected:
    void showEvent(QShowEvent *);

private:
    Ui::ChatView *ui;
    QString Nick;
    QString lastTime;

    QTimer *apiTimer;
    NetworkApi *networkApi;
    //
    QNetworkAccessManager *nam;
};

#endif // CHATVIEW_H
