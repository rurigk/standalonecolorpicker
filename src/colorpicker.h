#ifndef COLORPICKER_H
#define COLORPICKER_H

#include "pickerwindow.h"
#include "utils/colorutils.h"
#include <QList>
#include <QClipboard>

class ColorPicker : public QObject
{
	Q_OBJECT

	protected:
		ColorPicker();
	public:
		// Not clonable
		ColorPicker(ColorPicker &other) = delete;
		// Not assignable
		void operator=(const ColorPicker &) = delete;
		// Get Instance
		static ColorPicker * GetInstance();

		// Main window reference
		ColorPickerMainWindow * mainWindow;

		QList<PickerWindow*> *pickerWindows;

		void PickColor();
		void ClosePicker();
		void CancelPicker();

	private:
		bool restoreAfterPick = false;

	private slots:
		void ColorPicked(QColor color);
		void PickerCancelled();
		void MouseWheelEvent(int direction);
};

#endif // COLORPICKER_H
