QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    apply_m.cpp \
    check_apply.cpp \
    hcheck_work.cpp \
    hr.cpp \
    hr_check.cpp \
    hr_manage.cpp \
    init_id.cpp \
    main.cpp \
    mainwindow.cpp \
    ph.cpp \
    ph_check.cpp \
    refinelist.cpp \
    sell_me.cpp \
    ware.cpp \
    ware_add.cpp \
    ware_apply.cpp \
    ware_check.cpp

HEADERS += \
    apply_m.h \
    check_apply.h \
    hcheck_work.h \
    hr.h \
    hr_check.h \
    hr_manage.h \
    init_id.h \
    mainwindow.h \
    ph.h \
    ph_check.h \
    refinelist.h \
    sell_me.h \
    ware.h \
    ware_add.h \
    ware_apply.h \
    ware_check.h

FORMS += \
    apply_m.ui \
    check_apply.ui \
    hcheck_work.ui \
    hr.ui \
    hr_check.ui \
    hr_manage.ui \
    init_id.ui \
    mainwindow.ui \
    ph.ui \
    ph_check.ui \
    refinelist.ui \
    sell_me.ui \
    ware.ui \
    ware_add.ui \
    ware_apply.ui \
    ware_check.ui

# Link ODBC libraries
win32:LIBS += -lodbc32
unix:LIBS += -lodbc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT += core widgets charts
