#ifndef PICKERWINDOW_H
#define PICKERWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QColor>
#include <QShortcut>
#include <QScreen>
#include <QTimer>

namespace Ui {
	class PickerWindow;
}

class PickerWindow : public QWidget
{
    Q_OBJECT

public:
	explicit PickerWindow(QWidget *parent = nullptr);
	~PickerWindow();
    void paintEvent(QPaintEvent *);
	void ZoomStep(int direction);

    QPixmap emptyPixmap = *new QPixmap();
    QPixmap backgroundPixmap;
    int screenIndex = 0;
	//ColorPanel * colorPanel;

signals:
    void ColorPicked(QColor color);
    void PickerCancelled();
	void MouseWheelEvent(int direction);

private:
	Ui::PickerWindow *ui;
	void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void EscapeKeyPressed();
    void MouseTimer();

    QPoint *mousePos;

    float mousePositionX = -1;
    float mousePositionY = -1;

    QCursor *cursor;
    QTimer *timer;

    QShortcut *shortcut;
    bool drawLens = false;
	int lensRectSize = 160;
	int imageSampleRectSize = 20;

	int infoBoxWidth = lensRectSize + 1;
	int infoBoxHeight = 25;

	float zoomFactor = 1;

	int infoBoxPosition = -1;
	QPen penDefault;
	QPen penWhite;
	QPen penTranslucentDark;

	QColor translucentDark;

    QScreen * currentScreen = nullptr;
};

#endif // PICKERWINDOW_H
