#include <complex>
#include "ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) 
{
	m_pixel_size = { pixelWidth, pixelHeight };
	m_aspectRatio = (float)pixelHeight / (float)pixelWidth;

	m_plane_center = { 0,0 };
	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	m_zoomCount = 0;
	m_state = State::CALCULATING;

	m_vArray.setPrimitiveType(Points);
	m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const 
{
	target.draw(m_vArray);
}

void ComplexPlane::zoomIn() 
{
	//Increment m_zoomCount
	m_zoomCount++;
	//Set a local variable for the x size to BASE_WIDTH* (BASE_ZOOM to the m_zoomCount power)
	float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	//Set a local variable for the y size to BASE_HEIGHT* m_aspectRatio* (BASE_ZOOM to the m_zoomCount power)
	float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	//Assign m_plane_size with this new size
	m_plane_size = {x,y};
	//Set m_State to CALCULATING
	m_State = CALCULATING;
}

void ComplexPlane::zoomOut() 
{
	//Decrement m_zoomCount
	m_zoomCount--;
	//Set a local variable for the x size to BASE_WIDTH* (BASE_ZOOM to the m_zoomCount power)
	float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	//Set a local variable for the y size to BASE_HEIGHT* m_aspectRatio* (BASE_ZOOM to the m_zoomCount power)
	float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	//Assign m_plane_size with this new size
	m_plane_size = { x,y };
	//Set m_State to CALCULATING
	m_State = CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel) 
{
	//Use ComplexPlane::mapPixelToCoords to find the Vector2f coordinate in the complex plane that corresponds to the screen pixel location
	Vector2f current_coord = mapPixelToCoords({ mousePixel.x, mousePixel.y });
	//Assign m_plane_center with this coordinate
	m_plane_center = current_coord;
	//Set m_State to CALCULATING
	m_State = CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel) 
{
	//Use ComplexPlane::mapPixelToCoords to find the Vector2f coordinate in the complex plane that corresponds to the screen pixel location
	Vector2f current_coord = mapPixelToCoords({ mousePixel.x, mousePixel.y });
	//Assign m_mouseLocation with this coordinate
	m_mouseLocation = current_coord;
}

//void ComplexPlane::loadText(Text& text) {}

void ComplexPlane::updateRender() 
{
	int pixelWidth = m_pixel_size.x;
	int pixelHeight = m_pixel_size.y;

	if (m_state == CALCULATING)
	{
		for (int i = 0; i < pixelHeight; i++)
		{
			for (int j = 0; j < pixelWidth; j++)
			{
				m_vArray[j + i * pixelWidth].position = { (float)j, (float)i };
				Vector2f current_coord = mapPixelToCoords({ (float)j, (float)i });

				Uint8 r, g, b;
				iterationsToRGB(countIterations(current_coord), r, g, b);
				m_vArray[j + i * pixelWidth].color = { r, g, b };
			}
		}
		m_state = DISPLAYING;
	}
}

size_t ComplexPlane::countIterations(Vector2f coord) 
{
	size_t i = 0;
	complex<float> c = { coord.x, coord.y };
	complex<float> z = c;

	while (abs(z) < 2.0 && i < MAX_ITER)
	{
		z = z * z + c;
		i++;
	}

	return i;
}

//void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) {}

//Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) {}
