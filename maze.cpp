#include "maze.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <random>
#include <string>
//#include <dos.h>

using namespace std;


Maze::Maze() {
	//Open SDL
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	//Create window and renderer
	SDL_CreateWindowAndRenderer(1920, 1080, 0, &window, &renderer);
	//Make renderer black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);	
	
	


	
	Sizebutton_builder();								//makes the graph size button(blue) and exit button(red)
	build_grid(); //done								//builds a grid
	adjMat_Builder(); //done							//makes an adjMat matrix that would be used in prims
	rand_prims(realAdjMat, realAdjMat.size());			//creates a maze using prims algorithm


	

	//Upon quit, clean up SDL resources
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}





/*
repeat funtion that clears every vector and start a new maze
This function is called when a different size is clicked.
*/
void Maze::repeat(){
	grid.clear();
	edges.clear();
	weights.clear();
	adjMat.clear();
	realAdjMat.clear();
	pathway.clear();
	blackWindow();
	build_grid(); 
	adjMat_Builder(); 
	rand_prims(realAdjMat, realAdjMat.size());

}
//prints text, called after making each button
void Maze::text(string textureText){
	//font variable, openfont(file, font_size)
	TTF_Font * ourFont;
	ourFont = TTF_OpenFont("font/Montserrat-Regular.ttf", 64);
	//error check
	if(ourFont == nullptr){
		cout << "no font load :( \n";
		exit(1);
	}
	

    //make text a surface then convert the surface to a texture, renderText(font, c_string, color)
	SDL_Surface * surface = TTF_RenderText_Solid (ourFont, textureText.c_str(), { 255, 255, 255 });
	if(surface == nullptr){
		cout << "pain  " << TTF_GetError() << "\n";
	}
	SDL_Texture * texture;
	//convert surface to texture
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	//make a rectangle to hold text
	//each else if statement is for different printing 
	SDL_Rect rectangle;
	if(textureText == "5 x 5"){
		rectangle.x = 1650;
		rectangle.y = 250;
		rectangle.w = 100;
		rectangle.h = 50;
	}
	else if(textureText == "10 x 10"){
		rectangle.x = 1625;
		rectangle.y = 425;
		rectangle.w = 150;
		rectangle.h = 50;
	}else if(textureText == "25 x 25"){
		rectangle.x = 1625;
		rectangle.y = 600;
		rectangle.w = 150;
		rectangle.h = 50;
	}else if(textureText == "50 x 50"){
		rectangle.x = 1625;
		rectangle.y = 775;
		rectangle.w = 150;
		rectangle.h = 50;
	}else if(textureText == "100 x 100"){
		rectangle.x = 1625;
		rectangle.y = 950;
		rectangle.w = 150;
		rectangle.h = 50;
	}
	else if(textureText == "Quit"){
		rectangle.x = 1400;
		rectangle.y = 75;
		rectangle.w = 100;
		rectangle.h = 50;
	}else if(textureText == "BFS"){
		rectangle.x = 1200;
		rectangle.y = 325;
		rectangle.w = 100;
		rectangle.h = 50;
	}else if(textureText == "DFS"){
		rectangle.x = 1200;
		rectangle.y = 575;
		rectangle.w = 100;
		rectangle.h = 50;
	}else if(textureText == "Djikstra's"){
		rectangle.x = 1150;
		rectangle.y = 825;
		rectangle.w = 200;
		rectangle.h = 50;
	}
   

	//while(!quit){
	SDL_RenderCopy(renderer, texture, NULL, &rectangle);
    SDL_RenderPresent(renderer);
	//}
	SDL_FreeSurface(surface);
}

//blackWindow Function makes the pixels where the maze is made black.
void:: Maze::blackWindow(){
	for(int i = 0; i < 1050; i++){
		for(int j = 0; j < 1080; j++){
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);		
			SDL_RenderDrawPoint(renderer, i, j);			
		}
	}
	SDL_RenderPresent(renderer);
}



//creates the size button and exit button
int Maze::Sizebutton_builder(){
	string five = "5 x 5"; // y = 1100 | x = 150
	string ten = "10 x 10";
	string twenty = "25 x 25";
	string fifty = "50 x 50";
	string hundred = "100 x 100";
	string quit = "Quit";
	//Makes the different maze size buttons in numerical order
	for(int i = 0; i < 300; i++){
		//5
		for(int j = 0; j < 100; j++){
			SDL_SetRenderDrawColor(renderer, 102, 51, 153, 255);		
			SDL_RenderDrawPoint(renderer, i+1550, j+225);			
		}

		//10
		for(int j = 0; j < 100; j++){
			SDL_SetRenderDrawColor(renderer, 102, 51, 153, 255);	
			SDL_RenderDrawPoint(renderer, i+1550, j+400);			
		}

		//25
		for(int j = 0; j < 100; j++){
			SDL_SetRenderDrawColor(renderer, 102, 51, 153, 255);		
			SDL_RenderDrawPoint(renderer, i+1550, j+575);			
		}

		//50
		for(int j = 0; j < 100; j++){
			SDL_SetRenderDrawColor(renderer, 102, 51, 153, 255);		
			SDL_RenderDrawPoint(renderer, i+1550, j+750);			
		}

		//100
		for(int j = 0; j < 100; j++){
			SDL_SetRenderDrawColor(renderer, 102, 51, 153, 255);	
			SDL_RenderDrawPoint(renderer, i+1550, j+925);			
		}
	}
		text(five);
		text(ten);
		text(twenty);
		text(fifty);
		text(hundred);
	
	//makes the exit button red
	for(int i = 0; i < 100; i++){
		//exit 
		for(int j = 0; j < 460; j++){
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);		
			SDL_RenderDrawPoint(renderer, j+1230, i+50);	
		}
	}
	text(quit);
	SDL_RenderPresent(renderer);


	//waits for the next button clicked
	bool notClicked = true;
	while(notClicked){
		if(SDL_PollEvent(&event) &&event.type == SDL_MOUSEBUTTONDOWN){
			if(event.button.button == SDL_BUTTON_LEFT ){
				int x,y;
				SDL_GetMouseState(&x,&y);
				if(x >= 1550 && x <= 1850){
					if(y > 225 && y < 325){
						size = 50;
						cell = 1000/(size/10);
						notClicked = false;
					}

					if(y > 400 && y < 500){
						size = 100;
						cell = 1000/(size/10);
						notClicked = false;
					}

					if(y > 575 && y < 675){
						size = 200;
						cell = 1000/(size/10);
						notClicked = false;
					}

					if(y > 750 && y < 850){
						size = 500;
						cell = 1000/(size/10);
						notClicked = false;
					}

					if(y > 925 && y < 1025){
						size = 1000;
						cell = 1000/(size/10);
						notClicked = false;
					}
				
				}

				if(x>=1230 && x<=1690){
					if(y > 50 && y < 150){
						notClicked = false;
						quit = true;
						return 0;
					}
				}
			}
		}
	}


}


void Maze::build_grid() {
	//makes the walls
	for(int i =0; i <= 1000; i++){
		vector<int> tmp;
		for(int j = 0; j <= 1000; j++){
			if (i % cell == 0 || j % cell == 0) {
				tmp.push_back(1);
			}
			else {
				tmp.push_back(0);
			}
		}
		grid.push_back(tmp);
	}
	

	//draws the grid and makes adjMatrix
	for (int i = 0; i < grid.size() ; ++i) {
		vector<int> tmp;
		for (int j = 0; j < grid[i].size(); ++j) {
			if (grid[i][j] == 1) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);		//Set draw color to white
				SDL_RenderDrawPoint(renderer, i, j);			
			}
			//start point
			if(i < cell && j < cell && grid[i][j] !=1){
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);			//sets color to green
				SDL_RenderDrawPoint(renderer, i, j);			
			}
			//end point
			if(i > 1000-cell && j > 1000-cell && grid[i][j] !=1 && i < 1000 && j <1000){
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);		//Set draw color to red
				SDL_RenderDrawPoint(renderer, i, j);			
			}

			if(i % (cell/2) == 0 && j % (cell/2) == 0 && grid[i][j] != 1){
				

				//makes nodes  
				pair<int, int> p(i,j);			//note: x,y are swapped, so i is y and j is x
				edges.push_back(p);


				//random number for 1-20 for weighted adjmat for prims
			  	std::random_device dev;
        		std::mt19937 rng(dev());
        		std::uniform_int_distribution<std::mt19937::result_type> dist20(1,20); // distribution in range [1,20]
				tmp.push_back(dist20(rng));
			}
		}
		if(tmp.size() != 0){
			adjMat.push_back(tmp);
		}
	}


	SDL_RenderPresent(renderer);		//Show updated render

}




void Maze::adjMat_Builder(){
	int maxSize = adjMat.size()*adjMat.size();
	
	//makes the realAdjMat that would be size of maze by size of maze
	//lets say we choose the 5 size maze than the realAdjMat would be 25*25
	for(int i = 0; i < maxSize; i++){
		vector<int> tmp; 
		for(int j = 0; j < maxSize; j++){
			int check =0;
			//up
			if(i>= adjMat.size()){
				if(j == i-adjMat.size()){
					check =1;
					int row = j/adjMat.size();
					int col = j%adjMat.size();
					tmp.push_back(adjMat[row][col]);
				}
			}
			
			//down
			if(i < maxSize-adjMat.size()){
				if(j == i+adjMat.size()){
					check =1;					
					int row = j/adjMat.size();
					int col = j%adjMat.size();
					tmp.push_back(adjMat[row][col]);
				}
			}

			//right
			if((i+1)%adjMat.size() != 0){
				if(j == i+1){
					check =1;
					int row = j/adjMat.size();
					int col = j%adjMat.size();
					tmp.push_back(adjMat[row][col]);
				}
			}

			//left
			if((i+1)%adjMat.size() != 1){
				if(j == i-1){
					check =1;
					int row = j/adjMat.size();
					int col = j%adjMat.size();
					tmp.push_back(adjMat[row][col]);
				}
			}

			if(check ==0){
				tmp.push_back(1000);
			}
		}
		for(int m = 0; m < tmp.size(); m++){
			cout << tmp[m] << " ";
		}
		cout << endl;
		realAdjMat.push_back(tmp);
		
	}
}



//basic prims algorithm
void Maze::rand_prims(vector<vector<int>> adj, int vert){
	    //used to store the new graph with lowest weight
    vector<int> newGraph(vert,0);
    
    int key[vert];
    
 	bool setChecker[vert];
    // Initialize all keys as and setchecker to false
    for (int i = 0; i < vert; i++){
        key[i] = 99999, setChecker[i] = false;
    }
 

    key[0] = 0;
   

    newGraph[0] = -1;

    for (int count = 0; count < vert - 1; count++) {
         
     
     int mintmp = 9999999;
     int index;
       //finds the lowest index and also sets the setChecker at that index true to signal that its been added
       for (int v = 0; v < adj.size(); v++){
          if (setChecker[v] == false && key[v] < mintmp){
            mintmp = key[v];
            index = v;
          }
        }
        
        setChecker[index] = true;

        for (int w = 0; w < vert; w++){
 
            if (adj[index][w] && setChecker[w] == false && adj[index][w] < key[w]){
                newGraph[w] = index;
                key[w] = adj[index][w];
            }

		}
    }


	gen_maze(newGraph, vert);
	newGraph.clear();
}




/*makes the maze and the buttons for different solving algorithms*/
int Maze::gen_maze(vector<int> mazeLayout,int vert){
	string bfs = "BFS";
	string dfs = "DFS";
	string djikstras = "Djikstra's";
	for(int i =1; i < vert; i++){
		int a = i;
		int b = mazeLayout[i];

		realAdjMat[a][b] =50;
		realAdjMat[b][a] = 50;

		if(a > b){
			int tmp = b;
			b = a;
			a = tmp;
		}

		
		//right and left
		if(a+1 == b){

			int j = edges[a].second+(cell/2);
			int k = edges[a].first-(cell/2)+1;
			for(int l = 0; l < (cell-1); l++){
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);		
				SDL_RenderDrawPoint(renderer, j, k);
				grid[j][k] = 0;			
				k++;
			}
		}

		//up and down
		else if(a+(size/10) == b){
			int j = edges[a].second-(cell/2)+1;
			int k = edges[a].first+(cell/2);

			for(int l =0; l < (cell-1); l++){
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);		
				SDL_RenderDrawPoint(renderer, j, k);			
				grid[j][k] =0;
				j++;
			}
		}

	}
				SDL_RenderPresent(renderer);
			usleep(5000000/(10*size));

	for(int j = 0; j < 100; j++){

		//BFS
		for(int i =0; i < 300; i++){
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);		
			SDL_RenderDrawPoint(renderer, i+1100, j+300);			
		}

		//DFS
		for(int i =0; i < 300; i++){
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);		
			SDL_RenderDrawPoint(renderer, i+1100, j+550);			
		}
		
		//Djikstra's
		for(int i =0; i < 300; i++){
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);		
			SDL_RenderDrawPoint(renderer, i+1100, j+800);			
		}

	}
	text(bfs);
	text(dfs);
	text(djikstras);
	

	SDL_RenderPresent(renderer);
	
	//makes every number in the realAdjMat to 1 so it can be used in djikstra's algorithm
	for(int i = 0; i < realAdjMat.size(); i++){
		for(int j = 0; j < realAdjMat.size(); j++){
			if(realAdjMat[i][j] != 50){
				realAdjMat[i][j] = 0;
			}
			else{
				int index = i+j;
				int row = i/adjMat.size();
				int col = j%adjMat.size();
				realAdjMat[i][j] =adjMat[row][col];
			}
			cout << realAdjMat[i][j] << " ";
		}
		cout << endl;
	}



	//waits for next button clicked
	bool notClicked = true;
	while(notClicked){
		int x,y;
		
		if(SDL_PollEvent(&event) &&event.type == SDL_MOUSEBUTTONDOWN){
			SDL_GetMouseState(&x,&y);
				
				if(x >= 1550 && x <= 1850){
					if(y > 225 && y < 325){
						size = 50;
						cell = 1000/(size/10);
						notClicked = false;
						repeat();
					}

					if(y > 400 && y < 500){
						size = 100;
						cell = 1000/(size/10);
						notClicked = false;
						repeat();
					}

					if(y > 575 && y < 675){
						size = 200;
						cell = 1000/(size/10);
						notClicked = false;
						repeat();
					}

					if(y > 750 && y < 850){
						size = 500;
						cell = 1000/(size/10);
						notClicked = false;
						repeat();
					}

					if(y > 925 && y < 1025){
						size = 1000;
						cell = 1000/(size/10);
						notClicked = false;
						repeat();
					}
				
				}

				if(x>=1230 && x<=1690){
					if(y > 50 && y < 150){
						notClicked = false;
						quit = true;
						return 0;
					}
				}

				//BFS 
				if(y >=325 && y <= 425){
					if(x>1100 && x<1400){
						notClicked = false;
						visited = vector<bool>(realAdjMat.size(), false);
						int parent[realAdjMat.size()];
						for(int i = 0; i < realAdjMat.size(); i++){
							pathway.push_back(-1);
						}
						BFS(0, parent);
						int endpoint = pathway[realAdjMat.size()-1];
						while(endpoint != 0){
							for(int i = 0; i < cell; i++){
								int a = edges[pathway[endpoint]].second-(cell/2);
								int b = edges[pathway[endpoint]].first- (cell/2);
								for(int j = 0; j < cell; j++){
									if(grid[i+a][j+b] != 1){
										SDL_SetRenderDrawColor(renderer, 48, 255, 96, 255);		//Set draw color to light green
										SDL_RenderDrawPoint(renderer, i+a, j+b);
									}
								}
								
							}
							endpoint = pathway[endpoint];
							SDL_RenderPresent(renderer);
							usleep(5000000/(size));
						}
						
						pathway.clear();
						int checker = decide();
						if(checker = -1){
							notClicked =false;
							quit = true;
							return 0;
						}
					}
				}
				
				//DFS
				if(y >= 550 && y <= 650){
					if(x>=1100 && x <=1400){
						notClicked = false;
						visited = vector<bool>(realAdjMat.size(), false);
						DFS(0);
						for(int k = pathway.size()-1; k > 0; k--){
							for(int i = 0; i < cell; i++){
								int a = edges[pathway[k]].second-(cell/2);
								int b = edges[pathway[k]].first- (cell/2);
								for(int j = 0; j < cell; j++){
									if(grid[i+a][j+b] != 1){
										SDL_SetRenderDrawColor(renderer, 48, 255, 96, 255);		//Set draw color to light green
										SDL_RenderDrawPoint(renderer, i+a, j+b);
									}
								}
							}
							SDL_RenderPresent(renderer);
							usleep(5000000/(size));
						}
						pathway.clear();
						int checker = decide();
						if(checker == -1){
							notClicked = false;
							quit = true;
							return 0;
						}
					}
				}
				
			
		}

	}
	

}



int Maze::decide(){
bool notClicked = true;
	while(notClicked){
		int x,y;
		if(SDL_PollEvent(&event) &&event.type == SDL_MOUSEBUTTONDOWN){
			SDL_GetMouseState(&x,&y);
				
				if(x >= 1550 && x <= 1850){
					if(y > 225 && y < 325){
						size = 50;
						cell = 1000/(size/10);
						notClicked = false;
						repeat();
					}

					if(y > 400 && y < 500){
						size = 100;
						cell = 1000/(size/10);
						notClicked = false;
						repeat();
					}

					if(y > 575 && y < 675){
						size = 200;
						cell = 1000/(size/10);
						notClicked = false;
						repeat();
					}

					if(y > 750 && y < 850){
						size = 500;
						cell = 1000/(size/10);
						notClicked = false;
						repeat();
					}

					if(y > 925 && y < 1025){
						size = 1000;
						cell = 1000/(size/10);
						notClicked = false;
						repeat();
					}
				
				}

				if(x>=1230 && x<=1690){
					if(y > 50 && y < 150){
						notClicked = false;
						quit = true;
						return -1;
					}
				}

				//BFS 
				if(y >=325 && y <= 425){
					if(x>1100 && x<1400){
						for(int i = 0; i< 1001; i++){
							for(int j = 0; j < 1001; j++){
								if(grid[i][j] != 1){
										SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);		//Set draw color to black
										SDL_RenderDrawPoint(renderer, i, j);
										if(i>0 && i < cell && j> 0 && j < cell){
											SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);		//Set draw color to green
											SDL_RenderDrawPoint(renderer, i, j);
										}
										if(i>1001-cell && i < 1001 && j> 1001-cell && j < 1001){
											SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);		//Set draw color to red
											SDL_RenderDrawPoint(renderer, i, j);
										}
										
								}
							}
						}
						notClicked = false;
						visited = vector<bool>(realAdjMat.size(), false);
						int parent[realAdjMat.size()];
						for(int i = 0; i < realAdjMat.size(); i++){
							pathway.push_back(-1);
						}
						BFS(0, parent);
						int endpoint = pathway[realAdjMat.size()-1];
						while(endpoint != 0){
							for(int i = 0; i < cell; i++){
								int a = edges[pathway[endpoint]].second-(cell/2);
								int b = edges[pathway[endpoint]].first- (cell/2);
								for(int j = 0; j < cell; j++){
									if(grid[i+a][j+b] != 1){
										SDL_SetRenderDrawColor(renderer, 48, 255, 96, 255);		//Set draw color to light green
										SDL_RenderDrawPoint(renderer, i+a, j+b);
									}
								}
								
							}
							endpoint = pathway[endpoint];
							SDL_RenderPresent(renderer);
							usleep(5000000/(size));
						}
						
						pathway.clear();
						int checker = decide();
						if(checker = -1){
							notClicked =false;
							quit = true;
							return 0;
						}
					}
				}
				
				//DFS
				if(y >= 550 && y <= 650){
					if(x>=1100 && x <=1400){
						for(int i = 0; i< 1001; i++){
							for(int j = 0; j < 1001; j++){
								if(grid[i][j] != 1){
										SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);		//Set draw color to black
										SDL_RenderDrawPoint(renderer, i, j);
										if(i>0 && i < cell && j> 0 && j < cell){
											SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);		//Set draw color to green
											SDL_RenderDrawPoint(renderer, i, j);
										}
										if(i>1001-cell && i < 1001 && j> 1001-cell && j < 1001){
											SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);		//Set draw color to red
											SDL_RenderDrawPoint(renderer, i, j);
										}
										
								}
							}
						}
						notClicked = false;
						visited = vector<bool>(realAdjMat.size(), false);
						DFS(0);
						for(int k = pathway.size()-1; k > 0; k--){
							for(int i = 0; i < cell; i++){
								int a = edges[pathway[k]].second-(cell/2);
								int b = edges[pathway[k]].first- (cell/2);
								for(int j = 0; j < cell; j++){
									if(grid[i+a][j+b] != 1){
										SDL_SetRenderDrawColor(renderer, 48, 255, 96, 255);		//Set draw color to light green
										SDL_RenderDrawPoint(renderer, i+a, j+b);
									}
								}
							}
							SDL_RenderPresent(renderer);
							usleep(5000000/(size));
						}
						pathway.clear();
						int checker = decide();
						if(checker == -1){
							notClicked = false;
							quit = true;
							return 0;
						}
					}
				}
				
			
		}

	}
}


int Maze::BFS(int start, int parent[]){
	vector<int> q;
    q.push_back(start); 
	 
	pathway[0] = -1;
    // Set source as visited
    visited[start] = true;
    int vis;
    while (!q.empty()) {
		
        vis = q[0];
		if(vis == realAdjMat.size() -1){
			return 0;
		}
		int a = edges[vis].second;
		int b = edges[vis].first;
		a = a-(cell/2);
		b = b-(cell/2);
		for(int i = 0; i < cell; i++){
			for(int j = 0; j < cell; j++){
				if(grid[i+a][j+b] != 1){
					SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);		//Set draw color to gray
					SDL_RenderDrawPoint(renderer, i+a, j+b);
				}
			}
		}
		SDL_RenderPresent(renderer);
		usleep(5000000/(10*size));

        q.erase(q.begin());
  
        // For every adjacent vertex to the current vertex
        for (int i = 0; i < realAdjMat[vis].size(); i++) {
			
            if (realAdjMat[vis][i] == 1 && (!visited[i])) {
				if(realAdjMat[vis][i] == realAdjMat.size()){
					q.clear();
					return 0;
				}
                // Push the adjacent node to the queue
                q.push_back(i);
				pathway[i] = vis;
                // Set
                visited[i] = true;
            }
        }
    }
}



int Maze::DFS(int start){

	int check = 0;

	int a = edges[start].second;
	int b = edges[start].first;
	a = a-(cell/2);
	b = b-(cell/2);
	
	if(start == realAdjMat.size()-1){
		pathway.push_back(start);
		return -1;
	}
	
	for(int i = 0; i < cell; i++){
		for(int j = 0; j < cell; j++){
			if(grid[i+a][j+b] != 1){
				SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);		//Set draw color to gray
				SDL_RenderDrawPoint(renderer, i+a, j+b);
			}
		}
	}
	SDL_RenderPresent(renderer);
	usleep(5000000/(10*size));

  // Set current node as visited
  visited[start] = true;
 
  // For every node of the graph
  for (int i = 0; i < realAdjMat[start].size(); i++) {
 
  	// If some node is adjacent to the current node
   	// and it has not already been visited
    if (realAdjMat[start][i] == 1 && (!visited[i])) {
    	check = DFS(i);
			if(check == -1){
				pathway.push_back(start);
				return -1;
			}
    }
  }
}

