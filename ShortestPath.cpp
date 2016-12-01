#include "ShortestPath.hpp"

#include <iostream>
#include <stdint.h>

#define INITIAL_FRONTIER_ARRAY_SIZE 131

using namespace std;

ShortestPath::ShortestPath(Graph &gr, SCC &comp) : visit_version_(1), clevelf_(0), clevelf1_(0), clevelb_(0), clevelb1_(0), pr_graph_(gr), strongly_conn_(comp),
												   distance_front_(0), distance_back_(0), dirf_('F'), dirb_('B'),
												   frontier_front_(INITIAL_FRONTIER_ARRAY_SIZE), frontier_back_(INITIAL_FRONTIER_ARRAY_SIZE) {}

ShortestPath::~ShortestPath() { }

int ShortestPath::shortestPath(uint32_t& source, uint32_t& target, char mode) {

	if (source == target)
		return 0;

	//initilizations of structures
	uint32_t tempId, node_id, comp1, comp2;
	if (mode == 'S') {
		comp1 = strongly_conn_.findNodeStronglyConnectedComponentID(source);
		comp2 = strongly_conn_.findNodeStronglyConnectedComponentID(target);
	}
	short int child_check;

	clevelf_ = 1;
	clevelb_ = 1;

	frontier_front_.enqueue(source);
	frontier_back_.enqueue(target);

	pr_graph_.checkMarkVisitedNode(source, dirf_, visit_version_);
	pr_graph_.checkMarkVisitedNode(target, dirb_, visit_version_);

	uint32_t c1 = pr_graph_.getNeighborsCount(source, dirf_), c2 = pr_graph_.getNeighborsCount(target, dirb_);
	while (true) {

		//no solution, return failure
		if (frontier_front_.isEmpty() || frontier_back_.isEmpty())
			return -1;

		if (clevelf_ + c1 <= clevelb_ + c2) {

			c1 = 0;
			while (clevelf_ > 0) {
				node_id = frontier_front_.popFront();
				--clevelf_;
				// c1++;

				//expand node
				Garray<uint32_t > &neighbors = pr_graph_.getNeighbors(node_id, dirf_);
				for (int i = 0; i < neighbors.getElements(); i++) {
					tempId = neighbors[i];
					if (mode == 'S' && strongly_conn_.findNodeStronglyConnectedComponentID(tempId) != comp1)
						continue;
					if (pr_graph_.checkVisitedNode(tempId, dirb_, visit_version_))  {
						return distance_front_ + distance_back_ + 1;
					} else if (pr_graph_.checkMarkVisitedNode(tempId, dirf_, visit_version_)) {
						//explored_set_.insert(tempId);
						frontier_front_.enqueue(tempId);
						++clevelf1_;
						c1 += pr_graph_.getNeighborsCount(tempId, dirf_);
					}
				}
			}
			++distance_front_;
			clevelf_ = clevelf1_;
			clevelf1_ = 0;
		} else {

			c2 = 0;
			while (clevelb_ > 0) {
				node_id = frontier_back_.popFront();
				--clevelb_;
				// c2++;
				//expand node
				Garray<uint32_t > &neighbors = pr_graph_.getNeighbors(node_id, dirb_);
				for (int i = 0; i < neighbors.getElements(); i++) {
					tempId = neighbors[i];
					if (mode == 'S' && strongly_conn_.findNodeStronglyConnectedComponentID(tempId) != comp2)
						continue;
					if (pr_graph_.checkVisitedNode(tempId, dirf_, visit_version_))  {
						return distance_front_ + distance_back_ + 1;
					} else if (pr_graph_.checkMarkVisitedNode(tempId, dirb_, visit_version_)) {
						//explored_set_x.insert(tempId);
						frontier_back_.enqueue(tempId);
						++clevelb1_;
						c2 += pr_graph_.getNeighborsCount(tempId, dirb_);
					}
				}
			}
			++distance_back_;
			clevelb_ = clevelb1_;
			clevelb1_ = 0;
		}
	}

	return -1;
}

void ShortestPath::reset() {
	//clear structures for next queries
	distance_front_ = 0;
	distance_back_ = 0;
	clevelb_ = 0;
	clevelb1_ = 0;
	clevelf_ = 0;
	clevelf1_ = 0;
	frontier_front_.clear();
	frontier_back_.clear();
	visit_version_++;
}
