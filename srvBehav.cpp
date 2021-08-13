#include <QThread>
//#include <w32api.h>
//#define WINVER WindowsXP
//#define ES_AWAYMODE_REQUIRED 0x00000040
//#include <windows.h>
//#include <winbase.h>
#include "srvBehav.h"

//-----------------------------------------------------------------------------
//--- Constructor
//-----------------------------------------------------------------------------
TServer_Dt::TServer_Dt(QString dName,int nPort, QWidget *parent)
    : QMainWindow(parent)
{
  QString compilationTime = QString("%1 %2").arg(__DATE__).arg(__TIME__);

  PORT = nPort;
  NAME = dName;
  HWIN=(HWND)this->winId(); // set window ID
  bHWIN=QString(" WinID=%1 ").arg((int)HWIN,0,16).toLatin1();
  device=0;
  //sHWIN=QString("%1").arg(HWIN);
  logSys=new LogObject(QApplication::applicationDirPath()+"/log/"+"system"); logSys->setLogEnable(false,true); // enable system log into file only
  logSys->logMessage(tr(bHWIN+"Start Dt-server. Ver. "+APP_VERSION)+" "+compilationTime);
  logSys->logMessage(bHWIN+"Device name "+NAME+QString(" Port=%1").arg(PORT));
  setTrayIconActions();
  showTrayIcon();

  Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint |
                             Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint;
  setWindowFlags(flags);

// read settings
  setingsFileName=QApplication::applicationDirPath()+"/setup.ini" ;
  IniFileRead();

// create pages with log and settings
  pageSrvD= new TPageLog;
  pageNWD=  new TPageLog;
  pageDTD=  new TPageLog;
  pageSetup=new TPageSetup;

// create tab widget
  tabWidget=new QTabWidget;
  tabWidget->addTab(pageSrvD, tr("Show system diagnostics"));
  tabWidget->addTab(pageNWD, tr("Show network diagnostics"));
  tabWidget->addTab(pageDTD, tr("Show DT-XX diagnostics"));
  tabWidget->addTab(pageSetup, tr("Setup"));

  setCentralWidget(tabWidget);
  tabWidget->setCurrentIndex(0);

  setupUI();

  if((nPort<9000)||(nPort>9100))  //incorrect number of port
  {
    logSys->logMessage(tr(bHWIN+"Unable to start the server. Network error. Incorrect port number."));
    QTimer::singleShot(1000,this,SLOT(close()));
    return;
  }
  device=0;
  device=new TDtBehav(nPort,dName,HWIN,log_Size,log_Count);
  if(!device) {
    logSys->logMessage(tr("Object TDtBehav make error."));
    return;
  }
  if(device->readGlobalErr()){ //connect to network error. Program abort
    logSys->logMessage(bHWIN+device->readGlobalErrText());
    QTimer::singleShot(1000,this,SLOT(close()));
    return;
  }
  setLogEnable();

// signals for Log pages
  connect(device->logSrv,SIGNAL(logScrMessage(QString)),pageSrvD,SLOT(addToLog(QString)));
  connect(device->logNw,SIGNAL(logScrMessage(QString)),pageNWD,SLOT(addToLog(QString)));
  connect(device->logDev,SIGNAL(logScrMessage(QString)),pageDTD,SLOT(addToLog(QString)));
  connect(pageNWD,SIGNAL(pauseChange(bool)),pageSetup,SLOT(setCbLogNWS(bool)));

  connect(pageDTD,SIGNAL(pauseChange(bool)),pageSetup,SLOT(setCbLogDTS(bool)));
// signal on change data in setup page
  connect(pageSetup,SIGNAL(sigChangeUI()),this,SLOT(getUI())); //change combo box on page settings

  setMinimumWidth(500);
  resize(500,330);
  move(50, 50);
  setWindowTitle(dName+ "_" + QString("%1").arg(nPort));
  show();
  QTimer::singleShot(3000, this, SLOT(hide()));
  device->logSrv->logMessage(tr("Start Dt-server. Version ")+APP_VERSION+" "+compilationTime);
}

//-----------------------------------------------------------------------------
// procedure RUN logic of device
//-----------------------------------------------------------------------------
void TServer_Dt::run()
{
  if(device) device->run();
}

//-----------------------------------------------------------------------------
//--- Destructor
//-----------------------------------------------------------------------------

TServer_Dt::~TServer_Dt()
{
  qDebug()<<"Server destructor start.";
  if(device) delete device;
  device=0;
  delete tabWidget;
  delete trayIcon;
  logSys->logMessage(tr(bHWIN+"Stop Dt-server."));
  if(logSys) delete logSys;
  qDebug()<<"Server destructor finished.";
}

//-----------------------------------------------------------------------------
//--- Read ini settings
//-----------------------------------------------------------------------------
void TServer_Dt::IniFileRead(void)
{
  QSettings setings(setingsFileName,QSettings::IniFormat) ;
  logSrvScr=setings.value("logSystemScr",true).toBool();
  logSrv=setings.value("logSystem",true).toBool();
  logNWScr=setings.value("logNWScr",true).toBool();
  logNW=setings.value("logNW",true).toBool();
  logDTScr=setings.value("logDTScr",true).toBool();
  logDT=setings.value("logDT",true).toBool();
  logDTM=setings.value("logDTMASTER",false).toBool();
  log_Size=setings.value("logFileSize",100000).toInt();
  log_Count=setings.value("logFileCount",10).toInt();
  loc=setings.value("Locale","EN").toString();
}

//-----------------------------------------------------------------------------
//--- Write ini settings
//-----------------------------------------------------------------------------
void TServer_Dt::IniFileWrite(void)
{
   QSettings setings(setingsFileName,QSettings::IniFormat) ;

   setings.setValue("logSystemScr",logSrvScr);
   setings.setValue("logSystem",logSrv);
   setings.setValue("logNWScr",logNWScr);
   setings.setValue("logNW",logNW);
   setings.setValue("logDTScr",logDTScr);
   setings.setValue("logDT",logDT);
}

//-----------------------------------------------------------------------------
//--- Sets enable/disable log information in log object
//-----------------------------------------------------------------------------
void TServer_Dt::setLogEnable(void)
{
  device->logNw->setLogEnable(logNWScr,logNW);
  device->logSrv->setLogEnable(logSrvScr,logSrv);
  device->logDev->setLogEnable(logDTScr,logDT);
  device->logDTMaster->setLogEnable(false,logDTM);
}

//-----------------------------------------------------------------------------
//--- Put on user interface data from settings
//-----------------------------------------------------------------------------
void TServer_Dt::setupUI(void)
{
  pageSetup->setCbLogSysS(logSrvScr);
  pageSetup->setCbLogSys(logSrv);
  pageSetup->setCbLogNWS(logNWScr);
  pageSetup->setCbLogNW(logNW);
  pageSetup->setCbLogDTS(logDTScr);
  pageSetup->setCbLogDT(logDT);
}

//-----------------------------------------------------------------------------
//--- Private slot on change combo box in setup page
//-----------------------------------------------------------------------------
void TServer_Dt::getUI(void)
{
  logSrvScr=pageSetup->getCbLogSysS();
  logSrv=pageSetup->getCbLogSys();
  logNWScr=pageSetup->getCbLogNWS();
  logNW=pageSetup->getCbLogNW();
  logDTScr=pageSetup->getCbLogDTS();
  logDT=pageSetup->getCbLogDT();

  setLogEnable(); //update log
  IniFileWrite();
}

//-----------------------------------------------------------------------------
//--- Private slot
//--- Show icon from resurces in tray
//-----------------------------------------------------------------------------
void TServer_Dt::showTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    QIcon trayImage = QIcon(":/image/usb_32.png");
    trayIcon->setIcon(trayImage);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip("DT-XX server");
    setWindowIcon(trayImage);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,     SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon ->show();
}

//-----------------------------------------------------------------------------
//--- Private slot
//--- Set actions in pull up menu
//-----------------------------------------------------------------------------
void TServer_Dt::setTrayIconActions()
{
  //... Setting actions ...
  minimizeAction = new QAction(tr("Minimize"), this);
  restoreAction = new QAction(tr("Restore"), this);
  quitAction = new QAction(tr("Exit"), this);

  //... Connecting actions to slots ...
  connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hideProg()));
  connect(restoreAction, SIGNAL(triggered()), this, SLOT(showProg()));
  connect(quitAction, SIGNAL(triggered()), this, SLOT(quitProg()));


  //... Setting system tray's icon menu ...
  trayIconMenu = new QMenu(this);

  trayIconMenu->addAction (restoreAction);
  trayIconMenu->addAction (minimizeAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction (quitAction);
}

//-----------------------------------------------------------------------------
//--- Private slot
//--- tray processing function. Reaction on press mouse in the icon region
//-----------------------------------------------------------------------------
void TServer_Dt::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch (reason) {
  case QSystemTrayIcon::Trigger:
  case QSystemTrayIcon::DoubleClick:
    if(!isVisible()) this->showNormal(); else hide();
    break;
  case QSystemTrayIcon::MiddleClick:
    break;
  default:
    break;
  }
}

// actions on tray menu pressed  private slots
//-----------------------------------------------------------------------------
//--- Private slot
//--- restore window
//-----------------------------------------------------------------------------
void TServer_Dt::showProg(void)
{
  showNormal();
}
//-----------------------------------------------------------------------------
//--- Private slot
//--- hide window
//-----------------------------------------------------------------------------
void TServer_Dt::hideProg(void)
{
  hide();
}

//-----------------------------------------------------------------------------
//--- Private slot
//--- quite from program
//-----------------------------------------------------------------------------
void TServer_Dt::quitProg(void)
{
  logSys->logMessage(tr(bHWIN+"Get event on finish program from user."));
  if(device) {
    device->setAbort(true);
    device->msleep(50);
    device=0;
  }
  qApp->quit();
}

//-----------------------------------------------------------------------------
//--- Reimplement of the close window event
//-----------------------------------------------------------------------------
void TServer_Dt::closeEvent(QCloseEvent *event)
{
  if(trayIcon->isVisible()) {
    hide();
    event->ignore();
  }
}

//-------------------------------------------------------------------------------
//--- User events
//-------------------------------------------------------------------------------
bool TServer_Dt::nativeEvent(const QByteArray & /*eventType*/, void *message, long * /*result*/ )

{
  uint WM_USERCLOSE = 0x7fff + 0x666;

  MSG *msg = static_cast< MSG * >( message );

  if(msg->message == WM_USERCLOSE)
  {
    logSys->logMessage(tr(bHWIN+"Get event on finish program from server of USB."));
    if(device) {
      device->setAbort(true);
      device->msleep(50);
      device=0;
    }
    qApp->quit();
  }
  return false;
}

