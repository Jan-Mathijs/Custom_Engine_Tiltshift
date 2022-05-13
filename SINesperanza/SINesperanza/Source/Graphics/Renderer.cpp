#include "Graphics/Renderer.h"

using namespace SIN;

Renderer::Renderer() :
	m_NumPoints(0),
	m_NumLines(0)
{
	m_Points = (Point*)malloc(sizeof(Point) * g_MaxPointsLines); //Use the same cap for lines and points
	m_Lines = (Line*)malloc(sizeof(Line) * g_MaxPointsLines);
}

Renderer::~Renderer()
{
	
}

bool Renderer::MakePoint(float x, float y, float size)
{
	if (m_NumPoints >= g_MaxPointsLines)
		return false;
	m_Points[m_NumPoints++] = Point(x, y, size);
	return true;
}

bool Renderer::MakeLine(float x0, float y0, float x1, float y1)
{
	if (m_NumLines >= g_MaxPointsLines)
		return false;
	m_Lines[m_NumLines++] = Line(x0, y0, x1, y1);
	return true;
}