
#include "fastbrain.cc"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    Experiment exp;



    exp.set_learning_mode(true);

    // Step 1: Find a neural network that finds the cheese at least once

    for (int i=0; i<50; i++) {
        exp.setup_experiment();
        exp.run_cycle(1000);
        if (exp.won()) {
            cout << i << " won!\n\n";
            break;
        } else {
            cout << i << " lost.\n";
        }
    }

    // exp.save_brain("won.brain");

    ofstream ofs("won.brain");

    ofs << exp;
    ofs.close();



    Experiment copy;

    ifstream ifs("won.brain");

    // read the object back in
    ifs >> copy;
    ifs.close();

    ofstream ofs2("copy.brain");

    ofs2 << copy;
    ofs2.close();

    return 1;

    exp.set_learning_mode(false);

    // Step 2: Test the network 100 times and score it

    for (int i=0; i<100; i++) {
        exp.reset_board();
        exp.run_cycle(1000);
        if (exp.won()) {
            cout << i << " won!\n";
        } else {
            cout << i << " lost.\n";
        }
    }


}
