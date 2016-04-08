#include "ImageReact.h"

ImageReact* ImageReact::m_Instance;

ImageReact::ImageReact()
{
	
}

POINT* ImageReact::FindFirstColorPosition(ImageDetails* imageDetails, COLORREF color)
{
	POINT* ret_Position = new POINT();

	int red		= GetRValue(color);
	int green	= GetGValue(color);
	int blue	= GetBValue(color);

	BYTE* image = imageDetails->data;

	for (int row = 0; row < imageDetails->height; row++)
	{
		for (int col = 0; col < imageDetails->width; col += 3)
		{
			if ((image[3 * ((row*imageDetails->width) + col)] == blue) &&
				(image[3 * ((row*imageDetails->width) + col) + 1] == green) &&
				(image[3 * ((row*imageDetails->width) + col) + 2] == red))
			{
				ret_Position->x = col / 3;
				ret_Position->y = row;

				return ret_Position;
			}
		}
	}

	return NULL;
}

ImageReact* ImageReact::GetInstance()
{
	if (m_Instance == NULL)
		m_Instance = new ImageReact();
	return  m_Instance;
}

ImageReact::~ImageReact()
{
}
