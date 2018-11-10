TARGET = qlog_tcp_server
TEMPLATE = app

CONFIG += c++14

CONFIG += skip_target_version_ext

CONFIG -= debug_and_release
CONFIG(debug, debug|release) {
	BUILD_TYPE = debug
}
CONFIG(release, debug|release) {
	BUILD_TYPE = release
}

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
QMAKE_LFLAGS_CONSOLE = /SUBSYSTEM:CONSOLE,5.01

OBJECTS_DIR = $$OUT_PWD/obj
MOC_DIR = $$OUT_PWD/moc
RCC_DIR = $$OUT_PWD/qrc
UI_DIR = $$OUT_PWD/ui

DESTDIR = $$PWD/../bin/$$BUILD_TYPE

QT += network widgets

SOURCES += main.cpp\
	DataView.cpp \
	MainWnd.cpp \
	VarItem.cpp \
	../pub/QLogData.cpp

HEADERS  += \
	DataView.h \
	MainWnd.h \
	VarItem.h

FORMS    += \
	DataView.ui \
	MainWnd.ui
