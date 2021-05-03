#include "pickerwindow.h"
#include "ui_pickerwindow.h"
#include "utils/colorutils.h"
//#include "colorpanel.h"

PickerWindow::PickerWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PickerWindow)
{
	ui->setupUi(this);
    //setCursor(Qt::BlankCursor);

	//setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);

	setMouseTracking(true);
	mousePos = new QPoint(-1, -1);
	shortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
	connect(shortcut, &QShortcut::activated, this, &PickerWindow::EscapeKeyPressed);

	translucentDark = QColor(0, 0, 0, 180);

	penDefault = QPen (Qt::black, 1);
	penWhite = QPen (Qt::white, 1);
	penTranslucentDark = QPen (translucentDark, 1);
    cursor = new QCursor();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PickerWindow::MouseTimer);
    timer->start(1000/30);
}

PickerWindow::~PickerWindow()
{
    delete ui;
}

void PickerWindow::ZoomStep(int direction)
{
    zoomFactor += 0.1 * direction;
	zoomFactor = zoomFactor < 0.4 ? 0.4 : zoomFactor;
	zoomFactor = zoomFactor > 4 ? 4 : zoomFactor;

	float lensRectSizeScaled = (float)lensRectSize * zoomFactor;
	infoBoxPosition = (mousePos->y() >= height() - infoBoxHeight - (lensRectSizeScaled / 2))?  1 : 2;
	repaint();
}

void PickerWindow::paintEvent(QPaintEvent *)
{
	// Start widget painter
    QPainter painter;
    painter.begin(this);

    // Save the actual font to temporal variable
    QFont defaultFont = painter.font();

	// Calc the new font size
    float fontSize = defaultFont.pointSize() * (96 / screen()->logicalDotsPerInchX());
    QFont scaledFont = QFont(defaultFont.family(), fontSize);

	// Set the new font size
    painter.setFont(scaledFont);

	// Draw the background of the widget (screenshot)
    painter.drawPixmap(0, 0,
                       backgroundPixmap.width(),
                       backgroundPixmap.height(),
                       backgroundPixmap);

	// Check if the flag is active and the virtual mouse position is positive to enable lens drawing
    if(drawLens && mousePos->x() >= 0 && mousePos->y() >= 0)
    {
		float lensRectSizeScaled = (float)lensRectSize * zoomFactor;

		float minMouseXPos = (lensRectSizeScaled / 2);
		float maxMouseXPos = width() - (lensRectSizeScaled / 2);

		float minMouseYPos = (lensRectSizeScaled / 2);
		float maxMouseYPos = height() - (lensRectSizeScaled / 2);

		float mouseXPos = mousePos->x() < minMouseXPos ? minMouseXPos : mousePos->x();
		mouseXPos = mouseXPos > maxMouseXPos ? maxMouseXPos : mouseXPos;

		float mouseYPos = mousePos->y() < minMouseYPos ? minMouseYPos : mousePos->y();
		mouseYPos = mouseYPos > maxMouseYPos ? maxMouseYPos : mouseYPos;

		QPoint * mouseLimitedPos = new QPoint(mouseXPos, mouseYPos);

        // Draw rect bg
        painter.fillRect(mouseLimitedPos->x() - (lensRectSizeScaled / 2),
                        mouseLimitedPos->y() - (lensRectSizeScaled / 2),
                        lensRectSizeScaled, lensRectSizeScaled, QBrush(Qt::black, Qt::SolidPattern));

		// Draw a scaled fragment of the screenshot to simulate zoom
        painter.drawPixmap(
		                   mouseLimitedPos->x() - (lensRectSizeScaled / 2),
		                   mouseLimitedPos->y() - (lensRectSizeScaled / 2),
		                   lensRectSizeScaled,
		                   lensRectSizeScaled,
                           backgroundPixmap,
		                   mousePos->x() - (imageSampleRectSize / 2),
		                   mousePos->y() - (imageSampleRectSize / 2),
		                   imageSampleRectSize,
		                   imageSampleRectSize);

		// Draw the lens border
		painter.drawRect(mouseLimitedPos->x() - (lensRectSizeScaled / 2),
		                mouseLimitedPos->y() - (lensRectSizeScaled / 2),
		                lensRectSizeScaled, lensRectSizeScaled);



		// Transform the pixmap to image
        QImage image = backgroundPixmap.toImage();
		// Extract the color at pixel
		QColor color = image.pixel(mousePos->x(), mousePos->y());
		// Get Contrast color black or white
		QPen contrastColorPen = (color.red() * 0.299 + color.green() * 0.587 + color.blue() * 0.114) > 166 ? penDefault : penWhite;

		// Get the color string
		QString colorString = ColorUtils::GetInstance()->GetColorString(1, color);

		//QPen penColor(color, 1);
		painter.setPen(contrastColorPen);

		// Draw the middle pixel selector box
		painter.drawRect(mouseLimitedPos->x(), mouseLimitedPos->y(), floor((float)8 * zoomFactor), floor((float)8 * zoomFactor));

		// Set the pen color to translucent black
		painter.setPen(penDefault);

		// Draw info box
		if(infoBoxPosition == 1)
		{
			int startPositionX = mouseXPos - (lensRectSizeScaled / 2);
			int startPositionY = mouseYPos - (lensRectSizeScaled / 2) - infoBoxHeight;
			painter.fillRect(startPositionX, startPositionY, infoBoxWidth, infoBoxHeight, translucentDark);

			painter.fillRect(startPositionX + 4, startPositionY + 4, 16, 16, color);

			painter.setPen(QPen (Qt::white));
			painter.drawText(startPositionX + 24, startPositionY + 16, colorString);
		}
		else if(infoBoxPosition == 2)
		{
			int startPositionX = mouseXPos - (lensRectSizeScaled / 2);
			int startPositionY = mouseYPos + (lensRectSizeScaled / 2);
			painter.fillRect(startPositionX, startPositionY, lensRectSizeScaled < infoBoxWidth ? infoBoxWidth : lensRectSizeScaled, infoBoxHeight, translucentDark);

			painter.fillRect(startPositionX + 4, startPositionY + 4, 16, 16, color);

			painter.setPen(QPen (Qt::white));
			painter.drawText(startPositionX + 24, startPositionY + 16, colorString);
		}


		// Reset the pen color to black
		painter.setPen(penDefault);
    }

    painter.end();
}

void PickerWindow::wheelEvent(QWheelEvent *event)
{
	QPoint numPixels = event->pixelDelta();
	QPoint numDegrees = event->angleDelta() / 8;

	if (!numPixels.isNull()) {
		MouseWheelEvent(numPixels.y() > 0 ? 1 : -1);
	} else if (!numDegrees.isNull()) {
		QPoint numSteps = numDegrees / 15;
		MouseWheelEvent(numSteps.y() > 0 ? 1 : -1);
	}

	event->accept();
}

void PickerWindow::mouseMoveEvent(QMouseEvent * event)
{

}

void PickerWindow::MouseTimer()
{
    if(currentScreen == nullptr)
    {
        currentScreen = QGuiApplication::screens().at(screenIndex);
    }

    // Input scale factor
    float positionScale = 0.5;
    // Lens rect size scaled by zoom factor
    float lensRectSizeScaled = (float)lensRectSize * zoomFactor;

    QPoint cursorPos = cursor->pos();



    QRect vGeo = currentScreen->geometry();

    // Read the mouse input;
    QPoint *mousePoint = new QPoint(cursorPos.x() - vGeo.x(), cursorPos.y() - vGeo.y());
    QPoint *mousePointDBG = new QPoint(cursorPos.x() - vGeo.x(), cursorPos.y() - vGeo.y());
    if(
        cursorPos.x() >= vGeo.x() &&
        cursorPos.y() >= vGeo.y() &&
        cursorPos.x() <= vGeo.x() + vGeo.width() &&
        cursorPos.y() <= vGeo.y() + vGeo.height()
    )
    {

        // If the mouse position is less than 0 we just copy the raw position to the variables
        if(mousePositionX  < 0 || mousePositionY < 0)
        {
            mousePositionX = mousePoint->x();
            mousePositionY = mousePoint->y();

            qDebug() << "first position:" << mousePositionX << ":" << mousePositionY;
        }
        else
        {
            // If the virtual mouseposition is not negative we process the calcs to scale the input

            // Get the differences between the old and the new position
            float mouseXDiff = (float)mousePoint->x() - (float)mousePos->x();
            float mouseYDiff = (float)mousePoint->y() - (float)mousePos->y();

            // Scale the input value
            float mouseXDiffScaled = mouseXDiff * positionScale;
            float mouseYDiffScaled = mouseYDiff * positionScale;



            mousePoint = new QPoint(floorf(mouseXDiffScaled + mousePositionX), floorf(mouseYDiffScaled + mousePositionY));
            mousePositionX = mouseXDiffScaled + mousePositionX;
            mousePositionY = mouseYDiffScaled + mousePositionY;

            if(mouseXDiff != 0 || mouseYDiff != 0)
            {
                qDebug() << "mpvg" << mousePointDBG->x() << ":" << vGeo.x();
                qDebug() << "real mouse position:" << mapToGlobal(QPoint(mousePoint->x(), mousePoint->y()));
                qDebug() << "old virtual fmouse position:" << mousePositionX << ":" << mousePositionY;
                qDebug() << "mouse diff:" << mouseXDiff << ":" << mouseYDiff;
                qDebug() << "mouse diff scaled:" << mouseXDiffScaled << ":" << mouseYDiffScaled;
                qDebug() << "new virtual fmouse position:" << mousePositionX << ":" << mousePositionY;
                qDebug() << "virtual mouse position:" << mapToGlobal(QPoint(mousePoint->x(), mousePoint->y())) << endl  << endl;
            }


            cursor->setPos(mapToGlobal(QPoint(mousePoint->x(), mousePoint->y())));
        }
    }

    int xDiff = mousePos->x() - mousePoint->x() < 0? (mousePos->x() - mousePoint->x())*-1 : mousePos->x() - mousePoint->x();
    int yDiff = mousePos->y() - mousePoint->y() < 0? (mousePos->y() - mousePoint->y())*-1 : mousePos->y() - mousePoint->y();
    /*if(xDiff > lensRectSize / 2 || yDiff > lensRectSize / 2)
    {
        mousePos = mousePoint;
        update((mousePos->x() - (lensRectSize / 2)) - 300,
               (mousePos->y() - (lensRectSize / 2)) - 300,
               lensRectSize + 600, lensRectSize + 600);
    }
    else
    {
        mousePos = mousePoint;
        update((mousePos->x() - (lensRectSize / 2)) - 50,
               (mousePos->y() - (lensRectSize / 2)) - 50,
               lensRectSize + 100, lensRectSize + 100);
    }*/

    mousePos = mousePoint;
    update((mousePos->x() - (lensRectSize / 2)) - 300,
           (mousePos->y() - (lensRectSize / 2)) - 300,
           lensRectSize + 600, lensRectSize + 600);

    infoBoxPosition = (mousePos->y() >= height() - infoBoxHeight - (lensRectSizeScaled / 2))?  1 : 2;

    repaint();
}

void PickerWindow::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		QImage image = backgroundPixmap.toImage();
		QColor color = image.pixel(event->pos().x(), event->pos().y());
		ColorPicked(color);
		backgroundPixmap = emptyPixmap;
		mousePos = new QPoint(-1, -1);
	}
	else
	{
		backgroundPixmap = emptyPixmap;
		mousePos = new QPoint(-1, -1);
		PickerCancelled();
	}
}

void PickerWindow::enterEvent(QEvent *)
{
    drawLens = true;
    repaint();
}

void PickerWindow::leaveEvent(QEvent *)
{
    drawLens = false;
    mousePositionX = -1;
    mousePositionY = -1;
    repaint();
}

void PickerWindow::EscapeKeyPressed()
{
    backgroundPixmap = emptyPixmap;
    mousePos = new QPoint(-1, -1);
    PickerCancelled();
}
