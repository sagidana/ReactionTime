#include "ImageActions.h"

ImageActions* ImageActions::m_Instance;

ImageActions::ImageActions()
{
	
}

POINT* ImageActions::FindFirstColorPosition(ImageDetails* imageDetails, COLORREF color)
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

ImageActions* ImageActions::GetInstance()
{
	if (m_Instance == NULL)
		m_Instance = new ImageActions();
	return  m_Instance;
}

ImageActions::~ImageActions()
{
}
