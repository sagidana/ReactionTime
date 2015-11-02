#include "Color.h"


Color::Color()
{
	Color::_red = 0;
	Color::_green = 0;
	Color::_blue = 0;
}

Color::Color(int red, int green, int blue)
{
	Color::_red = red;
	Color::_green = green;
	Color::_blue = blue;
}

Color::~Color(){}

int Color::getRed()
{
	return Color::_red;
}

int Color::getGreen()
{
	return Color::_green;
}

int Color::getBlue()
{
	return Color::_blue;
}

void Color::setRed(int red)
{
	Color::_red = red;
}

void Color::setGreen(int green)
{
	Color::_green = green;
}

void Color::setBlue(int blue)
{
	Color::_blue = blue;
}

bool Color::operator == (Color &clr) 
{
	bool ret_IsEqual =	Color::getRed() == clr.getRed() && 
						Color::getGreen() == clr.getGreen() && 
						Color::getBlue() == clr.getBlue() ? true : false;

	return ret_IsEqual;
}