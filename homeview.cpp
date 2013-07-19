#include "homeview.h"

#include <QtCore>
#include <QtGui>

class LineEdit: public QLineEdit
{
public:
    LineEdit(QWidget *parent = 0): QLineEdit(parent) {}

    void paintEvent(QPaintEvent *event) {
        QLineEdit::paintEvent(event);
        if (text().isEmpty()) {
            QPainter p(this);
            int flags = Qt::AlignLeft | Qt::AlignVCenter;
            p.setPen(palette().color(QPalette::Disabled, QPalette::Text));
            p.drawText(rect().adjusted(10, 0, 0, 0), flags, "Enter your nick");
            p.end();
        }
    }
};

HomeView::HomeView(QWidget *parent)
    : QWidget(parent)
{
    vLayout = new QVBoxLayout(parent);
    //vLayout->setGeometry(parent->geometry());

    textNick = new LineEdit(parent);
    textNick->setMinimumHeight(50);
    
    buttonLogin = new QPushButton(parent);
    buttonLogin->setText("Login");
    buttonLogin->setFixedHeight(60);

    buttonExit = new QPushButton(parent);
    buttonExit->setText("Exit");
    buttonExit->setFixedHeight(60);

    connect(buttonLogin, SIGNAL(clicked()), SLOT(tryLogin()));
    connect(buttonExit, SIGNAL(clicked()), SLOT(exitApp()));
    connect(textNick, SIGNAL(returnPressed()), this, SLOT(tryLogin()));

    picTitle = new QWidget(parent);
    picTitle->resize(300, 400);
    picTitle->setAutoFillBackground(true);


    labelStatus = new QLabel(parent);
    labelStatus->setAlignment(Qt::AlignCenter);
    labelStatus->setMinimumHeight(50);
    labelStatus->setFrameStyle(2);

    QHBoxLayout *layoutButtons = new QHBoxLayout(this);

    layoutButtons->addWidget(buttonExit);
    layoutButtons->addWidget(buttonLogin);

    //vLayout->addWidget(buttonExit);
    vLayout->addWidget(picTitle, 100);
    vLayout->addWidget(labelStatus);
    vLayout->addWidget(textNick);
    //vLayout->addWidget(buttonLogin);
    vLayout->addLayout(layoutButtons);
    
    setLayout(vLayout);

}

void HomeView::exitApp()
{
    QApplication::quit();
}

void HomeView::init(NetworkApi *netApi)
{
    networkApi = netApi;

    connect(networkApi, SIGNAL(loginComplete()), this, SLOT(processLogin(QString*)));
    connect(networkApi, SIGNAL(commError(QString*)), this, SLOT(processError(QString*)));
    connect(networkApi, SIGNAL(usersReceived(QString*)), this, SLOT(processLogin(QString*)));
}

// login into chat service
void HomeView::tryLogin()
{
    labelStatus->setText("Connecting...");

    // process login
    if(textNick->text().count() <= 0)
    {
        labelStatus->setText("Please enter nick.");
        textNick->setFocus();
        return;
    }

    labelStatus->setText(textNick->text().append(" connecting..."));

    networkApi->loginUser(&textNick->text());
}

void HomeView::processLogin(QString *xmlData)
{
    if(!this->isHidden())
    {
        qDebug(QString("processLogin: ").append(xmlData).toAscii());

        // issue login complete signal
        emit loginComplete(textNick->text());
    }
}

void HomeView::processError(QString *errorData)
{
    if(!this->isHidden())
    {
        labelStatus->setText(*errorData);
    }
}

void HomeView::resetUI()
{
    labelStatus->clear();
    textNick->clear();
    textNick->setFocus();
}


void HomeView::focusInEvent(QFocusEvent *event)
{
    textNick->setFocus();
    QWidget::focusInEvent(event);
}




