#include <QDebug>
#include <QApplication>
#include "logobject.h"
#include <QDateTime>

//-----------------------------------------------------------------------------
//--- Constructor
//-----------------------------------------------------------------------------
LogObject::LogObject(QString logFileName,int logFileSize,int arch_length,QObject *parent) :
    QObject(parent)
{  
  FileName=logFileName+"_";
  log=new QFile(FileName+"0.log");
  logEnable=false;
  logScrEnable=false;
  max_log_size = logFileSize;
  archive_lenght = arch_length;
//  log->open( QIODevice::Text | QIODevice::Append);
//  QString s="---------------------------------------- "+QDateTime::currentDateTime().toString("dd/MM/yyyy ") +"----------------------------------------\n";
//  log->write(s.toLocal8Bit());
//  log->close();
}

//-----------------------------------------------------------------------------
//--- Destructor
//-----------------------------------------------------------------------------
LogObject::~LogObject()
{
  log->close();
  if(log) { delete log; log=0;}
}

//-----------------------------------------------------------------------------
//--- print log message on screen or into file
//-----------------------------------------------------------------------------
void LogObject::logMessage(QString message)
{
  if(logScrEnable) emit logScrMessage(message);
  if(log->isOpen()) {log->close(); return;}
  //QTime time;
  if(logEnable){
    if(!log->open( QIODevice::Text | QIODevice::Append)) return;
   // QString s=QDateTime::currentDateTime().toString("hh:mm:ss ") +message+"\n" ;
    QString s=QDateTime::currentDateTime().toString("dd-MM-yy hh:mm:ss ") +message+"\n" ;
    log->write(s.toLocal8Bit());
    log->close();
 //   time.start();
 //   while(log->isOpen()){
 //     QApplication::processEvents();
 //     if(time.elapsed()>500) { log->close(); return;}
 //   }
    if(log->size() > max_log_size) archive();
  }
}

//-----------------------------------------------------------------------------
//--- archive files. File with name _0.log all time old
//-----------------------------------------------------------------------------
void LogObject::archive(void)
{
  QFile file;
  file.setFileName(FileName +QString::number(archive_lenght) + ".log");
  file.remove();
  for(int n=archive_lenght; n>0; n--)
  {
    file.setFileName(FileName+ QString::number(n-1) + ".log");
    file.rename(FileName + QString::number(n) + ".log");
  }
//  log->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
//  log_out.setDevice(log);
//  log_out << "---------------------------------------- "+QDateTime::currentDateTime().toString("dd/MM/yyyy ") +"----------------------------------------\n" ;
// // log_out.flush();
// log->close();
}
