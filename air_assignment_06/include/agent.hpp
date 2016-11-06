/*
 * Daniel Vazquez
 * Aritificial Intelligence for Robotics
 * SS 2016
 * Assignment 6
 *
 * agent.hpp
 * */

#ifndef AGENT_HPP
#define AGENT_HPP

#include <vector>
#include <string>
#include <utility>
#include <map>

#define NUM_COLS 3
#define NUM_ROWS 3

using namespace std;

typedef vector<vector<int> > Puzzle;

enum Heuristic{
MISPLACED_TILES,
MANHATTAN_DISTANCE
};

enum Solver{
GREEDY,
A_STAR
};

class Agent {
public:
	Agent(Puzzle puzzle, Solver solver, Heuristic heuristic);
	~Agent();

	void run();

private:

	Puzzle puzzle;
    Puzzle start;
    Puzzle goal;


    std::vector<Puzzle> fringe;
    std::map<Puzzle, Puzzle> parents;
    std::map<Puzzle, std::pair<int, int>> costs;
    std::vector<Puzzle> visited;

	//Solver solver;
	//Heuristic heuristic;

    int (Agent::*heuristic)( Puzzle );
    void (Agent::*solver)( void );

	void greedy_search();
	void a_star();
	int misplaced_tiles(Puzzle puzzle);
	int manhattan_distance(Puzzle puzzle);

	void print_puzzle(Puzzle& puzzle);
	int mkSwitches(Puzzle& puzzle);

    Puzzle switch_nums(int row1, int col1, int row2, int col2);
    void add_node(int row, int col, int newrow, int newcol);
    bool in_parents(Puzzle &puzzle);

};

#endif
