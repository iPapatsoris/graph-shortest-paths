#ifndef GRAPHSHORTESTPATHS_LISTNODE_H
#define GRAPHSHORTESTPATHS_LISTNODE_H

#include <stdint.h>

// Test value: 3

#define MAX_NEIGHBORS 5
class ListNode {
friend class ShortestPath;

public:
    ListNode() : cur_neighbors_(0), next_pos_(-1) {}
    ~ListNode() {}
    void insertNeighbor(const uint32_t &neighbor_id, const char &mode, const uint32_t &edge_version);
    bool isFull() const {return cur_neighbors_ == MAX_NEIGHBORS;}
    void setNextPos(const long &next_pos) {next_pos_ = next_pos;}
    long getNextPos() {return next_pos_;}
    bool search(const uint32_t &target_id);
    uint32_t *getNeighborArray() {return neighbor_;}
    uint32_t *getEdgeVersion() {return edge_version_;}
    uint32_t getNeighborNumber() {return cur_neighbors_;}
    void print() const;
    uint32_t getMaxSize() { return MAX_NEIGHBORS; }

private:
    uint32_t neighbor_[MAX_NEIGHBORS];
    uint32_t cur_neighbors_;
    long next_pos_;
    uint32_t edge_version_[MAX_NEIGHBORS];
};

#endif //GRAPHSHORTESTPATHS_LISTNODE_H
