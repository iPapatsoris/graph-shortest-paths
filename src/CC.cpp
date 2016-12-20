#include "CC.hpp"

#include <assert.h>

#include <iostream>

using namespace std;

uint32_t count = 0;


CC::~CC() {}

void CC::estimateConnectedComponents() {
    this->reset();
    uint32_t total_nodes = graph_.getNodes('N');
    assert(total_nodes);

    bool first = true;
    uint32_t cc_id = 0;
    Garray<uint32_t> roots;
    for (uint32_t i = 0; i < total_nodes; i++) {
        if (graph_.getNeighborsCount(i, 'B') == 0 || graph_.getNeighborsCount(i, 'F') == 0) {
            roots.enstack(i);
        }
    }
    total_nodes = roots.getElements();
    for (uint32_t start_node = 0 ; start_node < total_nodes ; start_node++) {
        uint32_t root = roots[start_node];
        // uint32_t root = start_node;
        // cout << root << endl;
        if (! graph_.checkMarkCCFlag(root, cc_flag_, 'N')) {
            continue;
        }
        if (!first) {
            cc_id++;
        }
        ccindex_[root] = cc_id;
        frontier_.enstack(root);
        while (! frontier_.isEmpty()) {
            uint32_t node = frontier_.popBack();
            Garray<uint32_t > &neighbors = graph_.getNeighbors(node, 'A', 0);
            for (int i = 0; i < neighbors.getElements(); i++) {
                node = neighbors[i];
                if (graph_.checkMarkCCFlag(node, cc_flag_, 'N')) {
                    frontier_.enstack(node);
                    ccindex_[node] = cc_id;
                }
            }
        }
        if (first) {
            first = false;
        }
    }
    //if (visit_version_ == 1) {   // Omitting new/deletes with versioning barely helps
        update_index_.init(cc_id+1);
        update_index_.setElements(cc_id+1);
    //  }
    size_index_.init(cc_id+2);  // Size index compatibility with visit_version not implemented (not needed yet)
    size_index_.setElements(cc_id+2);
    size_index_[0] = 0; // indexing from 1
    size_index_[1] = cc_id+1;
    for (uint32_t i = 2 ; i < size_index_.getElements() ; i++) {
        size_index_[i] = 0;
    }
}

void CC::insertNewEdge(const uint32_t &source_node, const uint32_t &target_node) {
    assert(source_node < ccindex_.getElements() && target_node < ccindex_.getElements()); // Temp assert. If failure, implement new node addition
    uint32_t comp1 = findNodeConnectedComponentID(source_node);
    uint32_t comp2 = findNodeConnectedComponentID(target_node);
    if (comp1 == comp2) {
        return;
    }
    uint32_t size1 = update_index_[comp1].getElements();
    uint32_t size2 = update_index_[comp2].getElements();

    /* Make sure they components aren't already connected.
     * If they are, they will definitely have the same number of component neighbors,
     * so skip search if the don't. If they do, consult size_index */

   // cout << size1 << " Vs " << size2 << "\n";
    if (size1 == size2 && (size_index_[size1+1] == 1 || update_index_[comp1].search(comp2))) {
        return;
    }

    size_index_[size1+1]--;
    size_index_[size2+1]--;
    size_index_[size1+size2+2]++;

    /* Connect necessary components with each other */
    update_index_[comp1].pushBatch(update_index_[comp2].getArray(), size2);
    update_index_[comp1].enstack(comp2);
    for (uint32_t n = 0 ; n < size1 ; n++) {
        update_index_[update_index_[comp1][n]].pushBatch(update_index_[comp2].getArray(), size2);
        update_index_[update_index_[comp1][n]].enstack(comp2);
    }

    update_index_[comp2].pushBatch(update_index_[comp1].getArray(), size1);
    update_index_[comp2].enstack(comp1);
    for (uint32_t n = 0 ; n < size2 ; n++) {
        update_index_[update_index_[comp2][n]].pushBatch(update_index_[comp1].getArray(), size1);
        update_index_[update_index_[comp2][n]].enstack(comp1);
    }
}

bool CC::sameConnectedComponent(const uint32_t &source_node, const uint32_t &target_node) {
    count++;
    queries_count_++;
    uint32_t comp1 = findNodeConnectedComponentID(source_node);
    uint32_t comp2 = findNodeConnectedComponentID(target_node);
    if (comp1 == comp2) {
        return true;
    }
    update_index_use_count_++;
    uint32_t size1 = update_index_[comp1].getElements();
    uint32_t size2 = update_index_[comp2].getElements();

    //cout << size1 << " Vs " << size2 << "\n";
    if (size1 == size2 && (size_index_[size1+1] == 1 || update_index_[comp1].search(comp2))) {
        return true;
    }
    return false;
}

void CC::reset() {
    uint32_t total_nodes = graph_.getNodes('N');
    ccindex_.init(total_nodes);
    ccindex_.setElements(total_nodes);
    queries_count_ = 0;
    update_index_use_count_ = 0;
    visit_version_++;
    if (cc_flag_) {
        cc_flag_ = false;
    } else {
        cc_flag_ = true;
    }
}

void CC::print() {
    cout << "\n*** Sizes\n";
    for (uint32_t i = 1 ; i < size_index_.getElements() ; i++) {
        if (size_index_[i]) {
            cout << "size_index_[" << i << "] = " << size_index_[i] << "\n";
        }
    }
    cout << "Nodes are " << graph_.getNodes('N') << endl;
}