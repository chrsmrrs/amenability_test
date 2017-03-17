/**********************************************************************
 * Copyright (C) 2017 Christopher Morris <christopher.morris@udo.edu>
 *
 * This file is part of Amendable.
 *
 * Implementation of the algorithm described in:
 *
 * @InProceedings{Arvind+2015,
 *   author = "Arvind, V. and K{\"o}bler, Johannes and Rattan, Gaurav and Verbitsky, Oleg",
 *   title = "On the Power of Color Refinement",
 *   booktitle="20th International Symposium on Fundamentals of Computation Theory",
 *   year="2015",
 *   pages="339--350"
 * }
 *
 * Amendable can not be copied or distributed without the express
 * permission of Christopher Morris.
 *********************************************************************/

#include <chrono>
#include <iostream>
#include "src/AuxiliaryMethods.h"
#include "src/ColorRefinementAmendibility.h"

int main() {
    GraphDatabase graph_data_base = AuxiliaryMethods::read_graph_txt_file("NCI1");
    cout << "Graph data base loaded." << endl;

    double num_is_amendable = 0;
    auto start = chrono::high_resolution_clock::now();
    for (const auto g: graph_data_base) {
        ColorRefinementAmendibility::ColorRefinementAmendibility cra(g);
        if (cra.check_amendability()) {
            num_is_amendable += 1;
        }
    }
    auto end = chrono::high_resolution_clock::now();

    cout << num_is_amendable / graph_data_base.size() * 100.0 << endl;
    cout << "Running time [s]: " << chrono::duration<double>(end - start).count() / graph_data_base.size() << endl;

    return 0;
}