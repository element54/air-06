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

#include <chrono>
#include <thread>

Agent::Agent(Puzzle puzzle, Solver s, Heuristic h) : puzzle(puzzle), goal(NUM_ROWS, std::vector<int>(NUM_COLS, 0)) {
    if (s == Solver::GREEDY) {
        solver = &Agent::greedy_search;
        cout << "Solver: Greedy Search" << endl;
    } else {
        solver = &Agent::a_star;
        cout << "Solver: A*" << endl;
    }
    if (h == Heuristic::MISPLACED_TILES) {
        heuristic = &Agent::misplaced_tiles;
        cout << "Heuristic: Misplaced tiles" << endl;
    } else {
        heuristic = &Agent::manhattan_distance;
        cout << "Heuristic: Manhattan distance" << endl;
    }
    for (int i = 0;i < (NUM_COLS * NUM_COLS); i++) {
        goal[i / NUM_COLS][i % NUM_COLS] = (i + 1) % (NUM_COLS * NUM_COLS);
    }
}

 Agent::~Agent() {
 }

 void Agent::run() {
    (this->*solver)();
 }

int Agent::misplaced_tiles(Puzzle puzzle) {
    int res = 0;
    for (int i = 0;i < (NUM_ROWS * NUM_COLS); i++) {
        int row = i / NUM_COLS;
        int col = i % NUM_COLS;
        if(puzzle[row][col] != 0)
            res += goal[row][col] != puzzle[row][col] ? 1 : 0;
    }
    return res;
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
        int res = 0;
        for (int i = 0;i < (NUM_ROWS * NUM_COLS); i++) {
            int row = i / NUM_COLS;
            int col = i % NUM_COLS;
            int num = puzzle[row][col];
            if(num != 0) {
                int target_row = (num - 1) / NUM_COLS;
                int target_col = (num - 1) % NUM_COLS;
                res += std::abs(row - target_row) + std::abs(col - target_col);
            }
        }
        return res;
    }

Puzzle Agent::switch_nums(Puzzle puzzle, int row1, int col1, int row2, int col2) {
    Puzzle ret = puzzle;
    ret[row1][col1] = puzzle[row2][col2];
    ret[row2][col2] = puzzle[row1][col1];
    return ret;
}

std::pair<Puzzle, int> Agent::get_cheapest(Puzzle current, int row, int col, int newrow, int newcol, std::pair<Puzzle, int> cheapest) {
    if(newrow < 0 || newrow >= NUM_ROWS)
        return cheapest;
    if(newcol < 0 || newcol >= NUM_COLS)
        return cheapest;
    Puzzle np = switch_nums(puzzle, row, col, newrow, newcol);
    int c = (this->*heuristic)(np);
    if(c < cheapest.second) {
        return std::make_pair(np, c);
    } else {
        return cheapest;
    }
}

void Agent::greedy_search() {
    std::vector<Puzzle> path;
    path.push_back(puzzle);
    while(puzzle != goal) {
        std::pair<Puzzle, int> cheapest = std::make_pair(puzzle, 10000000);
        for (int i = 0;i < (NUM_ROWS * NUM_COLS); i++) {
            int row = i / NUM_COLS;
            int col = i % NUM_COLS;
            if(puzzle[row][col] == 0) {
                cheapest = get_cheapest(puzzle, row, col, row - 1, col, cheapest);
                cheapest = get_cheapest(puzzle, row, col, row, col + 1, cheapest);
                cheapest = get_cheapest(puzzle, row, col, row + 1, col, cheapest);
                cheapest = get_cheapest(puzzle, row, col, row, col - 1, cheapest);
            }
        }

        puzzle = cheapest.first;
        path.push_back(puzzle);
        print_puzzle(puzzle);
        cout << "---" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}


void Agent::a_star() {
    std::vector<Puzzle> path;
    path.push_back(puzzle);
    cout << (this->*heuristic)(puzzle) << endl;
	//TODO
	//Note: In this assignment you must find the proper way to
	//keep track of repeated states.
    /*
	std::list<Puzzle> fringe;
	int fitnessValue = 9999;
	if (heuristic == Heuristic::MISPLACED_TILES) {
		int fitnessValue = misplaced_tiles(puzzle);
	}
	else {
		int fitnessValue = manhattan_distance(puzzle);
	}
	cout << fitnessValue << endl;
    */
}


void Agent::print_puzzle(Puzzle& puzzle) {
	for (int rows = 0; rows < NUM_ROWS; rows++) {
		for (int cols = 0; cols < NUM_COLS; cols++) {
			std::cout << "|" << puzzle[rows][cols];
		}
		std::cout << "|" << std::endl;
	}
}
