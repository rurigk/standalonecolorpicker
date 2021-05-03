#ifndef COLORUTILS_H
#define COLORUTILS_H

#include <QString>
#include <QColor>
#include "../colorpickermainwindow.h"

class ColorUtils
{
	protected:
		ColorUtils();

	public:
		// Not clonable
		ColorUtils(ColorUtils &other) = delete;
		// Not assignable
		void operator=(const ColorUtils &) = delete;
		// Get Instance
		static ColorUtils * GetInstance();

		// Main window reference
		ColorPickerMainWindow * mainWindow = nullptr;

		QString GetColorString(int colorFormat, QColor color);

};

#endif // COLORUTILS_H
