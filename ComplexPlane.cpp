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
	//Set a local variable for the x size to BASE_WIDTH* (BASE_ZOOM to the m_ZoomCount power)
	int x = BASE_WIDTH * pow(BASE_ZOOM, m_ZoomCount);
	//Set a local variable for the y size to BASE_HEIGHT* m_aspectRatio* (BASE_ZOOM to the m_ZoomCount power)
	int y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_ZoomCount);
	//Assign m_plane_size with this new size
	m_plane_size = {x,y};
	//Set m_State to CALCULATING
	m_State = CALCULATING;
}

void ComplexPlane::zoomOut() 
{
	//Decrement m_zoomCount
	m_zoomCount--;
	//Set a local variable for the x size to BASE_WIDTH* (BASE_ZOOM to the m_ZoomCount power)
	int x = BASE_WIDTH * pow(BASE_ZOOM, m_ZoomCount);
	//Set a local variable for the y size to BASE_HEIGHT* m_aspectRatio* (BASE_ZOOM to the m_ZoomCount power)
	int y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_ZoomCount);
	//Assign m_plane_size with this new size
	m_plane_size = { x,y };
	//Set m_State to CALCULATING
	m_State = CALCULATING;
}

//void ComplexPlane::setCenter(Vector2i mousePixel) {}

//void ComplexPlane::setMouseLocation(Vector2i mousePixel) {}

//void ComplexPlane::loadText(Text& text) {}

//void ComplexPlane::updateRender() {}

//int ComplexPlane::countIterations(Vector2f coord) {}

//void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) {}

//Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) {}
