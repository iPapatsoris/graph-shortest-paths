#ifndef GRAPHSHORTESTPATHS_OPERATIONSCONTROL_H
#define GRAPHSHORTESTPATHS_OPERATIONSCONTROL_H

#include "Graph.hpp"
#include "ShortestPath.hpp"
#include "SCC.hpp"
#include "CC.hpp"
#include "GrailIndex.hpp"
#include "JobScheduler.hpp"

class OperationsControl {

public:
    OperationsControl(const uint32_t pool_size);
    ~OperationsControl();
    void run();

private:
    void buildGraph();
    void runQueries();
    void parseNodeIds(uint32_t *source, uint32_t *target);

    Graph graph_;
    Garray<ShortestPath *> paths_;
    SCC strongly_conn_;
    CC cc_;
    GrailIndex grail_index_;
    Garray<int> res_array_;
    JobScheduler scheduler_;
    char mode_;
};

#endif //GRAPHSHORTESTPATHS_OPERATIONSCONTROL_H
