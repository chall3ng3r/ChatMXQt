#ifndef HOMEVIEW_H
#define HOMEVIEW_H

#include <QWidget>
#include <QtGui>
#include "networkapi.h"

class HomeView : public QWidget
{
    Q_OBJECT

public:
    HomeView(QWidget *parent);
    void resetUI();
    void init(NetworkApi *networkApi);

signals:
    void loginComplete(const QString &nick);

private slots:
    void tryLogin();
    void exitApp();
    void processLogin(QString *xmlData);
    void processError(QString *errorData);

protected:
    void focusInEvent(QFocusEvent *event);

private:
    QPushButton *buttonLogin;
    QPushButton *buttonExit;
    QLineEdit   *textNick;
    QWidget     *picTitle;
    QLabel      *labelStatus;

    QVBoxLayout *vLayout;
    NetworkApi *networkApi;

};

#endif // HOMEVIEW_H

