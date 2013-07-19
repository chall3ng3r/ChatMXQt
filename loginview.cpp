#include "loginview.h"
#include "ui_loginview.h"
#include "networkapi.h"

#include <QPushButton>

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

LoginView::LoginView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginView)
{
    ui->setupUi(this);

    textNick = new LineEdit(parent);
    textNick->setMinimumHeight(50);

    ui->verticalLayout->insertWidget(2, textNick);
	textNick->setFocus();

    connect(ui->buttonLogin, SIGNAL(clicked()), SLOT(tryLogin()));
    connect(ui->buttonExit, SIGNAL(clicked()), SLOT(exitApp()));
    connect(textNick, SIGNAL(returnPressed()), this, SLOT(tryLogin()));
}

void LoginView::exitApp()
{
    QApplication::quit();
}

void LoginView::init(NetworkApi *netApi)
{
    networkApi = netApi;
    connect(networkApi, SIGNAL(commError(QString*)), this, SLOT(processError(QString*)));
}

// login into chat service
void LoginView::tryLogin()
{
    ui->labelStatus->setText("Connecting...");

    // process login
    if(textNick->text().count() <= 0)
    {
        ui->labelStatus->setText("Please enter nick.");
        textNick->setFocus();
        return;
    }

	// disable button to prevent further requests until response
	ui->buttonLogin->setDisabled(true);

    ui->labelStatus->setText(textNick->text().append(" connecting..."));

	// setup response handler, and make request
    connect(networkApi, SIGNAL(usersReceived(QString*)), this, SLOT(processLogin(QString*)));
    networkApi->loginUser(&textNick->text());
}

void LoginView::processLogin(QString *xmlData)
{
	// make first call to load messages
	networkApi->getMessages(&QString(""));

    // disconnect after first call
    disconnect(networkApi, SIGNAL(usersReceived(QString*)), this, SLOT(processLogin(QString*)));

    // issue login complete signal
    emit loginComplete(&textNick->text());
}

void LoginView::processError(QString *errorData)
{
	ui->buttonLogin->setEnabled(true);

    if(!this->isHidden())
    {
        ui->labelStatus->setText(*errorData);
    }
}

void LoginView::resetUI()
{
	ui->buttonLogin->setEnabled(true);
    ui->labelStatus->clear();
    textNick->clear();
    textNick->setFocus();
}


void LoginView::focusInEvent(QFocusEvent *event)
{
    textNick->setFocus();
    QWidget::focusInEvent(event);
}

LoginView::~LoginView()
{
    delete ui;
}
