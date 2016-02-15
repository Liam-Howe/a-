


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
using namespace std;


//typedef GraphArc<pair<string, int>, int > Arc;
typedef GraphNode<tuple<string, int,int,int,int>, int > Node;

void visit(Node * pNode) {
	cout << "Visiting: Data= " << get<0>(pNode->data()) << " Cost= " << get<1>(pNode->data()) << endl;
}

int main(int argc, char *argv[])
{
	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SFML First Program");

	
	// Graph<pair<string, int>,int>graph( 6 );
	Graph<tuple<string, int, int, int, int>, int> myTuple(30);
	string c;
	int i = 0;
	ifstream myfile;
	int hn, gn, x, y;
	myfile.open ("nodes.txt");

	while ( myfile >> c >> hn >> gn >> x>> y) {

	myTuple.addNode(tuple<string, int,int,int,int>(c, hn,gn,x,y), i++);
	int i = 0;
	}

	myfile.close();
	myfile.open("arcs3.txt");

	int from, to, weight;
	while ( myfile >> from >> to >> weight ) {
	myTuple.addArc(from, to, weight);
	}
	myfile.close();


	std::vector<Node*> path;
	cout << '\n' << "A *: " << endl;
	//.clearMarks();

	//path.clear();
	myTuple.aStar(myTuple.nodeArray()[0], myTuple.nodeArray()[5], visit, path);
	
	
	for (Node* n : path)
	visit(n);
	




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

		//prepare frame
		window.clear();

		//draw frame items
		


		// Finally, display rendered frame on screen 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
}
