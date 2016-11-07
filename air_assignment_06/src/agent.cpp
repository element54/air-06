/*
 * Daniel Vazquez
 * Aritificial Intelligence for Robotics
 * SS 2016
 * Assignment 6
 *
 * modified by Max Mensing, Torsten Jandt
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
        start(puzzle) {
    if (s == Solver::GREEDY) {
        cost_sum_impl = &Agent::cost_sum_greedy;
        cout << "Solver: Greedy Search" << endl;
    } else {
        cost_sum_impl = &Agent::cost_sum_a_star;
        cout << "Solver: A*" << endl;
    }
    if (h == Heuristic::MISPLACED_TILES) {
        heuristic = &Agent::misplaced_tiles;
        cout << "Heuristic: Misplaced tiles" << endl;
    } else {
        heuristic = &Agent::manhattan_distance;
        cout << "Heuristic: Manhattan distance" << endl;
    }
}

 Agent::~Agent() {
 }

 void Agent::run() {
    //(this->*solver)();
    int steps = 0;
    std::vector<Node> visited;
    std::vector<Node> fringe;
    fringe.push_back(create_node(start, std::make_pair((this->*heuristic)(start), 0)));
    while(true) {
        steps++;
        if(fringe.empty()) {
            cout << "Failed!" << endl;
            return;
        }

        int min_index = 0;
        int min_cost = cost_sum(fringe[0]);
        for(int i = 1;i<fringe.size();i++) {
            Node n = fringe[i];
            Cost c = cost_of(n);
            if(cost_sum(c) < min_cost) {
                min_cost = cost_sum(c);
                min_index = i;
            }
        }
        Node node = fringe[min_index];
        Puzzle puzzle = state_of(node);
        Cost cost = cost_of(node);

        if(is_goal(puzzle)) {
            cout << "Success!" << endl;
            print_puzzle(puzzle);
            cout << "Iterations: " << steps << endl;
            cout << "Finge: " << fringe.size() << endl;
            cout << "Visited: " << visited.size() << endl;
            cout << "Solution path length: " << cost_of(node).second << endl << endl;
            return;
        } else {
            visited.push_back(node);
            fringe.erase(fringe.begin() + min_index);
            expand(fringe, visited, node);
        }
    }
 }


std::vector<Node> Agent::get_children(Node &node) {
    std::vector<Node> childs;
    Puzzle &puzzle = state_of(node);
    for (int i = 0;i < (NUM_ROWS * NUM_COLS); i++) {
        int row = i / NUM_COLS;
        int col = i % NUM_COLS;
        if(puzzle[row][col] != 0)
            continue;
        for(int n = 0;n<4;n++) {
            int new_row = row;
            int new_col = col;
            switch (n) {
                case 0:
                    new_row = row - 1; break;
                case 1:
                    new_row = row + 1; break;
                case 2:
                    new_col = col - 1; break;
                case 3:
                    new_col = col + 1; break;
            }
            if(new_row >= 0 && new_row < NUM_ROWS && new_col >= 0 && new_col < NUM_COLS) {
                Puzzle np = switch_nums(state_of(node), row, col, new_row, new_col);
                Cost nc = create_cost(np, node);
                childs.push_back(create_node(np, nc));
            }
        }
    }
    return childs;
}

bool Agent::is_in_and_cheaper(std::vector<Node> &list, Node &node) {
    for (auto &n : list) {
        if(!is_equal(n, node))
            continue;
        if(cost_sum(n) <= cost_sum(node))
            return true;
    }
    return false;
}

void Agent::expand(std::vector<Node> &fringe, std::vector<Node> &visited, Node &parent) {
    std::vector<Node> childs = get_children(parent);
    for(Node &child : childs) {
        bool in_visited = is_in_and_cheaper(visited, child);
        bool in_fringe = is_in_and_cheaper(fringe, child);
        if(!in_visited && !in_fringe)
            fringe.push_back(child);
    }
}

Puzzle Agent::switch_nums(Puzzle &puzzle, int row1, int col1, int row2, int col2) {
   Puzzle ret = puzzle;
   ret[row1][col1] = puzzle[row2][col2];
   ret[row2][col2] = puzzle[row1][col1];
   return ret;
}

Puzzle& Agent::state_of(Node &node) {
   return node.first;
}
Cost& Agent::cost_of(Node &node) {
   return node.second;
}

int Agent::cost_sum(Node &node) {
   return cost_sum(cost_of(node));
}

int Agent::cost_sum(Cost &cost) {
    return (this->*cost_sum_impl)(cost);
}

Cost Agent::create_cost(Puzzle puzzle, Node parent) {
    return std::make_pair((this->*heuristic)(puzzle), cost_of(parent).second + 1);
}

int Agent::cost_sum_greedy(Cost &cost) {
    return cost.first;
}

int Agent::cost_sum_a_star(Cost &cost) {
    return cost.first + cost.second;
}

Node Agent::create_node(Puzzle puzzle, Cost cost) {
    return std::make_pair(puzzle, cost);
}

bool Agent::is_goal(Puzzle &p) {
    for (int i = 0;i < (NUM_ROWS * NUM_COLS); i++) {
        int row = i / NUM_COLS;
        int col = i % NUM_COLS;
        if(p[row][col] != (i + 1) % (NUM_ROWS * NUM_COLS))
            return false;
    }
    return true;
}


bool Agent::is_equal(Node &na, Node &nb) {
    return is_equal(state_of(na), state_of(nb));
}

bool Agent::is_equal(Puzzle &pa, Puzzle &pb) {
    for (int i = 0;i < (NUM_ROWS * NUM_COLS); i++) {
        int row = i / NUM_COLS;
        int col = i % NUM_COLS;
        if(pa[row][col] != pb[row][col] )
            return false;
    }
    return true;
}

int Agent::misplaced_tiles(Puzzle puzzle) {
   int res = 0;
   for (int i = 0;i < (NUM_ROWS * NUM_COLS); i++) {
       int row = i / NUM_COLS;
       int col = i % NUM_COLS;
       if(puzzle[row][col] != 0)
           res += (((i + 1) % (NUM_ROWS * NUM_COLS)) != puzzle[row][col] ? 1 : 0);
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

void Agent::print_puzzle(Puzzle& puzzle) {
	for (int rows = 0; rows < NUM_ROWS; rows++) {
		for (int cols = 0; cols < NUM_COLS; cols++) {
			std::cout << "|" << puzzle[rows][cols];
		}
		std::cout << "|" << std::endl;
	}
}
