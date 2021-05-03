#include "colorpickermainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QtGlobal>
#if defined( Q_OS_WIN )
    #include <windows.h>
#endif

void noMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
     Q_UNUSED(type);
     Q_UNUSED(context);
     Q_UNUSED(msg);
}

int main(int argc, char *argv[])
{
    qputenv("QT_LOGGING_RULES", "qt.qpa.*=false");
    #if defined( Q_OS_WIN )
        ::ShowWindow( ::GetConsoleWindow(), SW_HIDE ); //hide console window
    #endif
    //QCoreApplication app(argc, argv);
    QApplication app(argc, argv);


    app.setApplicationVersion("1.0.0");

    app.setOrganizationName("Rurigk");
    app.setApplicationName("StandaloneColorPicker");
    app.setOrganizationDomain("moe.rurigk.standalonecolorpicker");

    QCommandLineParser parser;
    parser.setApplicationDescription("Standalone Color Picker");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption({{"f", "format"}, "The color output format ", "format"});
    parser.addOption({{"d", "debug"}, "Enable debug messages"});

    parser.process(app);

	// Prevent app exit when all the windows are closed
    //QApplication::setQuitOnLastWindowClosed(false);

    if (parser.isSet("d") || parser.isSet("debug")) {
        qInstallMessageHandler(noMessageOutput);
    }

    int cformat = 0;

    if(parser.isSet("format"))
    {
        QString format = parser.value("format");
        if(format == "hex")
        {
            cformat = 0;
        }
        else if(format == "rgb")
        {
            cformat = 1;
        }
        else if(format == "hsv")
        {
            cformat = 2;
        }
        else if(format == "hsl")
        {
            cformat = 3;
        }
        else if(format == "cmyk")
        {
            cformat = 4;
        }
    }

    ColorPickerMainWindow * w = new ColorPickerMainWindow;

    w->format = cformat;
    //w.show();

    //return 0;
    int e = app.exec();
    delete w;
    exit( e );
    return e;
}
