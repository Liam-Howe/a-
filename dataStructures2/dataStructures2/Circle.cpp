#include "stdafx.h"
#include "Circle.h"


Circle::~Circle()
{


}
Circle::Circle()
{

}
Circle::Circle(sf::CircleShape circle1, bool start, bool goal,std::string name)
{
	circle = circle1;
	m_start = start;
	m_goal = goal;
	m_name = name;
	
}



void Circle::Update()
{
	if (m_start == false && m_goal == false && path == false)
	{
		circle.setFillColor(sf::Color::Yellow);
	}
	 else if (m_start == true && m_goal == false )
	{
		circle.setFillColor(sf::Color::Green);
	}
	else if (m_start == false && m_goal == true)
	{
		circle.setFillColor(sf::Color::Red);
	}
	else if ( path == true)
	{
		circle.setFillColor(sf::Color::Blue);
	}
	else if (path == false && attemptedPath == true)
	{
		circle.setFillColor(sf::Color::White);
	}

}



void Circle::setStart(bool boolean)
{

	m_start = boolean;
}
void Circle::setGoal(bool boolean)
{

	m_goal = boolean;
}
void Circle::setPath(bool boolean)
{

	path = boolean;
}

sf::CircleShape Circle::getCircle()
{

	return circle;
}
bool Circle::getStart()
{
	return m_start;
}
bool Circle::getGoal()
{
	return m_goal;
}
std::string Circle::getName()
{
	return m_name;
}