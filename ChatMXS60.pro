#-------------------------------------------------
#
# Project created by QtCreator 2010-12-01T20:36:20
#
#-------------------------------------------------

QT       += core gui network xml

TARGET = ChatMXS60
TEMPLATE = app


SOURCES += main.cpp\
    qtchatmx.cpp \
    chatview.cpp \
    networkapi.cpp \
    loginview.cpp

HEADERS  += \
    qtchatmx.h \
    chatview.h \
    networkapi.h \
    loginview.h

FORMS    += \
    chatview.ui \
    loginview.ui

CONFIG += mobility
MOBILITY = 

symbian {
    LIBS += -lcone -lavkon -lefsrv
	ICON = chatmx_s60_icon2.svg
	TARGET.UID3 = 0xea2479c5
    TARGET.CAPABILITY += NetworkServices
    TARGET.EPOCSTACKSIZE = 0x14000
	TARGET.EPOCHEAPSIZE = 0x20000 0x2000000
}

OTHER_FILES += \
    chatmx_s60_icon2.svg

RESOURCES += \
    MainGfx.qrc
