#include <QThread>
#include <QTextStream>
#include "colorpicker.h"
#include "utils/desktoputils.h"

ColorPicker::ColorPicker()
{
	pickerWindows = new QList<PickerWindow*>(); // Initialize the ColorPicker Widgets (Windows)
	mainWindow = nullptr;
}

ColorPicker * ColorPicker::GetInstance()
{
	static ColorPicker Instance;
	return &Instance;
}

void ColorPicker::PickColor()
{
	if(mainWindow->isHidden())
	{
		restoreAfterPick = false;
	}
	else
	{
		restoreAfterPick = true;
	}
	mainWindow->hide();

	// Blocks the thread until the window is closed
	QThread::msleep(250);

	int screenCount = DesktopUtils::GetInstance()->GetScreensCount();
	if(screenCount > pickerWindows->count())
	{
		int screensToAdd = screenCount - pickerWindows->count();
		for(int wIndex = 0; wIndex < screensToAdd; wIndex++)
		{
			PickerWindow *colorPicker = new PickerWindow();
            colorPicker->screenIndex = wIndex;
			connect(colorPicker, &PickerWindow::ColorPicked, this, &ColorPicker::ColorPicked);
			connect(colorPicker, &PickerWindow::PickerCancelled, this, &ColorPicker::PickerCancelled);
			connect(colorPicker, &PickerWindow::MouseWheelEvent, this, &ColorPicker::MouseWheelEvent);
			colorPicker->setMouseTracking(true);
			pickerWindows->push_back(colorPicker);
		}
        //qDebug() << "Added " << screensToAdd << " screens, Total screens:" << pickerWindows->count();
	}

	for(int wIndex = 0; wIndex < screenCount; wIndex++)
	{
		QScreen *screen = QGuiApplication::screens().at(wIndex);
		QRect screenRect = screen->geometry();
		PickerWindow *picker = pickerWindows->at(wIndex);
		picker->move(screenRect.x(), screenRect.y());
		picker->resize(screenRect.width(), screenRect.height());
		picker->setFixedSize(screenRect.width(), screenRect.height());

		QPixmap desktopImage = screen->grabWindow(QDesktopWidget().winId(),
		                                          screenRect.x(),
		                                          screenRect.y(),
		                                          screenRect.width(),
		                                          screenRect.height());

		picker->backgroundPixmap = desktopImage;
        if(wIndex >= 0)
        {
            picker->show();
            picker->activateWindow();
        }
	}
}

void ColorPicker::ColorPicked(QColor color)
{
    QTextStream cout(stdout);
    cout << ColorUtils::GetInstance()->GetColorString(mainWindow->format, color) << endl;

	ClosePicker();
}

void ColorPicker::PickerCancelled()
{
	ClosePicker();
}

void ColorPicker::ClosePicker()
{
	for(int wIndex = 0; wIndex < pickerWindows->count(); wIndex++)
	{
		PickerWindow *picker = pickerWindows->at(wIndex);
        picker->hide();
    }

    mainWindow->close();
}

void ColorPicker::MouseWheelEvent(int direction)
{
	for(int wIndex = 0; wIndex < pickerWindows->length(); wIndex++)
	{
		PickerWindow *picker = pickerWindows->at(wIndex);
		picker->ZoomStep(direction);
	}
}
