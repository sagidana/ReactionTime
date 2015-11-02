#pragma once
class Color
{
private:
	int _red;
	int _green;
	int _blue;
public:
	Color();
	Color(int red, int green, int blue);
	~Color();
	int getRed();
	int getGreen();
	int getBlue();
	void setRed(int red);
	void setGreen(int green);
	void setBlue(int blue);
	bool operator== (Color &clr);
};

