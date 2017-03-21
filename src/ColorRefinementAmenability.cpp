/**********************************************************************
 * Copyright (C) 2017 Christopher Morris <christopher.morris@udo.edu>
 *
 * This file is part of amenability_test. An implementation of the algorithm described in:
 *
 * @InProceedings{Arvind+2015,
 *   author = "Arvind, V. and K{\"o}bler, Johannes and Rattan, Gaurav and Verbitsky, Oleg",
 *   title = "On the Power of Color Refinement",
 *   booktitle = "20th International Symposium on Fundamentals of Computation Theory",
 *   year = "2015",
 *   pages = "339--350"
 * }
 *
 * Amenability_test can not be copied or distributed without the express
 * permission of Christopher Morris.
 *********************************************************************/

#include <algorithm>
#include <queue>
#include "ColorRefinementAmenability.h"

namespace ColorRefinementAmenability {
    ColorRefinementAmenability::ColorRefinementAmenability(const GraphLibrary::Graph &graph) : m_graph(graph) {}

    bool ColorRefinementAmenability::check_amenability() {
        // Manages colors of stable coloring.
        unordered_set<Label> colors;
        // Maps color to nodes.
        unordered_multimap<Label, Node> colors_to_nodes;
        // Maps node to color.
        unordered_map<Node, Label> node_to_color;

        // Compute stable coloring
        compute_stable_coloring(colors, colors_to_nodes, node_to_color);

        uint num_cells = colors.size();
        // Adjacency matrix of cell graph, initialize all entries with "0".
        vector<vector<uint>> am_cell_graph(num_cells, vector<uint>(num_cells, 0));
        unordered_map<Node, uint> num_nodes_in_cell;
        unordered_map<Node, Label> color_to_cell;
        GraphLibrary::Graph cell_graph;
        int i = 0;
        for (const auto &c: colors) {
            cell_graph.add_node();
            color_to_cell.insert({{c, i}});
            num_nodes_in_cell.insert({{i, colors_to_nodes.count(c)}});

            i++;
        }

        i = 0;
        for (const auto &c: colors) {
            Node node_c = colors_to_nodes.find(c)->second;

            Nodes neighbors_node_c = m_graph.get_neighbours(node_c);
            for (const auto &n: neighbors_node_c) {
                Label c = node_to_color[n];
                uint cell = color_to_cell[c];

                if (am_cell_graph[i][cell] == 0) {
                    cell_graph.add_edge(i, cell);
                }

                am_cell_graph[i][cell] += 1;
            }
            i++;
        }

        unordered_map<Node, uint> vertex_label;
        unordered_map<Edge, uint> edge_label;
        // Check local properties.
        for (uint i = 0; i < num_cells; ++i) {
            // Check property "A".
            if (am_cell_graph[i][i] == num_nodes_in_cell[i] - 1) {  // Is complete.
                vertex_label.insert({{i, 0}});
            } else if (am_cell_graph[i][i] == 0) {  // Is empty
                vertex_label.insert({{i, 0}});
            } else if ((am_cell_graph[i][i] == 2) and (num_nodes_in_cell[i] == 5)) {  // Is 5-cycle.
                vertex_label.insert({{i, 1}});
            } else if ((am_cell_graph[i][i] == 1) and (num_nodes_in_cell[i] >= 4)) {  // Is matching graph.
                vertex_label.insert({{i, 1}});
            } else if ((am_cell_graph[i][i] == num_nodes_in_cell[i] - 2) and
                       (num_nodes_in_cell[i] >= 4)) {  // Is co-matching graph.
                vertex_label.insert({{i, 1}});
            } else {  // Graph violates property "A".
                return false;
            }

            // Check properties "B".
            for (uint j = 0; j < num_cells; ++j) {
                if (i != j) {
                    if (am_cell_graph[i][j] == 0) {  // Is empty.
                        edge_label.insert({{std::make_pair(i, j), 0}});
                    } else if (am_cell_graph[i][j] == num_nodes_in_cell[j]) {  // Is complete bipartite.
                        edge_label.insert({{std::make_pair(i, j), 0}});
                    } else if ((num_nodes_in_cell[j] % am_cell_graph[i][j] == 0) and
                               (am_cell_graph[j][i] == 1) and
                               (am_cell_graph[i][j] * num_nodes_in_cell[i] ==
                                num_nodes_in_cell[j])) {  // Is disjoint union of stars, j nodes are leaves.
                        edge_label.insert({{std::make_pair(i, j), 1}});
                    } else if ((num_nodes_in_cell[i] % am_cell_graph[j][i] == 0) and
                               (am_cell_graph[i][j] == 1) and
                               (am_cell_graph[j][i] * num_nodes_in_cell[j] ==
                                num_nodes_in_cell[i])) {  // Is disjoint union of stars, i nodes are leaves.
                        edge_label.insert({{std::make_pair(i, j), 1}});
                    } else {  // Is bipartite complement of disjoint union of stars?
                        if (num_nodes_in_cell[j] % num_nodes_in_cell[i] == 0) {
                            uint d = num_nodes_in_cell[j] / num_nodes_in_cell[i];

                            if ((am_cell_graph[i][j] == num_nodes_in_cell[j] - d) and
                                (am_cell_graph[j][i] == num_nodes_in_cell[i] - 1)) {
                                edge_label.insert({{std::make_pair(i, j), 1}});
                            } else {
                                return false;
                            }
                        } else if (num_nodes_in_cell[i] % num_nodes_in_cell[j] == 0) {
                            uint d = num_nodes_in_cell[i] / num_nodes_in_cell[j];

                            if ((am_cell_graph[j][i] == num_nodes_in_cell[i] - d) and
                                (am_cell_graph[i][j] == num_nodes_in_cell[j] - 1)) {
                                edge_label.insert({{std::make_pair(j, i), 1}});
                            } else {
                                return false;
                            }
                        } else {  // Does not fulfill property "B".
                            return false;
                        }
                    }
                }
            }
        }

        // Check properties "G" and "H".
        // Find anisotropic compentents and mininal nodes.
        unordered_multimap<uint, Node> anistropic_components;
        unordered_map<Node, int> node_to_ac;
        unordered_map<uint, Node> minimum;
        uint num_ac = -1;

        for (uint i = 0; i < num_cells; ++i) {
            if (node_to_ac.find(i) == node_to_ac.end()) {  // New anisotropic components found.
                num_ac++;

                node_to_ac.insert({{i, num_ac}});
                anistropic_components.insert({{num_ac, i}});

                minimum.insert({{num_ac, i}});

                queue<Node> node_queue;
                node_queue.push(i);

                while (not node_queue.empty()) {
                    Node node = node_queue.front();
                    node_queue.pop();

                    Nodes neighbors_node = cell_graph.get_neighbours(node);
                    for (const auto &n: neighbors_node) {
                        if ((node_to_ac.find(n) == node_to_ac.end()) and
                            (edge_label.find(make_pair(node, n))->second == 1)) {
                            node_to_ac.insert({{n, num_ac}});
                            anistropic_components.insert({{num_ac, n}});
                            node_queue.push(n);

                            if (num_nodes_in_cell[n] < num_nodes_in_cell[node]) {
                                minimum[num_ac] = n;
                            }
                        }
                    }
                }
            }
        }

        unordered_map<Node, bool> visited;
        unordered_map<Edge, bool> check;
        unordered_map<Node, Node> parent;
        for (uint i = 0; i < num_ac; ++i) {
            Node min_node = minimum[i];

            queue<Node> node_queue;
            node_queue.push(min_node);
            uint ac = node_to_ac[min_node];
            visited.insert({{min_node, true}});

            uint num_nodes_min = num_nodes_in_cell[min_node];
            parent.insert({{min_node, -1}});

            while (not node_queue.empty()) {
                Node node = node_queue.front();
                node_queue.pop();

                Nodes neighbors_node = cell_graph.get_neighbours(node);
                for (const auto &n: neighbors_node) {
                    if ((node_to_ac.find(n)->second == ac) and
                        (visited.find(n) == visited.end())) { // Same anisotropic component and not visited yet.
                        if ((n != min_node) and vertex_label.find(n)->second ==
                                                1) {  // Found heterogenous node that is not the minimum node.
                            return false;
                        }

                        uint num_nodes_n = num_nodes_in_cell[n];
                        if (num_nodes_min > num_nodes_n) {  // Monotonicity property violated.
                            return false;
                        }

                        node_queue.push(n);
                        visited.insert({{n, true}});
                        check.insert({{std::make_pair(node, n), true}});
                        parent.insert({{n, node}});

                    } else if ((node_to_ac.find(n)->second == ac) and (visited.find(n) != visited.end()) and
                               (n != parent[node])) {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    Labeling ColorRefinementAmenability::compute_stable_coloring(unordered_set<Label> &node_colors,
                                                                 unordered_multimap<Label, Node> &colors_to_nodes,
                                                                 unordered_map<Node, Label> &node_to_color) const {
        Labeling node_label;
        size_t num_nodes = m_graph.get_num_nodes();

        Labels coloring;
        Labels new_coloring;

        // Labels each node with the same label.
        coloring.resize(num_nodes, 0);
        new_coloring.resize(num_nodes, 1);
        uint num_old_colors = -1;
        uint num_new_colors = 1;

        while (num_new_colors != num_old_colors) {
            unordered_set<Label> new_colors;

            // Update coloring.
            coloring = new_coloring;
            num_old_colors = num_new_colors;

            // Iterate over all nodes.
            for (Node v = 0; v < num_nodes; ++v) {
                Labels colors;
                Nodes neighbors(m_graph.get_neighbours(v));
                colors.reserve(neighbors.size() + 1);

                // Get colors of neighbors.
                for (const Node &n: neighbors) {
                    colors.push_back(coloring[n]);
                }
                sort(colors.begin(), colors.end());
                colors.push_back(coloring[v]);

                // Compute new label using composition to bijectively map two integers to on integer.ŧ
                Label new_color = colors.back();
                colors.pop_back();
                for (const Label &c: colors) {
                    new_color = pairing(new_color, c);
                }

                new_colors.insert(new_color);
                new_coloring[v] = new_color;

            }

            num_new_colors = new_colors.size();
            if (num_new_colors == num_old_colors) {
                uint num_nodes = m_graph.get_num_nodes();

                node_colors = new_colors;

                for (Node i = 0; i < num_nodes; ++i) {
                    colors_to_nodes.insert({{new_coloring[i], i}});
                    node_to_color.insert({{i, new_coloring[i]}});
                }
            }

        }

        return node_label;
    }

    Label ColorRefinementAmenability::pairing(const Label a, const Label b) const {
        return a >= b ? a * a + a + b : a + b * b;
    }

    ColorRefinementAmenability::~ColorRefinementAmenability() {}

}
