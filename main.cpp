#include "qtchatmx.h"
#include <QtGui/QApplication>
#include <QtCore>
#include <QtGui>

// Lock S60 orientation
#ifdef Q_OS_SYMBIAN
#include <AknAppUi.h>
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef Q_OS_SYMBIAN
    // Lock S60 orientation
    // Req: LIBS += -lcone -leikcore -lavkon avkon cone eiksrv
    CAknAppUi* appUi = dynamic_cast<CAknAppUi*>(CEikonEnv::Static()->AppUi());
    if(appUi)
    {
        QT_TRAP_THROWING(appUi->SetOrientationL(CAknAppUi::EAppUiOrientationLandscape))
    }
#endif

    app.setApplicationName("ChatMX");
    app.setApplicationVersion("1.0.0");

    QtChatMX window;

#if defined(Q_WS_S60)
    window.showFullScreen();
#elif defined(Q_WS_SIMULATOR)
    window.showFullScreen();
#else
	window.show();
#endif

    return app.exec();
}
