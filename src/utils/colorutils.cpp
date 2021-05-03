#include "colorutils.h"

ColorUtils::ColorUtils()
{
	//if(Instance)
}

ColorUtils * ColorUtils::GetInstance()
{
	static ColorUtils Instance;
	return &Instance;
}

QString ColorUtils::GetColorString(int colorFormat, QColor color)
{
	switch(colorFormat)
	{
		case 0: // HEX
		    return color.name(QColor::HexRgb).toUpper();
		case 1: // RGB
		{
			QString str = "rgb(" +
			QString::number( color.red() ) + ", " +
			QString::number( color.green() ) + ", " +
			QString::number( color.blue() ) + ")";
			return str;
		}
		case 2: // HSV
		{
			QString str = "hsv(" +
			QString::number( color.hsvHue() ) + "º, " +
			QString::number( (int)round(((float)color.hsvSaturation() / (float)255) * (float)100) ) + "%, " +
			QString::number( (int)round(((float)color.value() / (float)255) * (float)100) ) + "%)";
			return str;
		}
		case 3: // HSL
		{
			QString str = "hsl(" +
			QString::number( color.hslHue() ) + ", " +
			QString::number( (int)round(((float)color.hslSaturation() / (float)255) * (float)100) ) + "%, " +
			QString::number( (int)round(((float)color.lightness() / (float)255) * (float)100) ) + "%)";
			return str;
		}
		case 4: // CMYK
		{
			QString str = "cmyk(" +
			QString::number( (int)round(((float)color.cyan() / (float)255) * (float)100) ) + "%, " +
			QString::number( (int)round(((float)color.magenta() / (float)255) * (float)100) ) + "%, " +
			QString::number( (int)round(((float)color.yellow() / (float)255) * (float)100) ) + "%, " +
			QString::number( (int)round(((float)color.black() / (float)255) * (float)100) ) + "%)";
			return str;
		}
		default:
		    return color.name(QColor::HexRgb);
	}
}
