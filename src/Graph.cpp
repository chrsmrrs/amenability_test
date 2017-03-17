/**********************************************************************
 * Copyright (C) 2017 Christopher Morris <christopher.morris@udo.edu>
 *
 * This file is part of Amendable.
 *
 * Amendable can not be copied or distributed without the express
 * permission of Christopher Morris.
 *********************************************************************/

#include <algorithm>
#include "Graph.h"

namespace GraphLibrary {
    Graph::Graph() : m_num_nodes(0), m_num_edges(0), m_node_labels() {}

    Graph::Graph(const uint num_nodes, const EdgeList &edgeList, const Labels node_labels)
            : m_adjacency_lists(), m_num_nodes(num_nodes), m_num_edges(edgeList.size()), m_node_labels(node_labels) {
        m_adjacency_lists.resize(num_nodes);

        for (auto const &e: edgeList) {
            add_edge(e.first, e.second);
        }
    }

    size_t Graph::add_node() {
        vector<Node> new_node;
        m_adjacency_lists.push_back(move(new_node));
        m_num_nodes++;

        return m_num_nodes - 1;
    }

    void Graph::add_edge(const Node v, const Node w) {
        m_adjacency_lists[v].push_back(w);
        m_adjacency_lists[w].push_back(v);

        m_num_edges++;
    }

    size_t Graph::get_degree(const Node v) const {
        return m_adjacency_lists[v].size();
    }

    Nodes Graph::get_neighbours(const Node v) const {
        return m_adjacency_lists[v];
    }

    size_t Graph::get_num_nodes() const {
        return m_num_nodes;
    }

    size_t Graph::get_num_edges() const {
        return m_num_edges;
    }

    Labels Graph::get_labels() const {
        return m_node_labels;
    }

    bool Graph::has_edge(const Node v, const Node w) const {
        if (find(m_adjacency_lists[v].begin(), m_adjacency_lists[v].end(), w) != m_adjacency_lists[v].end()) {
            return true;
        } else {
            return false;
        }
    }

    Graph::~Graph() {}
}