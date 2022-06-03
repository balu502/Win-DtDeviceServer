#include "srvBehav.h"
#include <stdio.h>



int main(int argc, char *argv[])
{
//    qputenv("PATH", "C:\\DtXml\\");

    QApplication a(argc, argv);
    QStringList args = a.arguments();
// setup locale loc get from settings file
    QTranslator translator(0);
    QString loc;
    QSettings setings(QApplication::applicationDirPath()+"/setup.ini",QSettings::IniFormat) ;
    loc=setings.value("Locale","EN").toString();
    if(!loc.compare("auto",Qt::CaseInsensitive))
      translator.load(QApplication::applicationDirPath()+QString("/Server_Dev_")+QLocale::system().name(),".");
    else
      if(!loc.compare("ru",Qt::CaseInsensitive))
        translator.load("ServerDev_ru",".");
      else
        translator.load("ServerDev_en",".");
    a.installTranslator(&translator);
    if(args.size() < 3) {
      QMessageBox msgBox;
      msgBox.setText("Command parameters are absent...");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.exec();
      return 1;
    }
    bool ok;
    int port = args.at(2).toInt(&ok); //9001
    if(!ok) return 1;

    a.setApplicationName(args.at(1)+ "_" + args.at(2));

    TServer_Dt process(args.at(1),port);
    process.run();
    return 0;
}
