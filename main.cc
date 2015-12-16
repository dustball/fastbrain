
#include "fastbrain.cc"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

int main() {

    Experiment exp;
    int version = 1;
    char filename[50];

    // Step 1. Find a neural network that finds the cheese at least once

    exp.set_learning_mode(true);

    for (int i=0; i<200; i++) {
        exp.setup_experiment();
        exp.run_cycle(200);

        double score = exp.get_score();
        if (exp.won()) {
            cout << i << " won! Score: " << score << "\n";
            break;
        } else {
            cout << i << " lost. Score: " << score << "\n";
        }
    }

    cout << "\n";

    // Step 1a. Save the winning brain to disk
    sprintf(filename,"mouse.brain.v%i",version);
    ofstream ofs(filename);
    ofs << exp;
    ofs.close();

    // Step 2. Test the brain over-and-over and score it, mutate it, repeat

    double best_score_yet = 0.0;

    do {

        double score_total = 0.0;
        const int tests = 200;

        Experiment derivative = exp;
        derivative.mutate(4);
        // std::this_thread::sleep_for (std::chrono::seconds(2));

        for (int i=0; i<tests; i++) {

            Experiment copy = derivative;

            copy.setup_experiment();
            copy.set_learning_mode(false);
            copy.reset_board();
            copy.randomize_locations();
            copy.run_cycle(100);

            double score = copy.get_score();
            score_total += score;

            if (copy.won()) {
                cout << i << " won! Score: " << score << "\n";
            } else {
                cout << i << " lost. Score: " << score << "\n";
                // copy.show_status();
            }

        }

        cout << "Average Score: " << (score_total/tests) << "\n";
        cout << "Best Score: " << (best_score_yet) << "\n\n";

        if (score_total/tests>best_score_yet) {
            best_score_yet = score_total/tests;
            version++;
            cout << "\n\nSaving New Version " << version << "\n\n";
            sprintf(filename,"mouse.brain.v%i",version);
            ofstream ofs2(filename);
            ofs2 << derivative;
            ofs2.close();
            exp = derivative;
            std::this_thread::sleep_for (std::chrono::seconds(1));
        }

    } while (1);


}
