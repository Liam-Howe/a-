

#include "stdafx.h"
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

#include <iostream>
#include <fstream>
#include <string>
#include "Graph.h"
#include "GraphArc.h"
//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>
#include "Circle.h"
using namespace std;


//typedef GraphArc<pair<string, int>, int > Arc;
typedef GraphNode<tuple<string, int,int,int,int>, int > Node;

void visit(Node * pNode) {
	cout << "Visiting: Node = " << get<0>(pNode->data()) << " HN = " << get<1>(pNode->data()) << " GN = " << get<2>(pNode->data()) << " FN = " << get<2>(pNode->data()) + get<1>(pNode->data()) << endl;
}


int main(int argc, char *argv[])
{
	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SFML First Program");
	int maxNodes = 30;
	vector<Circle> NodesCircles;

	std::vector<Node*> path;
	bool startSelected = false;
	bool goalSelected = false;
	vector<sf::Text> NodesNames;
	vector<sf::Text> WeightText;
	vector<sf::VertexArray> Arcs;
	bool runAstar = false;
	sf::Font font;
	sf::Texture startbuttonTex, resetTex1,resetTex2;
	sf::Sprite startbuttonSprite,resetSprite;

	startbuttonTex.loadFromFile("startbutton.png");
	resetTex1.loadFromFile("reset1.png");
	resetTex2.loadFromFile("reset2.png");
	resetSprite.setTexture(resetTex1);
	resetSprite.setPosition(sf::Vector2f(300, 500));

	startbuttonSprite.setTexture(startbuttonTex);
	startbuttonSprite.setTextureRect(sf::IntRect(0, 0, 201, 71));
	startbuttonSprite.setPosition(sf::Vector2f(100, 500));
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");

	// Graph<pair<string, int>,int>graph( 6 );
	Graph<tuple<string, int, int, int, int>, int> myTuple(maxNodes);
	string c;
	int i = 0;
	ifstream myfile;
	int hn, gn, x, y;
	myfile.open ("nodes.txt");

	while ( myfile >> c >> x >> y) {

	myTuple.addNode(tuple<string, int,int,int,int>(c, 9999,9999,x,y), i++);

	}

	myfile.close();



	myfile.open("arcs.txt");
	int from, to, weight;
	while ( myfile >> from >> to >> weight ) {
	myTuple.addArc(from, to, weight);


	sf::VertexArray lines(sf::LinesStrip, 2);
	lines[0].position = sf::Vector2f(get<3>(myTuple.nodeArray()[from]->data()), get<4>(myTuple.nodeArray()[from]->data())) + sf::Vector2f(50,50);
	lines[0].color =sf::Color::Yellow;
	lines[1].position = sf::Vector2f(get<3>(myTuple.nodeArray()[to]->data()), get<4>(myTuple.nodeArray()[to]->data())) + sf::Vector2f(50, 50);
	lines[1].color = sf::Color::Yellow;

	sf::Text arcWeight;
	arcWeight.setFont(font);
	arcWeight.setStyle(sf::Text::Bold);
	arcWeight.setCharacterSize(20);
	arcWeight.setColor(sf::Color::Red);
	arcWeight.setString(std::to_string(weight));
	arcWeight.setPosition(sf::Vector2f((lines[1].position.x + lines[0].position.x) / 2, (lines[1].position.y + lines[0].position.y) / 2));

	WeightText.push_back(arcWeight);
	Arcs.push_back(lines);
	}
	myfile.close();

	for (int i = 0; i < maxNodes; i++)
	{
		sf::CircleShape temp(15);
		temp.setFillColor(sf::Color::Yellow);
		temp.setOrigin(15, 15);
		temp.setPosition(sf::Vector2f(get<3>(myTuple.nodeArray()[i]->data()), get<4>(myTuple.nodeArray()[i]->data())) + sf::Vector2f(50, 50));
		Circle Tempcircle(temp, false, false, get<0>(myTuple.nodeArray()[i]->data()));
		NodesCircles.push_back(Tempcircle);

		sf::Text nodename;
		nodename.setFont(font);
		nodename.setStyle(sf::Text::Bold);
		nodename.setCharacterSize(20);
		nodename.setColor(sf::Color::Black);
		nodename.setString(get<0>(myTuple.nodeArray()[i]->data()));
		nodename.setPosition(temp.getPosition() - sf::Vector2f(5,15));
		NodesNames.push_back(nodename);

	}


	
	int start = 0;
	int goal = 0;



	




	// Start game loop 
	while (window.isOpen())
	{
		// Process events 
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			// Close window : exit 
			if (Event.type == sf::Event::Closed)
				window.close();

			// Escape key : exit 
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();


		}
		sf::Vector2i Mouseposition = sf::Mouse::getPosition(window) ;
	

		if (Mouseposition.x > startbuttonSprite.getGlobalBounds().left
			&& Mouseposition.x < (startbuttonSprite.getGlobalBounds().left + startbuttonSprite.getGlobalBounds().width)
			&& Mouseposition.y > startbuttonSprite.getGlobalBounds().top
			&& Mouseposition.y < (startbuttonSprite.getGlobalBounds().top + startbuttonSprite.getGlobalBounds().height))
		{
		
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && startSelected == true && goalSelected == true)
			{
				runAstar = true;
						startSelected = false;
				NodesCircles[start].setStart(false);
				goalSelected = false;
				NodesCircles[goal].setGoal(false);
			}
		}



		if (Mouseposition.x > resetSprite.getGlobalBounds().left
			&& Mouseposition.x < (resetSprite.getGlobalBounds().left + resetSprite.getGlobalBounds().width)
			&& Mouseposition.y > resetSprite.getGlobalBounds().top
			&& Mouseposition.y < (resetSprite.getGlobalBounds().top + resetSprite.getGlobalBounds().height))
		{
			resetSprite.setTexture(resetTex2);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				path.clear();
				myTuple.clearMarks();
				myTuple.reset();
				goal = 0;
				start = 0;
				startSelected = false;
				goalSelected = false;

				for (int i = 0; i < NodesCircles.size(); i++)
				{
			
					NodesCircles[i].setGoal(false);
					NodesCircles[i].setPath(false);
					NodesCircles[i].setStart(false);
				}
				
		
			}
		}
	



		if (runAstar)
		{
		
			cout << '\n' << "A *: " << endl;

			path.clear();
			myTuple.aStar(myTuple.nodeArray()[start], myTuple.nodeArray()[goal], visit, path);

			for (Node* n : path)
				visit(n);
			runAstar = false;

		}

     	for (int i = 0; i < path.size(); i++)
		{
			for (int j = 0; j < NodesCircles.size(); j++)
			{
				if (get<0>(path[i]->data()) == NodesCircles[j].getName())
				{
					NodesCircles[j].setPath(true);
				}
			}
		}


		//prepare frame
		window.clear();

		//draw frame items

		for (int i = 0; i < Arcs.size(); i++)
		{
			window.draw(Arcs[i]);
			window.draw(WeightText[i]);
		}
		for (int i = 0; i < NodesCircles.size(); i++)
		{
			NodesCircles[i].Update();

			float distanceX = (Mouseposition.x) - (NodesCircles[i].getCircle().getPosition().x);
			float distanceY = (Mouseposition.y) - (NodesCircles[i].getCircle().getPosition().y);
			float distance = sqrt((distanceX * distanceX) + (distanceY * distanceY));

			if (distance < (NodesCircles[i].getCircle().getRadius()) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && startSelected == false && NodesCircles[i].getGoal()==false)
			{
			
				NodesCircles[i].setStart(true);
				start = i;
				startSelected = true;

			}
			if (distance < (NodesCircles[i].getCircle().getRadius()) && sf::Mouse::isButtonPressed(sf::Mouse::Right) && goalSelected == false && NodesCircles[i].getStart() == false)
			{

				NodesCircles[i].setGoal(true);
				goal = i;
				goalSelected = true;

			}
		
			window.draw(resetSprite);
			window.draw(startbuttonSprite);
			window.draw(NodesCircles[i].getCircle());
			window.draw(NodesNames[i]);
		}
	
	 

		// Finally, display rendered frame on screen 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
}
