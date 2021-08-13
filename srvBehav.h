#ifndef SRVBEHAV_H
#define SRVBEHAV_H

#include <QtWidgets>

#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QAction>
#include <QMessageBox>
#include <QEvent>
#include <QCloseEvent>
#include <QTimer>
#include <QLabel>
#include <QDebug>

#include "pages.h"
#include "logobject.h"
#include "dtBehav.h"

#define TERMINATE_EV 3302

class TServer_Dt : public QMainWindow
{
    Q_OBJECT

public:
  TServer_Dt(QString dName = "A5UN01", int nPort = 9001, QWidget *parent = 0);
  ~TServer_Dt();

  void IniFileRead(void);
  void IniFileWrite(void);
  void setupUI(void);
  void setLogEnable(void);
  void run(void);

protected:
  void closeEvent(QCloseEvent *event); //Q_DECL_OVERRIDE;
  bool nativeEvent(const QByteArray&, void*, long* );


private slots:        
  void iconActivated(QSystemTrayIcon::ActivationReason reason);
  void setTrayIconActions();
  void showTrayIcon();
  void getUI(void);
  void hideProg(void);
  void showProg(void);
  void quitProg(void);

// private variables
private:
  QMessageBox message;
  //QTimer dev_timer;
  TDtBehav *device;
  LogObject *logSys;

  QSystemTrayIcon *trayIcon;
  QMenu   *trayIconMenu;
  QAction *minimizeAction;
  QAction *restoreAction;
  QAction *quitAction;

// var for check box seting in pages UI
  bool logSrvScr,logSrv,logNWScr,logNW,logDTScr,logDT,logDTM;

  QString setingsFileName;
  int log_Size,log_Count; // size of log file and count zips archives get from settings
  QString loc; // localisation RU ENG Auto

  int PORT;     // connection port
  QString NAME; // request device name
  HWND HWIN;    //header of the window
  QByteArray bHWIN;
// main window
  QTabWidget *tabWidget;

// pages
  TPageLog *pageSrvD;
  TPageLog  *pageNWD;
  TPageLog  *pageDTD;
  TPageSetup   *pageSetup;

};

#endif // srvBehav_H
