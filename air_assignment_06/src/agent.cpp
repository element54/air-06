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

Agent::Agent(Puzzle puzzle, Solver s, Heuristic h) :
        puzzle(puzzle), start(puzzle), goal(NUM_ROWS, std::vector<int>(NUM_COLS, 0)), fringe(), parents(), costs() {
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
    costs[start] = std::make_pair((this->*heuristic)(puzzle), 0);;
    fringe.push_back(puzzle);
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

Puzzle Agent::switch_nums(int row1, int col1, int row2, int col2) {
    Puzzle ret = puzzle;
    ret[row1][col1] = puzzle[row2][col2];
    ret[row2][col2] = puzzle[row1][col1];
    return ret;
}

void Agent::add_node(int row, int col, int newrow, int newcol) {
    if(newrow < 0 || newrow >= NUM_ROWS)
        return;
    if(newcol < 0 || newcol >= NUM_COLS)
        return;
    Puzzle np = switch_nums(row, col, newrow, newcol);
    //if(!in_parents(np)) {
    if(std::find(visited.begin(), visited.end(), np) == visited.end()) {
        int h = (this->*heuristic)(np);
        int g = costs[puzzle].second + 1;
        int cost = h + g;
        if(std::find(fringe.begin(), fringe.end(), np) != fringe.end()) {
            auto c = costs[np];
            int nc = c.first + c.second;
            if(cost < nc)
                //parents[np] = puzzle;
                costs[np] = std::make_pair(h, g);
        } else {
            fringe.push_back(np);
            //parents[np] = puzzle;
            costs[np] = std::make_pair(h, g);
        }
    }
}

bool Agent::in_parents(Puzzle &search_for) {
    Puzzle current = puzzle;
    while(true) {
        if(current == search_for)
            return true;
        if(current == start)
            break;
        current = parents[current];
    }
    return false;
}



void Agent::a_star() {
    int steps = 0;
    while(true) {
        steps++;
        auto cheapest_index = fringe.begin();
        int cheapest_cost = 9999999;
        std::pair<int, int> t;
        Puzzle cheapest;
        for (auto i = fringe.begin(); i != fringe.end(); ++i) {
            auto c = costs[*i];
            int cost = c.first + c.second;
            if(cost < cheapest_cost) {
                cheapest = *i;
                cheapest_index = i;
                cheapest_cost = cost;
                t = c;
            }
        }

        if(steps % 100 == 0) {
            cout << "Steps: " << steps << endl;
            cout << "H: " << t.first << endl;
            cout << "Depth: " << t.second << endl;
            cout << "Fringe: " << fringe.size() << endl;
        }
        puzzle = cheapest;
        visited.push_back(puzzle);
        fringe.erase(cheapest_index);
        if(puzzle == goal)
            break;
        //print_puzzle(puzzle);
        //cout << "---" << endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        for (int i = 0;i < (NUM_ROWS * NUM_COLS); i++) {
            int row = i / NUM_COLS;
            int col = i % NUM_COLS;
            if(puzzle[row][col] == 0) {
                add_node(row, col, row - 1, col);
                add_node(row, col, row + 1, col);
                add_node(row, col, row, col - 1);
                add_node(row, col, row, col + 1);
            }
        }
    }
    cout << "Steps: " << steps << endl;
    cout << "Depth: " << costs[puzzle].second << endl;
}


void Agent::greedy_search() {
    int steps = 0;
    while(true) {
        steps++;
        cout << "Steps: " << steps << endl;
        auto cheapest_index = fringe.begin();
        int cheapest_cost = 9999999;
        Puzzle cheapest;
        for (auto i = fringe.begin(); i != fringe.end(); ++i) {
            int cost = (this->*heuristic)(*i);
            if(cost < cheapest_cost) {
                cheapest = *i;
                cheapest_index = i;
                cheapest_cost = cost;
            }
        }
        puzzle = cheapest;
        fringe.erase(cheapest_index);
        if(puzzle == goal)
            break;
        //print_puzzle(puzzle);
        //cout << "---" << endl;

        for (int i = 0;i < (NUM_ROWS * NUM_COLS); i++) {
            int row = i / NUM_COLS;
            int col = i % NUM_COLS;
            if(puzzle[row][col] == 0) {
                add_node(row, col, row - 1, col);
                add_node(row, col, row + 1, col);
                add_node(row, col, row, col - 1);
                add_node(row, col, row, col + 1);
            }
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    cout << "Steps: " << steps << endl;
}


void Agent::print_puzzle(Puzzle& puzzle) {
	for (int rows = 0; rows < NUM_ROWS; rows++) {
		for (int cols = 0; cols < NUM_COLS; cols++) {
			std::cout << "|" << puzzle[rows][cols];
		}
		std::cout << "|" << std::endl;
	}
}
