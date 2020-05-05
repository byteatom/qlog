TARGET = qlog
TEMPLATE = lib

VERSION = 1.0.0.0

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

QT += network

DEFINES += QLOG_LIB

msvc {
	#QMAKE_CXXFLAGS_EXCEPTIONS_ON = /EHa
	#QMAKE_CXXFLAGS_STL_ON = /EHa

	QMAKE_CXXFLAGS_WARN_ON -= -w34100
	QMAKE_CFLAGS_WARN_ON -= -w34100

	QMAKE_CFLAGS += -we4715 -wd4100 -wd4101 -wd4102 -wd4189
	QMAKE_CXXFLAGS += -we4715 -wd4100 -wd4101 -wd4102 -wd4189

	QMAKE_TARGET_PRODUCT = $${TARGET}
	CONFIG(release, debug|release) {
		#PDB
		QMAKE_CXXFLAGS_RELEASE += /Zi /Oy-
		PDB_DIR = $$PWD/../pdb/$$VERSION
		mkpath($${PDB_DIR})
		QMAKE_LFLAGS_RELEASE += /DEBUG /PDB:"$${PDB_DIR}/$${TARGET}.pdb"
	}
}

SOURCES += \
	NetSink.cpp \
	FileSink.cpp \
	QLog.cpp \
	../pub/QLogData.cpp

HEADERS += \
	NetSink.h \
	FileSink.h \
	QLogDataPool.h \
	../inc/QLog.h \
	../pub/QLogData.h


