#ifndef NETWORKAPI_H
#define NETWORKAPI_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>

class NetworkApi : public QObject
{
    Q_OBJECT

public:
    NetworkApi(QObject *parent = 0);

public slots:
    void loginUser(const QString *nick);
    void logoutUser(const QString *nick);
    void getMessages(const QString *lastTime);
    void getUsers();
    void postMessage(QString *nick, QString *nessage);

signals:
    void loginComplete(QString*);
    void messagesReceived(QString*);
    void usersReceived(QString*);
    void messagePosted(QString*);
    void commError(QString*);

private slots:
    void handleNetworkReply(QNetworkReply*);

private:
    QNetworkAccessManager *nam;

};

#endif // NETWORKAPI_H
