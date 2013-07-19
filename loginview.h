#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>
#include <QtGui>
#include <QtCore>
#include "networkapi.h"

namespace Ui {
    class LoginView;
}

class LoginView : public QWidget
{
    Q_OBJECT

public:
    void resetUI();
    void init(NetworkApi *networkApi);
    //
    explicit LoginView(QWidget *parent = 0);
    ~LoginView();

signals:
    void loginComplete(QString *nick);

private slots:
    void tryLogin();
    void exitApp();
    void processLogin(QString *xmlData);
    void processError(QString *errorData);

protected:
    void focusInEvent(QFocusEvent *event);

private:
    Ui::LoginView *ui;
    QLineEdit  *textNick;
    NetworkApi *networkApi;
};

#endif // LOGINVIEW_H
