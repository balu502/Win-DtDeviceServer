# HPOS return 0
# comment 352  396 1113 1628 1649
# uncoment 396 1113 1628 (21.03.22)
#-------------------------------------------------
#
# Project created by QtCreator 2016-02-04T15:59:10
#
#-------------------------------------------------

QT       += core gui
QT       += network

VERSION = 3.00.00
COMPATIBLE_VERSION = 7.30
DEFINES += MIN_VERSION=\\\"$$COMPATIBLE_VERSION\\\"

#version 1.00.00 begin
#version 1.00.01 change error processing
#1.00.05 new state machine processing + add Linguist
#1.00.07 change error processing model
#1.00.09 small error was removed
# press cover add, barcode read procedure in close cover procedure add.
#1.00.10 video online read function add
#1.00.11 add measure size of tube
#1.00.11 10.12.19 RSAV DRAV call change. Stay only for 96 and 384 type of device in
#USBCy_RW() was
#          if(answer.count() >= 3 && answer[2] == '>') { answer.remove(0,3); sts = true; if(answer[0]=='?') {sts=false; res=-4; break;}}
#          now
#          if(answer.count() >= 3 && answer[2] == '>') { answer.remove(0,3); sts = true; if(answer[0]=='?') {sts=false; res=-4; break;}}
#          else {
#             if(answer[0]=='?') {sts=false; res=-4; break;} else  sts = true;
#          }
# 23.01.20 was error read from device with chanel<Device chanel
# dtBehav.cpp
# add
#        active_ch=getActCh;  // 23.01.20
# 22.06.20 Program name in Russian was wrong on display
# change dtBehav.cpp
# was strncpy(protBuf.ProtocolSec0_96.num_protocol,map_Run.value(run_name).toAscii(),10);
# change on strncpy(protBuf.ProtocolSec0_96.num_protocol,map_Run.value(run_name).toLocal8Bit(),10);
# 24.09.2020
# New log processing. New release after error when write to log file.
# In logobject.cpp remove property QIODevice::WriteOnly in open function.
# In logMessage methode place while(log->isOpen())QApplication::processEvents(); and canWrite variable for
# right archive data
# 19.10.20
# Stop device operation when server don't connected to DtMaster
# 26.10.20
# log file error remove
# 1_00_15 12.11.20 Add get video files after RUN
# 1_00_16 20.11.20 Add reagent contract function
# 03.12.20 change logobject.cpp add date&time information in all strings
# 15.01.21 Going on new Qt version 5.15.2
# 2.00.00
# new network processing. Use emit signal inside thread
# 2.00.01 17.02.2021
# Change QThread on QObject in class TDtBehav
# 29.03.21 Add readTemperatureProgram(void) function for read Last Runprocedure on client side
# 2.10.00 30.03.2021
# remove sleep in ready state on waite condition
# 2.10.01 17.05.21
# comment line  nextPhase=READY; in dtBehav.cpp in case DEVICE_ERROR_STATE
# длительные операции не заканчивались OPEN CLOSE block если были ошибки при выполнении команды execcmd
# 2.10.01 25.05.21
# CRYC separated request was created
# 3.00.00 03.08.21
# New optical controller (V4.0) support

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
greaterThan(QT_MAJOR_VERSION, 4) : QT += widgets

MOC_DIR = ../moc
OBJECTS_DIR = ../obj
include (config.pro)

TARGET = Server_Dev
TEMPLATE = app


LIBS += -lsetupapi \
        -lwinmm

SOURCES += main.cpp\
          CypressUsb.cpp \
          srvBehav.cpp \
          dtBehav.cpp \
          pages.cpp \
          logobject.cpp \
          CANChannels.cpp \
          progerror.cpp

HEADERS +=CypressUsb.h \
          srvBehav.h \
          dtBehav.h \
          pages.h \
          logobject.h \
          progerror.h \
          can_ids.h \
          code_errors.h \
          CANChannels.h \
          digres.h \
          request_dev.h

RESOURCES += \
    graph.qrc

DESTDIR = ../$${CURRENT_BUILD}

TRANSLATIONS +=Server_Dev_ru.ts



















