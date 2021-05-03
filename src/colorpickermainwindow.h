#ifndef COLORPICKERMAINWINDOW_H
#define COLORPICKERMAINWINDOW_H

#include <math.h>
#include <QMainWindow>
#include <QApplication>

#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class ColorPickerMainWindow; }
QT_END_NAMESPACE

class ColorPickerMainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		ColorPickerMainWindow(QWidget *parent = nullptr);
		~ColorPickerMainWindow();

		// Get Instance
		static ColorPickerMainWindow * GetInstance();

        int format = 0;

	private:
        Ui::ColorPickerMainWindow *ui;

};
#endif // COLORPICKERMAINWINDOW_H
