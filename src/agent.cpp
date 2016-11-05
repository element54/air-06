/*
 * Daniel Vazquez
 * Aritificial Intelligence for Robotics
 * SS 2016
 * Assignment 6
 *
 * agent.cpp
 * */
#include "agent.hpp"
#include <iostream>
#include <queue>
#include <algorithm>
#include <queue>
#include <stack>
#include <cstdlib>
#include <list>

 Agent::Agent(Puzzle puzzle, Solver solver, Heuristic heuristic) :
 puzzle(puzzle), solver(solver), heuristic(heuristic) {

 }

 Agent::~Agent() {

 }

 void Agent::run() {

 	if (solver == Solver::GREEDY) {
 		cout << "Solver: Greedy Search" << endl;
 		if (heuristic == Heuristic::MISPLACED_TILES) {
 			cout << "Heuristic: Misplaced tiles" << endl;
 		}

 		else {
 			cout << "Heuristic: Manhattan distance" << endl;
 		}
 		greedy_search (heuristic);
 	}
 	if (solver == Solver::A_STAR) {
 		cout << "Solver: A*" << endl;
 		if (heuristic == Heuristic::MISPLACED_TILES) {
 			cout << "Heuristic: Misplaced tiles" << endl;
 		}

 		else {
 			cout << "Heuristic: Manhattan distance" << endl;
 		}
 		a_star(heuristic);
 	}
 }

 int Agent::misplaced_tiles(Puzzle puzzle) {
 	int counter = 1;
 	int mpt = 9;
 	for (int rows = 0; rows < NUM_ROWS; rows++) {
 		for (int cols = 0; cols < NUM_COLS; cols++) {
 			if ((puzzle[rows][cols] == counter) && (counter != 9))  {
 				mpt--;
 			}
 			counter++;
 		}
 	}
 	if (puzzle[2][2]==0){mpt--;}
	 //cout << "misplaced tiles " << endl;
 	return mpt;
 }

//int Agent::mkSwitches(Puzzle puzzle) {
//	int counter = 1;
//	for (int rows = 0; rows < NUM_ROWS; rows++) {
 //       for (int cols = 0; cols < NUM_COLS; cols++) {
  //          if (puzzle[rows][cols] == 0)  {
   //         	zeroPos = counter;
	//			zeroCol = cols;
	//			zeroRow = rows;
	//		}
	//	counter++;
    //    }
  // 	}
//	cout << zeroPos << endl;	
////	cout << zeroPos << endl;	
////	cout << zeroPos << endl;	
//}

 int Agent::manhattan_distance(Puzzle puzzle) {
 	int designatedPos[8][2];
 	designatedPos[0][0] = 2;
 	designatedPos[0][1] = 2;
 	designatedPos[1][0] = 0;
 	designatedPos[1][1] = 0;
 	designatedPos[2][0] = 0;
 	designatedPos[2][1] = 1;
 	designatedPos[3][0] = 0;
 	designatedPos[3][1] = 2;
 	designatedPos[4][0] = 1;
 	designatedPos[4][1] = 0;
 	designatedPos[5][0] = 1;
 	designatedPos[5][1] = 1;
 	designatedPos[6][0] = 1;
 	designatedPos[6][1] = 2;
 	designatedPos[7][0] = 2;
 	designatedPos[7][1] = 0;
 	designatedPos[8][0] = 2;
 	designatedPos[8][1] = 1;
 	int manhattanDistance = 0;	
	int counter = 1; // used to check if the tile is in the correct position
	for (int rows = 0; rows < NUM_ROWS; rows++) {
		for (int cols = 0; cols < NUM_COLS; cols++) {
			if (puzzle[rows][cols] != counter)  {
              	//cout << "Tile " <<  counter << " is misplaced "  << endl; 
				int ind = puzzle[rows][cols];
				int distanceX = std::abs(designatedPos[ind][0]-rows); 
				int distanceY = std::abs(designatedPos[ind][1]-cols);
				int totalDist = distanceX + distanceY;
				manhattanDistance += totalDist;
				//	cout << puzzle[rows][cols] << " is at pos " << rows <<", " << cols << 	" should be at " << 
	           		//designatedPos[ind][0] << ", " <<  designatedPos[ind][1] <<  "total distance: " << totalDist <<" MHD= " <<  manhattanDistance << endl;
			}
			counter++;
		}
	}
	return manhattanDistance;
}

void Agent::greedy_search(Heuristic heuristic) {
		//TODO
	//Note: In this assignment you must find the proper way to
	//keep track of repeated states.
	int fitnessValue;
	std::vector<Puzzle> fringe;
	std::vector<Puzzle> visited;
	if (heuristic == Heuristic::MISPLACED_TILES) {
		fitnessValue = misplaced_tiles(puzzle);
	} 
	else {
		fitnessValue = manhattan_distance(puzzle);
	}
	fringe.push_back(puzzle);
	bool solved = false;
	bool failed = false;
	int iterations = 0;
	while ((solved == false) && (failed == false)){
		if (fringe.empty()){ failed = false; break;	}
		if (fitnessValue == 0){ solved = true; break; }
	///////////////////////////////// making switches
		int counter = 1;
		int zeroPos;
		int zeroCol;
		int zeroRow;

		for (int rows = 0; rows < NUM_ROWS; rows++) {
			for (int cols = 0; cols < NUM_COLS; cols++) {
				if (puzzle[rows][cols] == 0)  {
					zeroPos = counter;
					zeroCol = cols;
					zeroRow = rows;
				}
				counter++;
			}
		} 

		Puzzle puzzle_tmp = puzzle;
		if (zeroPos == 1) {
			
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow+1][zeroCol]; 
			puzzle_tmp[zeroRow+1][zeroCol] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow][zeroCol+1]; 
			puzzle_tmp[zeroRow][zeroCol+1] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}		
		}


		if (zeroPos == 2) {
			
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow+1][zeroCol]; 
			puzzle_tmp[zeroRow+1][zeroCol] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}			
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow][zeroCol+1]; 
			puzzle_tmp[zeroRow][zeroCol+1] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow][zeroCol-1]; 
			puzzle_tmp[zeroRow][zeroCol-1] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}		
		}


		if (zeroPos == 3) {
			
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow+1][zeroCol]; 
			puzzle_tmp[zeroRow+1][zeroCol] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow][zeroCol-1]; 
			puzzle_tmp[zeroRow][zeroCol-1] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}		
		}


		if (zeroPos == 4) {
			
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow+1][zeroCol]; 
			puzzle_tmp[zeroRow+1][zeroCol] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}			
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow][zeroCol+1]; 
			puzzle_tmp[zeroRow][zeroCol+1] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow-1][zeroCol];
			puzzle_tmp[zeroRow-1][zeroCol] = puzzle[zeroRow][zeroCol];
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}
		}


		if (zeroPos == 5) {
			
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow+1][zeroCol]; 
			puzzle_tmp[zeroRow+1][zeroCol] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}


			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow][zeroCol+1]; 
			puzzle_tmp[zeroRow][zeroCol+1] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}

			Puzzle puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow-1][zeroCol];
			puzzle_tmp[zeroRow-1][zeroCol] = puzzle[zeroRow][zeroCol];
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}

			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow][zeroCol-1];
			puzzle_tmp[zeroRow][zeroCol-1] = puzzle[zeroRow][zeroCol];
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}


		}

		if (zeroPos == 6) {
			
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow+1][zeroCol]; 
			puzzle_tmp[zeroRow+1][zeroCol] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow][zeroCol-1]; 
			puzzle_tmp[zeroRow][zeroCol-1] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}
			Puzzle puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow-1][zeroCol];
			puzzle_tmp[zeroRow-1][zeroCol] = puzzle[zeroRow][zeroCol];
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}
		} 

		if (zeroPos == 7) {
			
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow-1][zeroCol]; 
			puzzle_tmp[zeroRow-1][zeroCol] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow][zeroCol+1]; 
			puzzle_tmp[zeroRow][zeroCol+1] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}	
		}


		if (zeroPos == 8) {
			
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow-1][zeroCol]; 
			puzzle_tmp[zeroRow-1][zeroCol] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow][zeroCol+1]; 
			puzzle_tmp[zeroRow][zeroCol+1] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}
			Puzzle puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow][zeroCol-1];
			puzzle_tmp[zeroRow][zeroCol-1] = puzzle[zeroRow][zeroCol];
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}
		}


		if (zeroPos == 9) {
			
			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow-1][zeroCol]; 
			puzzle_tmp[zeroRow-1][zeroCol] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}			puzzle_tmp = puzzle;
			puzzle_tmp[zeroRow][zeroCol] = puzzle[zeroRow][zeroCol-1]; 
			puzzle_tmp[zeroRow][zeroCol-1] = puzzle[zeroRow][zeroCol]; 
			if (((std::find(fringe.begin(), fringe.end(),puzzle_tmp)!=fringe.end())==false) && ((std::find(visited.begin(), visited.end(),puzzle_tmp)!=visited.end())==false) )  {
				fringe.push_back(puzzle_tmp);
			}	
		}


	    /*This prints the entire content of the fringe */
		/*	
		for (unsigned i=0; i<fringe.size(); i++){
			cout << "#######################################" <<  endl;
    		Puzzle printIt = fringe.at(i);
			for (int rows = 0; rows < NUM_ROWS; rows++) {
    	   		for (int cols = 0; cols < NUM_COLS; cols++) {
    	        	std::cout << "|" << printIt[rows][cols];
    	    	}
    	    	std::cout << "|" << std::endl;
 			}
			cout << manhattan_distance(printIt) <<  endl;	
			cout << misplaced_tiles(printIt) << endl;	
		}  
		*/
		int minIndex;
		int smallestFitness = 9999;
		if (heuristic == Heuristic::MISPLACED_TILES) {
			for (unsigned i=0; i<fringe.size(); i++){
				int tmpFitness = misplaced_tiles( fringe.at(i));
    	    	//	cout << tmpFitness << endl;
				if (tmpFitness < smallestFitness){ 
					minIndex = i;
					smallestFitness = tmpFitness;
				}
			}
          //cout << smallestFitness << " ;  " << minIndex  << " ;  "<< fringe.size()  << endl;
		} 
		else {
			for (unsigned i=0; i<fringe.size(); i++){
				int tmpFitness = manhattan_distance( fringe.at(i));
                  //  cout << tmpFitness << endl;
				if (tmpFitness < smallestFitness){
					minIndex = i;
					smallestFitness = tmpFitness;
				}
			}
          //cout << smallestFitness << " ;  " << minIndex  << " ;  "<< fringe.size()  << endl;

		}
		//cout << " before erasing "<< fringe.size()  << endl;
		puzzle = fringe.at(minIndex);
		visited.push_back(fringe.at(minIndex));
		fringe.erase(fringe.begin()+minIndex);
		//cout << " after erasing "<< fringe.size()  << endl;
		cout << smallestFitness << endl;
		//cout << zeroPos << endl;
		fitnessValue =  smallestFitness ;
		/*cout << "newPuzzle: " <<  endl;
			for (int rows = 0; rows < NUM_ROWS; rows++) {
    	   		for (int cols = 0; cols < NUM_COLS; cols++) {
    	        	std::cout << "|" << puzzle[rows][cols];
    	    	}
    	    	std::cout << "|" << std::endl;
 			}
 						cout << "#######################################" <<  endl;

		*/
 								cout << iterations++ << endl;


	//	solved = true;
	} 
}


void Agent::a_star(Heuristic heuristic) {
	//TODO
	//Note: In this assignment you must find the proper way to
	//keep track of repeated states.
	std::list<Puzzle> fringe;
	int fitnessValue = 9999;
	if (heuristic == Heuristic::MISPLACED_TILES) {
		int fitnessValue = misplaced_tiles(puzzle);
	}
	else {
		int fitnessValue = manhattan_distance(puzzle);
	} 
	cout << fitnessValue << endl;
}


void Agent::print_puzzle(Puzzle& puzzle) {
	for (int rows = 0; rows < NUM_ROWS; rows++) {
		for (int cols = 0; cols < NUM_COLS; cols++) {
			std::cout << "|" << puzzle[rows][cols];
		}
		std::cout << "|" << std::endl;
	}
}

