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
	m_state = CALCULATING;
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
	m_state = CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
	m_plane_center = mapPixelToCoords({ mousePixel.x, mousePixel.y });
	m_state = CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
	m_mouseLocation = mapPixelToCoords({ mousePixel.x, mousePixel.y });
}

void ComplexPlane::loadText(Text& text) 
{
	//Use a stringstream and the corresponding member variables to create the following output :
	//Note:  Cursor should update live as the user moves the mouse.  Center should only update after they click.
	stringstream out;
	out << "Mandelbrot Set" << endl;
	out << "Center: (" << m_plane_center.x << "," << m_plane_center.y << ")" << endl;
	out << "Cursor: (" << m_mouseLocation.x << "," << m_mouseLocation.y << ")" << endl;
	out << "Left-click to Zoom in" << endl;
	out << "Right-click to Zoom out" << endl;
}

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

	while (abs(z) < 2.0)
	{
		z = z * z + c;
		i++;
	}

	return i;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) 
{
	//If the coord converges, assign colours to black
	if (count >= MAX_ITER)
	{
		r = g = b = 0;
		return;
	}

	//For smooth colour transition it is easier to work with the values from 0 to 1
	float gradient = (float)count / MAX_ITER;

	//Max values: r = 255, g = 255, b = 255
	//First region: Purple (128, 0, 255) to Blue (0, 0, 255)
	if (gradient < 0.2)
	{
		r = (128 + 127 * gradient / 0.2);
		g = 0;
		b = 255;
	}
	//Second region: Turquoise (0, 255, 255)
	else if (gradient < 0.4)
	{
		float grad = (gradient - 0.2) / 0.2; //Rescale gradient back to 0 to 1
		r = 255 * (1.0 - grad);
		g = 255 * grad;
		b = 255;
	}
	//Third region: Green (0, 255, 0)
	else if (gradient < 0.6)
	{
		float grad = (gradient - 0.4) / 0.2;
		r = 0;
		g = 255;
		b = 255 * (1.0 - grad);
	}
	//Fourth region: Yellow (255, 255, 0)
	else if (gradient < 0.8)
	{
		float grad = (gradient - 0.6) / 0.2;
		r = 255 * grad;
		g = 255;
		b = 0;
	}
	//Fifth region: Red (255, 0, 0)
	else
	{
		float grad = (gradient - 0.8) / 0.2;
		r = 255;
		g = 255 * (1.0 - grad);
		b = 0;
	}
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
	//Formula: ((n  - a) / (b - a)) * (d - c) + c
	//Example x: x = 960 would map to:  ((960 - 0) / (1920 - 0)) * (2 - - 2) + (-2) == 0
	//Example y: y = 540 would map to:  ((540 - 1080) / (0 - 1080)) * (2 - - 2) + (-2) == 0

	float coord_x = ((mousePixel.x - 0) / float(m_plane_size.x - 0)) * m_plane_size.x
		+ (m_plane_center.x - m_plane_size.x / 2.0);

	float coord_y = ((mousePixel.y - m_plane_size.y) / float(0 - m_plane_size.y)) * m_plane_size.y
		+ (m_plane_center.y - m_plane_size.y / 2.0);

	return { coord_x, coord_y };
}
