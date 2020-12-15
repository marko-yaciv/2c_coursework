QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++20

RC_ICONS = img/graduation-hat.ico
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
    allmembers.cpp \
    data/account.cpp \
    data/discipline.cpp \
    data/exept.cpp \
    data/flparser.cpp \
    data/human.cpp \
    data/student.cpp \
    data/studyprocessdata.cpp \
    data/teacher.cpp \
    main.cpp \
    learnsystem.cpp \
    newdiscipline.cpp \
    studentdialog.cpp \
    teacherdialog.cpp

HEADERS += \
    allmembers.h \
    data/account.h \
    data/discipline.h \
    data/exept.h \
    data/flparser.h \
    data/human.h \
    data/student.h \
    data/studyprocessdata.h \
    data/teacher.h \
    learnsystem.h \
    newdiscipline.h \
    studentdialog.h \
    teacherdialog.h

FORMS += \
    allmembers.ui \
    learnsystem.ui \
    newdiscipline.ui \
    studentdialog.ui \
    teacherdialog.ui

TRANSLATIONS += \
    learn-system_uk_UA.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    datafiles/dcourse1.json \
    datafiles/dcourse2.json \
    datafiles/dcourse3.json \
    datafiles/dcourse4.json \
    datafiles/groups.json \
    datafiles/students.json \
    datafiles/studstudyproc.json \
    datafiles/teachers.json \
    datafiles/teachstudyproc.json
