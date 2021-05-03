#include "colorpickermainwindow.h"
#include "ui_colorpickermainwindow.h"
#include "colorpicker.h"

#include "utils/desktoputils.h"
#include "utils/colorutils.h"

ColorPickerMainWindow::ColorPickerMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ColorPickerMainWindow)
{
	// Setup the window UI
	ui->setupUi(this);

	// Setup theme
    /*QFile defaultTheme(":/themes/default.qss");

	if (!defaultTheme.exists())   {
		printf("Unable to set stylesheet, file not found\n");
	}
	else   {
		defaultTheme.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&defaultTheme);
		setStyleSheet(ts.readAll());
    }*/

	// Set singletons references to this object
	DesktopUtils::GetInstance()->mainWindow = this;
	ColorUtils::GetInstance()->mainWindow = this;
	ColorPicker::GetInstance()->mainWindow = this;

    hide();
    ColorPicker::GetInstance()->PickColor();
}

ColorPickerMainWindow::~ColorPickerMainWindow()
{
	delete ui;
}

