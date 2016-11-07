/*
 * Daniel Vazquez
 * Aritificial Intelligence for Robotics
 * SS 2016
 * Assignment 6
 *
 * modified by Max Mensing, Torsten Jandt
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

class Agent {
public:
	Agent(Puzzle puzzle, Solver solver, Heuristic heuristic);
	~Agent();

	void run();

private:
    Puzzle start;

    int (Agent::*heuristic)( Puzzle );
    int (Agent::*cost_sum_impl) (Cost &cost);

    void expand(std::vector<Node> &fringe, std::vector<Node> &visited, Node &node);
    std::vector<Node> get_children(Node &n);
	int misplaced_tiles(Puzzle puzzle);
	int manhattan_distance(Puzzle puzzle);

	void print_puzzle(Puzzle& puzzle);

    Puzzle switch_nums(Puzzle &puzzle, int row1, int col1, int row2, int col2);

    Puzzle& state_of(Node &node);
    Cost& cost_of(Node &node);

    int cost_sum(Cost &cost);
    int cost_sum(Node &node);
    int cost_sum_greedy(Cost &cost);
    int cost_sum_a_star(Cost &cost);

    Node create_node(Puzzle puzzle, Cost cost);
    Cost create_cost(Puzzle puzzle, Node parent);

    bool is_goal(Puzzle &p);
    bool is_equal(Puzzle &pa, Puzzle &pb);
    bool is_equal(Node &na, Node &nb);

    bool is_in_and_cheaper(std::vector<Node> &list, Node &node);



};

#endif
