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
#include <queue>

#define NUM_COLS 3
#define NUM_ROWS 3

using namespace std;

typedef vector<vector<int> > Puzzle;
typedef std::pair<int, int> Cost;
typedef std::pair<Puzzle, Cost> Node;

enum Heuristic{
MISPLACED_TILES,
MANHATTAN_DISTANCE
};

enum Solver{
GREEDY,
A_STAR
};

/*auto cmp_nodes = [](Node left, Node right) {
    Cost l = left.second;
    Cost r = right.second;
    return l.first + l.second < r.first + r.second;
};
typedef std::priority_queue<Node, std::vector<Node>, decltype(cmp_nodes)> Fringe;*/

class Agent {
public:
	Agent(Puzzle puzzle, Solver solver, Heuristic heuristic);
	~Agent();

	void run();

private:
    Puzzle start;

    int (Agent::*heuristic)( Puzzle );
    void (Agent::*solver)( void );

	void greedy_search();
	void a_star();
    void a_star_expand(std::vector<Node> &fringe, std::vector<Node> &visited, Node &node, int row, int col, int newrow, int newcol);
	int misplaced_tiles(Puzzle puzzle);
	int manhattan_distance(Puzzle puzzle);

	void print_puzzle(Puzzle& puzzle);
	int mkSwitches(Puzzle& puzzle);

    Puzzle switch_nums(Puzzle &puzzle, int row1, int col1, int row2, int col2);
    void add_node(int row, int col, int newrow, int newcol);
    bool in_parents(Puzzle &puzzle);

    Puzzle& state_of(Node &node);
    Cost& cost_of(Node &node);

    bool is_goal(Puzzle &p);
    bool is_equal(Puzzle &pa, Puzzle &pb);

    std::vector<Node> expand(Node &n);

};

#endif
