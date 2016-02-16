#ifndef CIRCLE_H
#define CIRCLE_H


#include "SFML/Graphics.hpp"





class Circle{

public:
	Circle();
	~Circle();
	Circle(sf::CircleShape circle1, bool start,bool goal,std::string name);
	
	
	
	void setStart(bool boolean);
	void setGoal(bool boolean);
	void setPath(bool boolean);
	void setAttemptedPath(bool boolean);
	sf::CircleShape getCircle();
	bool getStart();
	bool getGoal();
	std::string getName();
	void Update();




private:

	bool m_start;
	bool m_goal;
	std::string m_name;
	sf::CircleShape circle;
	bool path = false;
	bool attemptedPath = false;

};



#endif