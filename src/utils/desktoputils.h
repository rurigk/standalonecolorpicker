#ifndef DESKTOPUTILS_H
#define DESKTOPUTILS_H

#include <QTimer>
#include <QScreen>
#include <QPixmap>
#include <QGuiApplication>
#include <QDesktopWidget>
#include "../colorpickermainwindow.h"

class DesktopUtils
{
	protected:
		DesktopUtils();
	public:

		// Not clonable
		DesktopUtils(DesktopUtils &other) = delete;
		// Not assignable
		void operator=(const DesktopUtils &) = delete;
		// Get instance
		static DesktopUtils * GetInstance();

		// Main window reference
		ColorPickerMainWindow * mainWindow = nullptr;

		int GetScreensCount();
		QScreen * GetScreen(int screenIndex);
		QPixmap GetScreenImage(int screenIndex);

	private:

		QTimer * screensCheckTimer;
		int lastScreenCount = 0;
};


#endif // DESKTOPUTILS_H
