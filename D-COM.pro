QT += core gui
QT += network
QT += sql
QT += xlsx


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FundBalanceQuery.cpp \
    ImportOfDeclarations.cpp \
    MaskForm.cpp \
    MaximumAmountRegularDeclaration.cpp \
    RTGSEntryQuery.cpp \
    balance.cpp \
    dashboard.cpp \
    entering.cpp \
    globalData.cpp \
    main.cpp \
    record.cpp \
    test.cpp \
    toast.cpp \
    user.cpp \
    widget.cpp

HEADERS += \
    FundBalanceQuery.h \
    ImportOfDeclarations.h \
    MaskForm.h \
    MaximumAmountRegularDeclaration.h \
    RTGSEntryQuery.h \
    dashboard.h \
    globalData.h \
    test.h \
    toast.h \
    widget.h

FORMS += \
    FundBalanceQuery.ui \
    ImportOfDeclarations.ui \
    MaximumAmountRegularDeclaration.ui \
    RTGSEntryQuery.ui \
    dashboard.ui \
    globalData.ui \
    test.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    Data/RTGS录入查询.xlsx \
    Data/备付金余额查询.xlsx \
    Data/深圳最高额度常规申报.xlsx \
    Data/用户.xlsx
