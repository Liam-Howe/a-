

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
typedef GraphNode<tuple<string, float,float,float,float>, int > Node;

void visit(Node * pNode) {
	cout << "Visiting: Node = " << get<0>(pNode->data()) << " HN = " << get<1>(pNode->data()) << " GN = " << get<2>(pNode->data()) << " FN = " << get<2>(pNode->data()) + get<1>(pNode->data()) << endl;

}




int main(int argc, char *argv[])
{
	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SFML First Program");//the render window
	int maxNodes = 30;//maximum number of nodes
	vector<Circle> NodesCircles;//vector of circles used to represent the nodes on screen 
	vector<sf::Text>GNS;//vect of tects used to display each nodes GN
	vector<sf::Text>HNS;//vect of tects used to display each nodes HN
	vector<sf::Text>FNS;//vect of tects used to display each nodes FN
	std::vector<Node*> path;//holds the path taken
	std::vector<Node*> attemptedpath;//holad all attempted routes
	bool startSelected = false;//has the origin node been selected
	bool goalSelected = false;//has the goal node been selected
	vector<sf::Text> NodesNames;//hold a vector of texts to display nodes names on screen 
	vector<sf::Text> WeightText;//holds a vector of texts to display the weight of each arc on screen 
	vector<sf::VertexArray> Arcs;// holds vertex arrays used to draw a line between to nodes. represents the arcs
	bool runAstar = false;//should the a* be called
	sf::Font font;//font for displaying sf Text
	sf::Texture startbuttonTex, resetTex1,resetTex2;//textures to represent the start and reset button on screen 
	sf::Sprite startbuttonSprite,resetSprite;//sprites for the buttonds

	sf::Text UI1;//used to show what the cyan colour Text represents
	UI1.setFont(font);
	UI1.setCharacterSize(20);
	UI1.setColor(sf::Color::Cyan);
	UI1.setPosition(sf::Vector2f(650, 50));
	UI1.setString("GN = CYAN");

	sf::Text UI2;//used to show what White Text  represents
	UI2.setFont(font);
	UI2.setCharacterSize(20);
	UI2.setColor(sf::Color::White);
	UI2.setPosition(sf::Vector2f(650, 100));
	UI2.setString("FN = White");

	sf::Text UI3;//used to show what Magenta Text  represents
	UI3.setFont(font);
	UI3.setCharacterSize(20);
	UI3.setColor(sf::Color::Magenta);
	UI3.setPosition(sf::Vector2f(650, 150));
	UI3.setString("HN = MAGENTA");
	

	startbuttonTex.loadFromFile("startbutton.png");//load the texture for start button
	resetTex1.loadFromFile("reset1.png");//load reset button texture
	resetTex2.loadFromFile("reset2.png");
	resetSprite.setTexture(resetTex1);//sets the sprite texture to that witch was loaded
	resetSprite.setPosition(sf::Vector2f(300, 500));//sets sprites position

	startbuttonSprite.setTexture(startbuttonTex);
	startbuttonSprite.setTextureRect(sf::IntRect(0, 0, 201, 71));
	startbuttonSprite.setPosition(sf::Vector2f(100, 500));
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");

	sf::Text startText;//used for displaying witch number node was selected for origin 
	startText.setPosition(sf::Vector2f(650, 200));
	sf::Text goalText;//used for displaying witch number node was selected for goal 
	goalText.setPosition(sf::Vector2f(650, 250));

	startText.setFont(font);
	startText.setStyle(sf::Text::Bold);
	startText.setCharacterSize(20);
	startText.setColor(sf::Color::White);

	goalText.setFont(font);
	goalText.setStyle(sf::Text::Bold);
	goalText.setCharacterSize(20);
	goalText.setColor(sf::Color::White);


	Graph<tuple<string, float, float, float, float>, int> mygraph(maxNodes);//The Graph. takes in a tuple contaings <name,hn,gn,xpositiom,yposition>
	string c;
	int i = 0;
	ifstream myfile;
	int hn, gn, x, y;
	myfile.open ("nodes.txt");

	while ( myfile >> c >> x >> y) {

		mygraph.addNode(tuple<string, float, float, float, float>(c, 9999, 9999, x, y), i++);

	}

	myfile.close();



	myfile.open("arcs.txt");
	int from, to, weight;
	while ( myfile >> from >> to >> weight ) {
	mygraph.addArc(from, to, weight);

	//////////////////////////////////////////////////////////
	sf::VertexArray lines(sf::LinesStrip, 2);
	lines[0].position = sf::Vector2f(get<3>(mygraph.nodeArray()[from]->data()), get<4>(mygraph.nodeArray()[from]->data())) + sf::Vector2f(50, 50);
	lines[0].color =sf::Color::Yellow;
	lines[1].position = sf::Vector2f(get<3>(mygraph.nodeArray()[to]->data()), get<4>(mygraph.nodeArray()[to]->data())) + sf::Vector2f(50, 50);
	lines[1].color = sf::Color::Yellow;
	//////////////////////////////////////////////////////////Gets the positions of the nodes and draws a line between 2 of them

	sf::Text arcWeight;//used to show the arcs weight on screen 
	arcWeight.setFont(font);
	arcWeight.setStyle(sf::Text::Bold);
	arcWeight.setCharacterSize(20);
	arcWeight.setColor(sf::Color::Red);
	arcWeight.setString(std::to_string(weight));
	arcWeight.setOrigin(arcWeight.getGlobalBounds().width / 2, arcWeight.getGlobalBounds().width / 2);//for drawing
	arcWeight.setPosition(sf::Vector2f((lines[1].position.x + lines[0].position.x) / 2, (lines[1].position.y + lines[0].position.y) / 2));

	WeightText.push_back(arcWeight);//pushes the weight into a list for drawing later
	Arcs.push_back(lines);//pushes the arcs into the arc list for drawing later
	}
	myfile.close();

	for (int i = 0; i < maxNodes; i++)//loop through all nodes
	{
		sf::CircleShape temp(20);//create a circle of radius 20
		temp.setFillColor(sf::Color::Yellow);//set is colour to yellow
		temp.setOrigin(20, 20);//set its origin to the center
		
		temp.setPosition(sf::Vector2f(get<3>(mygraph.nodeArray()[i]->data()), get<4>(mygraph.nodeArray()[i]->data())) + sf::Vector2f(50, 50));//get the nodes position and set the circle to it. add 50,50, so that nodes dont draw in top left corner
		Circle Tempcircle(temp, false, false, get<0>(mygraph.nodeArray()[i]->data()));//create a Circle object and pass in the sf circle shape. and make sure that is start bool and goal bool are initilied to false
		NodesCircles.push_back(Tempcircle);//push the circle into the list

		sf::Text nodename;//
		nodename.setFont(font);
		nodename.setStyle(sf::Text::Bold);
		nodename.setCharacterSize(20);
		nodename.setColor(sf::Color::Black);
		nodename.setString(get<0>(mygraph.nodeArray()[i]->data()));//gets the nodes name
		nodename.setOrigin(nodename.getGlobalBounds().width / 2, nodename.getGlobalBounds().height / 2);
		nodename.setPosition(temp.getPosition());
		NodesNames.push_back(nodename);//pushes the name to the nodes name vector to draw later

	}


	
	int start = 0;//the start node
	int goal = 0;//the origin node


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

		sf::Vector2i Mouseposition = sf::Mouse::getPosition(window) ;//get the mouses position
		startText.setString("Start = " + std::to_string(start));//updates the start text to display the current start node
		goalText.setString("End = " + std::to_string(goal));//update the goal node to display the current goal node

		if (Mouseposition.x > startbuttonSprite.getGlobalBounds().left
			&& Mouseposition.x < (startbuttonSprite.getGlobalBounds().left + startbuttonSprite.getGlobalBounds().width)
			&& Mouseposition.y > startbuttonSprite.getGlobalBounds().top
			&& Mouseposition.y < (startbuttonSprite.getGlobalBounds().top + startbuttonSprite.getGlobalBounds().height))
		{//if the mouse is inside the start button
		
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && startSelected == true && goalSelected == true)// if the mouse is clicked and both the origin and strt node are selected
			{
				GNS.clear();//clear the gns list
				HNS.clear();//clear the hns list
				FNS.clear();//clear the fns list
				runAstar = true;//reun the astar algoritihm
				startSelected = false;//unselect start
				goalSelected = false;//unselect goal 
				NodesCircles[start].setStart(false);//reset the start circle so it is nolonger start
				NodesCircles[goal].setGoal(false);//reset the goal circle so it is no linger goal
			}
		}



		if (Mouseposition.x > resetSprite.getGlobalBounds().left
			&& Mouseposition.x < (resetSprite.getGlobalBounds().left + resetSprite.getGlobalBounds().width)
			&& Mouseposition.y > resetSprite.getGlobalBounds().top
			&& Mouseposition.y < (resetSprite.getGlobalBounds().top + resetSprite.getGlobalBounds().height))
		{//if mouse is in the reset button 

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))//if mouse is clicked
			{
				
		
				GNS.clear();
				HNS.clear();
				FNS.clear();
				path.clear();
				attemptedpath.clear();
				//clear the lists
				///////////////////////////
				mygraph.clearMarks();//clear all the marked nodes so thy are no longer marked
				mygraph.reset();//calls a reset method that resets the previous nodes and sets gn and hn back to 9999
				goal = 0;//reset goal 
				start = 0;//reset start
				startSelected = false;//reset 
				goalSelected = false;//reset

				for (int i = 0; i < NodesCircles.size(); i++)
				{
			
					NodesCircles[i].setGoal(false);
					NodesCircles[i].setPath(false);
					NodesCircles[i].setAttemptedPath(false);
					NodesCircles[i].setStart(false);
				}//Reset all circles back to scratch
				
		
			}
		}
	



		if (runAstar)//when the a star is called
		{	
			cout << '\n' << "A *: " << endl;

			path.clear();
			mygraph.aStar(mygraph.nodeArray()[start], mygraph.nodeArray()[goal], visit, path,attemptedpath);//reuns the A Star

			for (Node* n : path)
				visit(n);


			for (int i = 0; i < maxNodes; i++)
			{
				
				sf::Text GN;
				GN.setFont(font);
				GN.setCharacterSize(15);
				GN.setOrigin(GN.getGlobalBounds().width / 2, GN.getGlobalBounds().height / 2);
				GN.setColor(sf::Color::Cyan);
				int gn = get<2>(mygraph.nodeArray()[i]->data());//gets the Gn value 
				if (get<2>(mygraph.nodeArray()[i]->data()) >= 9999)
				{
					GN.setString("-");
				}
				else
				{

					GN.setString(std::to_string(gn));//sets the texts string to be gn value
				}

				GN.setPosition(sf::Vector2f(get<3>(mygraph.nodeArray()[i]->data()) + 25, get<4>(mygraph.nodeArray()[i]->data()) + 15));//sets it positon to top left corner of the node

				GNS.push_back(GN);//pushes into list for drawing later

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				sf::Text HN;
				HN.setFont(font);
				HN.setCharacterSize(15);
				HN.setOrigin(HN.getGlobalBounds().width / 2, HN.getGlobalBounds().height / 2);
				HN.setColor(sf::Color::Magenta);
				int hn = get<1>(mygraph.nodeArray()[i]->data());
				if (get<2>(mygraph.nodeArray()[i]->data()) >= 9999)
				{
					HN.setString("-");
				}
				else
				{

					HN.setString(std::to_string(hn));//sets the hn string to the hn value
				}

				HN.setPosition(sf::Vector2f(get<3>(mygraph.nodeArray()[i]->data()) + 65, get<4>(mygraph.nodeArray()[i]->data()) + 15));//sets the hn position to top right of node

				HNS.push_back(HN);//pushes it into list for drawing later



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				sf::Text FN;
				FN.setFont(font);
				FN.setCharacterSize(15);
				FN.setOrigin(FN.getGlobalBounds().width / 2, FN.getGlobalBounds().height / 2);
				FN.setColor(sf::Color::White);
				int fn = get<2>(mygraph.nodeArray()[i]->data()) + get<1>(mygraph.nodeArray()[i]->data());//value = gn + hn
				if (fn >= 9999)
				{
					FN.setString("-");
				}
				else
				{

					FN.setString(std::to_string(fn));//set string to fn value
				}

				FN.setPosition(sf::Vector2f(get<3>(mygraph.nodeArray()[i]->data()) + 25, get<4>(mygraph.nodeArray()[i]->data()) + 65));//set position to lower left of node

				FNS.push_back(FN);//push into list for drawing later

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}


			runAstar = false;//sets back to false to stop repeat call
		}

		// prepare frame
			window.clear();
	
     	for (int i = 0; i < path.size(); i++)
		{
			for (int j = 0; j < NodesCircles.size(); j++)
			{
				if (get<0>(path[i]->data()) == NodesCircles[j].getName())
				{
					NodesCircles[j].setPath(true);//this will make the circles in path turn blue
				}
			}
			
		}
		for (int i = 0; i < attemptedpath.size(); i++)
		{
			for (int j = 0; j < NodesCircles.size(); j++)
			{
				if (get<0>(attemptedpath[i]->data()) == NodesCircles[j].getName())
				{
					NodesCircles[j].setAttemptedPath(true);//this will make all attempted circles turn whit
				}
			}
		}
	
		//draw frame items

		for (int i = 0; i < Arcs.size(); i++)
		{
			window.draw(Arcs[i]);//draws all arcs 
			window.draw(WeightText[i]);//draws all arc weights
		}
		for (int i = 0; i < NodesCircles.size(); i++)
		{
			NodesCircles[i].Update();//update all circles 

			float distanceX = (Mouseposition.x) - (NodesCircles[i].getCircle().getPosition().x);
			float distanceY = (Mouseposition.y) - (NodesCircles[i].getCircle().getPosition().y);
			float distance = sqrt((distanceX * distanceX) + (distanceY * distanceY));//gets mouse distance to circle center

			if (distance < (NodesCircles[i].getCircle().getRadius()) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && startSelected == false && NodesCircles[i].getGoal()==false)//chekes if mouse is in circle an left mouse pressed
			{
			
				NodesCircles[i].setStart(true);
				start = i;//sets this node to origin
				startSelected = true;

			}
			if (distance < (NodesCircles[i].getCircle().getRadius()) && sf::Mouse::isButtonPressed(sf::Mouse::Right) && goalSelected == false && NodesCircles[i].getStart() == false)//chekes if mouse is in circle an Right mouse pressed
			{

				NodesCircles[i].setGoal(true);
				goal = i;//sets this node to goal
				goalSelected = true;

			}
			
			window.draw(startText);
			window.draw(goalText);
			window.draw(resetSprite);
			window.draw(startbuttonSprite);
			window.draw(NodesCircles[i].getCircle());
			window.draw(NodesNames[i]);

		}

		
		for (int i = 0; i < GNS.size(); i++)
		{
			window.draw(GNS[i]);
			window.draw(HNS[i]);
			window.draw(FNS[i]);
		}
		
		
		window.draw(UI1);
		window.draw(UI2);
		window.draw(UI3);
		// Finally, display rendered frame on screen 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
}
