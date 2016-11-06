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
        start(puzzle) {
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


bool Agent::is_equal(Puzzle &pa, Puzzle &pb) {
    for (int i = 0;i < (NUM_ROWS * NUM_COLS); i++) {
        int row = i / NUM_COLS;
        int col = i % NUM_COLS;
        if(pa[row][col] != pb[row][col] )
            return false;
    }
    return true;
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


std::vector<Node> Agent::expand(Node &node) {
    std::vector<Node> childs;
    Puzzle &puzzle = state_of(node);
    for (int i = 0;i < (NUM_ROWS * NUM_COLS); i++) {
        int row = i / NUM_COLS;
        int col = i % NUM_COLS;
        if(puzzle[row][col] != 0)
            continue;
        if(row - 1 >= 0) {
            Puzzle np = switch_nums(state_of(node), row, col, row - 1, col);
            Cost nc = std::make_pair((this->*heuristic)(np), 0);
            childs.push_back(std::make_pair(np, nc));
        }
        if(row + 1 < NUM_ROWS) {
            Puzzle np = switch_nums(state_of(node), row, col, row + 1, col);
            Cost nc = std::make_pair((this->*heuristic)(np), 0);
            childs.push_back(std::make_pair(np, nc));
        }
        if(col - 1 >= 0) {
            Puzzle np = switch_nums(state_of(node), row, col, row, col - 1);
            Cost nc = std::make_pair((this->*heuristic)(np), 0);
            childs.push_back(std::make_pair(np, nc));
        }
        if(col + 1 < NUM_COLS) {
            Puzzle np = switch_nums(state_of(node), row, col, row, col + 1);
            Cost nc = std::make_pair((this->*heuristic)(np), 0);
            childs.push_back(std::make_pair(np, nc));
        }
        break;
    }
    return childs;
}

void Agent::greedy_search() {
    int steps = 0;
    std::vector<Node> visited;
    std::vector<Node> fringe;
    fringe.push_back(std::make_pair(start, std::make_pair((this->*heuristic)(start), 0)));
    while(true) {
        steps++;
        if(fringe.empty()) {
            cout << "Failed!" << endl;
            return;
        }

        int min_index = 0;
        int min_cost = cost_of(fringe[0]).first + cost_of(fringe[0]).second;
        for(int i = 1;i<fringe.size();i++) {
            Node n = fringe[i];
            Cost c = cost_of(n);
            if(c.first + c.second < min_cost) {
                min_cost = c.first + c.second;
                min_index = i;
            }
        }
        Node node = fringe[min_index];
        Puzzle puzzle = state_of(node);
        Cost cost = cost_of(node);

        print_puzzle(puzzle);
        if(is_goal(puzzle)) {
            cout << "Success!" << endl;
            return;
        }
        visited.push_back(node);
        fringe.erase(fringe.begin() + min_index);
        cout << endl;
        std::vector<Node> childs = expand(node);
        for(Node &child : childs) {
            //print_puzzle(state_of(child));
            Cost child_cost = cost_of(child);
            Puzzle child_puzzle = state_of(child);
            bool in_visited = false;
            for (auto &n : visited) {
                Puzzle &p = state_of(n);
                if(is_equal(p, child_puzzle)) {
                    in_visited = true;
                    break;
                }
            }
            if(in_visited) {
                continue;
            }
            bool in_fringe = false;
            for (auto &n : fringe) {
                Puzzle &p = state_of(n);
                if(!is_equal(p, child_puzzle))
                    continue;
                in_fringe = true;
                Cost &c = cost_of(n);
                if(c.first + c.second < child_cost.first + child_cost.second) {
                    fringe.push_back(child);
                    break;
                }
            }
            if(!in_fringe) {
                fringe.push_back(child);

                visited.push_back(child);
            }
        }
    }

}

void Agent::a_star_expand(std::vector<Node> &fringe, std::vector<Node> &visited, Node &node, int row, int col, int newrow, int newcol) {
    if(newrow < 0 || newrow >= NUM_ROWS)
        return;
    if(newcol < 0 || newcol >= NUM_COLS)
        return;

    Puzzle np = switch_nums(state_of(node), row, col, newrow, newcol);
    Cost nc = std::make_pair((this->*heuristic)(np), cost_of(node).second + 1);
    for (auto &n : visited) {
        Puzzle &p = state_of(n);
        if(!is_equal(p, np))
            continue;
        Cost &c = cost_of(n);
        if(c.first + c.second < nc.first + nc.second)
            return;
    }
    for (auto &n : fringe) {
        Puzzle &p = state_of(n);
        if(!is_equal(p, np))
            continue;
        Cost &c = cost_of(n);
        if(c.first + c.second < nc.first + nc.second)
            return;
    }
    fringe.push_back(std::make_pair(np, nc));
}

void Agent::a_star() {
    int steps = 0;
    std::vector<Node> visited;
    std::vector<Node> fringe;
    fringe.push_back(std::make_pair(start, std::make_pair((this->*heuristic)(start), 0)));
    while(true) {
        steps++;
        if(fringe.empty()) {
            cout << "Failed!" << endl;
            return;
        }

        auto cheapest_index = fringe.begin();
        int cheapest_cost = 9999999;
        Node cheapest;
        for (auto i = fringe.begin(); i != fringe.end(); ++i) {
            auto n = *i;
            Cost c = cost_of(n);
            int ct = c.first + c.second;
            if(ct < cheapest_cost) {
                cheapest = *i;
                cheapest_index = i;
                cheapest_cost = ct;
            }
        }

        Node node = cheapest;
        Puzzle puzzle = state_of(node);
        fringe.erase(cheapest_index);

        if(steps % 100 == 0) {
            cout << "Steps: " << steps << endl;
            cout << "H: " << cost_of(node).first << endl;
            cout << "G: " << cost_of(node).second << endl;
            cout << "Fringe: " << fringe.size() << endl;
            cout << "Visited: " << visited.size() << endl << endl;
        }


        if(is_goal(puzzle)) {
            cout << "Success!" << endl;
            return;
        }
        visited.push_back(node);
        for (int i = 0;i < (NUM_ROWS * NUM_COLS); i++) {
            int row = i / NUM_COLS;
            int col = i % NUM_COLS;
            if(puzzle[row][col] != 0)
                continue;

            a_star_expand(fringe, visited, node, row, col, row - 1, col);
            a_star_expand(fringe, visited, node, row, col, row + 1, col);
            a_star_expand(fringe, visited, node, row, col, row, col - 1);
            a_star_expand(fringe, visited, node, row, col, row, col + 1);
            break;
        }
        // Expand

    }
}


void Agent::print_puzzle(Puzzle& puzzle) {
	for (int rows = 0; rows < NUM_ROWS; rows++) {
		for (int cols = 0; cols < NUM_COLS; cols++) {
			std::cout << "|" << puzzle[rows][cols];
		}
		std::cout << "|" << std::endl;
	}
}
