/**********************************************************************
 * Copyright (C) 2017 Christopher Morris <christopher.morris@udo.edu>
 *
 * This file is part of Amenable. An implementation of the algorithm described in:
 *
 * @InProceedings{Arvind+2015,
 *   author = "Arvind, V. and K{\"o}bler, Johannes and Rattan, Gaurav and Verbitsky, Oleg",
 *   title = "On the Power of Color Refinement",
 *   booktitle = "20th International Symposium on Fundamentals of Computation Theory",
 *   year = "2015",
 *   pages = "339--350"
 * }
 *
 * Amenable can not be copied or distributed without the express
 * permission of Christopher Morris.
 *********************************************************************/

#ifndef AMEN_GRAPH_H
#define AMEN_GRAPH_H

#include <unordered_set>
#include <vector>

using namespace std;

using uint = unsigned int;
using Node = uint;
using Nodes = vector<Node>;
using Label = ulong;
using Labels = vector<Label>;
using Edge = pair<Node, Node>;
using EdgeList = vector<Edge>;

namespace std {
    namespace {
        // Code from boost: Reciprocal of the golden ratio helps spread entropy and handles duplicates.
        // See Mike Seymour in magic-numbers-in-boosthash-combine: http://stackoverflow.com/questions/4948780 .
        template<class T>
        inline void hash_combine(std::size_t &seed, T const &v) {
            seed ^= hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        // Recursive template code derived from Matthieu M.
        template<class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
        struct HashValueImpl {
            static void apply(size_t &seed, Tuple const &tuple) {
                HashValueImpl<Tuple, Index - 1>::apply(seed, tuple);
                hash_combine(seed, get<Index>(tuple));
            }
        };

        template<class Tuple>
        struct HashValueImpl<Tuple, 0> {
            static void apply(size_t &seed, Tuple const &tuple) {
                hash_combine(seed, get<0>(tuple));
            }
        };
    }

    // Hash tuples.
    template<typename ... TT>
    struct hash<std::tuple<TT...>> {
        size_t
        operator()(std::tuple<TT...> const &tt) const {
            size_t seed = 0;
            HashValueImpl<std::tuple<TT...> >::apply(seed, tt);
            return seed;
        }
    };

    // Hash pairs.
    template<typename S, typename T>
    struct hash<pair<S, T>> {
        inline size_t operator()(const pair<S, T> &v) const {
            size_t seed = 0;
            hash_combine(seed, v.first);
            hash_combine(seed, v.second);
            return seed;
        }
    };

}

namespace GraphLibrary {
    class Graph {
    public:
        Graph();

        Graph(const uint num_nodes, const EdgeList &edgeList, const Labels node_labels);

        // Add a single node to the graph.
        size_t add_node();

        // Add a single edge to the graph.
        void add_edge(const Node v, const Node w);

        // Get degree of node "v".
        size_t get_degree(const Node v) const;

        // Get neighbors of node "v".
        Nodes get_neighbours(const Node v) const;

        // Get number of nodes in graph.
        size_t get_num_nodes() const;

        // Get number of edges in graph.
        size_t get_num_edges() const;

        // Get node labels of graphs.
        Labels get_labels() const;

        // Returns "true" if edge {u,w} exists, otherwise "false".
        bool has_edge(const Node v, const Node w) const;

        ~Graph();

    private:
        vector<vector<Node>> m_adjacency_lists;

        // Manage number of nodes in graph.
        size_t m_num_nodes;
        // Manage number of edges in graph.
        size_t m_num_edges;
        // Manage node labels.
        Labels m_node_labels;
    };

    typedef vector<Graph> GraphDatabase;
}

#endif //AMEN_GRAPH_H
