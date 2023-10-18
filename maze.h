#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <vector>
#include <string>
#ifndef MAZE_H
#define MAZE_H
using namespace std;

class Maze {
	public:
		Maze();							//Sets initial state of maze and SDL window
		void repeat();
		void blackWindow();
		int Sizebutton_builder();
		void build_grid();	//Builds the initial grid before maze construction
		void adjMat_Builder();
		void rand_prims(std::vector<std::vector<int>>, int);
		int gen_maze(vector<int>, int);
		int BFS(int, int[]);
		int BFS_decide();
		int DFS(int);
		int DFS_decide();
		int decide();
		void reset();
		void text(string textureText);
	private:
		//adjList represents the connected graph of each cell in the grid. This graph is used to 
		//construct a MST that determines the layout of the final maze
		int size;
		int cell;
		std::vector<int> pathway;
		std::vector<std::vector<int>> grid;
		std::vector<bool> visited;
		std::vector<std::pair<int,int>> edges;
		std::vector<int> weights;
		std::vector<std::vector<int>> adjMat; //cost matrix
		std::vector<std::vector<int>> realAdjMat; 
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Event event;			//Used to poll SDL window
		bool quit = false;		//Used to determine when to kill SDL processes
};

#endif
