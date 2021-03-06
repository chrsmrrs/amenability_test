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

#ifndef AMEN_COLORREFINEMENTAMENABILITY_H
#define AMEN_COLORREFINEMENTAMENABILITY_H

#include <iostream>
#include <unordered_map>
#include "Graph.h"

using Label = ulong;
using Labels = vector<Label>;
using Labeling = std::unordered_multimap<Label, Node>;


namespace ColorRefinementAmenability {
    class ColorRefinementAmenability {
    public:
        // Constructor.
        explicit ColorRefinementAmenability(const GraphLibrary::Graph &graph);

        // Check if graph "m_graph" is amenable.
        bool check_amenability();

        // Destructor.
        ~ColorRefinementAmenability();

    private:
        // A graph.
        GraphLibrary::Graph m_graph;

        // Compute stable coloring of graph "m_graph".
        Labeling
        compute_stable_coloring(unordered_set<Label> &node_colors, unordered_multimap<Label, Node> &colors_to_nodes,
                                unordered_map<Node, Label> &node_to_color) const;

        // Bijection to map two labels to a single label.
        Label pairing(const Label a, const Label b) const;
    };
}

#endif //AMEN_COLORREFINEMENTAMENABILITY_H
