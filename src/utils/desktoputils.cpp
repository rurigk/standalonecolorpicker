#include "desktoputils.h"

DesktopUtils::DesktopUtils()
{

}

DesktopUtils * DesktopUtils::GetInstance()
{
	static DesktopUtils Instance;
	return &Instance;
}

int DesktopUtils::GetScreensCount()
{
	return QGuiApplication::screens().count();
}

QScreen * DesktopUtils::GetScreen(int screenIndex)
{
	return QGuiApplication::screens().at(screenIndex);
}

QPixmap DesktopUtils::GetScreenImage(int screenIndex)
{
	QScreen * screen = GetScreen(screenIndex);
	QRect screenRect = screen->geometry();

	return screen->grabWindow(QDesktopWidget().winId(),
	                          screenRect.x(),
	                          screenRect.y(),
	                          screenRect.width(),
	                          screenRect.height());
}
