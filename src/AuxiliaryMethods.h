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

#ifndef AMENABILITY_TEST_AUXILIARYMETHODS_H
#define  AMENABILITY_TEST_AUXILIARYMETHODS_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include "Graph.h"

using namespace std;
using namespace GraphLibrary;

namespace AuxiliaryMethods {
    // Simple function for converting a comma separated string into a vector of integers.
    vector<int> split_string(string s);

    // Parser for text-based graph format from http://graphkernels.cs.tu-dortmund.de.
    GraphDatabase read_graph_txt_file(string data_set_name);
}

#endif //  AMENABILITY_TEST_AUXILIARYMETHODS_H
