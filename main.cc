#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <iomanip>

#include "fastbrain.cc"

using namespace std;

int main() {

    int version = 1;
    char filename[50];

    Experiment control_mouse1, control_mouse2;

    control_mouse1.setup_experiment();
    control_mouse2.setup_experiment();

//
//    for (int i=0; i<200; i++) {
//        control_mouse1.setup_experiment();
//
//        ifstream ifs("deform.brain");
//        ifs >> control_mouse1;
//        ifs.close();
//
//        control_mouse1.set_learning_mode(false);
//        control_mouse1.run_cycle(500);
////        control_mouse1.show_status();
//
//        double score = control_mouse1.get_score();
//        if (control_mouse1.won()) {
//            cout << i << " won! Score: " << score << "\n";
//            break;
//        } else {
//            cout << i << " lost. Score: " << score << "\n";
//        }
//    }
//
//    control_mouse2 = control_mouse1;


    Experiment expiremantal_mouse1(control_mouse1), expiremantal_mouse2(control_mouse2);


//                ofstream ofs("fixed.1");
//                ofs << control_mouse1;
//                ofs.close();
//
//                ofstream ofs2("fixed.2");
//                ofs2 << control_mouse2;
//                ofs2.close();

    expiremantal_mouse1.mutate(1);
    expiremantal_mouse2.mutate(1);

    int test_iterations = 0;
    double cm1_score_total = 0.0, cm2_score_total = 0.0, em1_score_total = 0.0, em2_score_total = 0.0;

    do {

        test_iterations++;



        const int cycles = 800;

        control_mouse1.reset_board();
        control_mouse1.randomize_locations();
        control_mouse1.set_learning_mode(false);
        control_mouse1.set_debug(false);
        control_mouse1.run_cycle(cycles);
        double cm1_score = control_mouse1.get_score();
        double moves = control_mouse1.get_moves();

        control_mouse2.reset_board();
        control_mouse2.randomize_locations();
        control_mouse2.set_learning_mode(false);
        control_mouse2.run_cycle(cycles);
        double cm2_score = control_mouse2.get_score();

        expiremantal_mouse1.reset_board();
        expiremantal_mouse1.randomize_locations();
        expiremantal_mouse1.set_learning_mode(false && test_iterations==1);
        expiremantal_mouse1.run_cycle(cycles);
        double em1_score = expiremantal_mouse1.get_score();
        double em1_moves = expiremantal_mouse1.get_moves();

        expiremantal_mouse2.reset_board();
        expiremantal_mouse2.randomize_locations();
        expiremantal_mouse2.set_learning_mode(false && test_iterations==1);
        expiremantal_mouse2.run_cycle(cycles);
        double em2_score = expiremantal_mouse2.get_score();
        double em2_moves = expiremantal_mouse2.get_moves();

        cm1_score_total += cm1_score;
        cm2_score_total += cm2_score;
        em1_score_total += em1_score;
        em2_score_total += em2_score;

//cout << "EM2 Score: "<< em2_score << " " << em2_score_total << endl;

        double cm1_average = cm1_score_total / test_iterations;
        double cm2_average = cm2_score_total / test_iterations;
        double em1_average = em1_score_total / test_iterations;
        double em2_average = em2_score_total / test_iterations;

        if (true || test_iterations % 100==20) {

            // cout << "I: " << test_iterations << " ";

            cout << "Won: " << std::fixed << setprecision(1) << setw(5) << (cm2_average+cm1_average)/2 << "% ";
            cout << "m=" << std::fixed << std::setfill('0') << setprecision(0) << setw(3) << moves << " ";
            cout << "m1=" << std::fixed << setprecision(0) << setw(3) << em1_moves << " ";
            cout << "m2=" << std::fixed << setprecision(0) << setw(3) << em2_moves << " ";
            cout << "Δ=" << std::fixed << std::setfill(' ') << setprecision(5) << setw(8) << abs(cm2_average-cm1_average) << " ";
            cout << "CM1: " << std::fixed << setprecision(4) << setw(8) << cm1_average << setprecision(1) << setw(6) << cm1_score << "% ";
            cout << "CM2: " << std::fixed << setprecision(4) << setw(8) << cm2_average << setprecision(1) << setw(6) << cm2_score << "% ";
            cout << "EM1: " << std::fixed << setprecision(4) << setw(8) << em1_average << setprecision(1) << setw(6) << em1_score << "% ";
            cout << "EM2: " << std::fixed << setprecision(4) << setw(8) << em2_average << setprecision(1) << setw(6) << em2_score << "% ";
            cout << "1Δ=" << std::fixed << setprecision(5) << setw(8) << (em1_average-((cm1_average+cm2_average)/2)) << " ";
            cout << "2Δ=" << std::fixed << setprecision(5) << setw(8) << (em2_average-((cm1_average+cm2_average)/2)) << " ";
            cout << "\n";
        }

        if (test_iterations >= 20  && (cm1_average+cm2_average)/2>99.9) {
            version++;
            cout << endl;
            cout << "  Experiment complete.  Mouse has found the cheese ";
            cout << std::fixed << setprecision(1) << (cm2_average+cm1_average)/2 << "% of the time." << endl;
            cout << "  Saving New Version " << version << "\n";
            sprintf(filename,"mouse.brain.v%i",version);
            ofstream ofs(filename);
            ofs << control_mouse1;
            ofs.close();

            sprintf(filename,"mouse.brain.v%i.gv",version);
            control_mouse1.graphviz(filename);

            exit(1);
        }

        if (test_iterations >= 50  && abs(cm2_average-cm1_average)<2) {
            // pick a winner
            cout << endl;

            if (em1_average>em2_average && em1_average>max(cm1_average,cm2_average)+1) {
                cout << "Winner: Experimental Mouse #1\n";
                cout << "  Score: " << std::fixed << setprecision(1) << setw(4) << em1_average << "%\n";
                cout << "  Δ=+" << std::fixed << setprecision(5) << setw(7) << em1_average-max(cm2_average,cm1_average) << "\n";
                expiremantal_mouse2 = expiremantal_mouse1;
                control_mouse1 = expiremantal_mouse1;
                control_mouse2 = expiremantal_mouse1;
            } else if (em2_average>em1_average && em2_average>max(cm1_average,cm2_average)+1) {
                cout << "Winner: Experimental Mouse #2\n";
                cout << "  Score: " << std::fixed << setprecision(1) << setw(4) << em2_average << "%\n";
                cout << "  Δ=+" << std::fixed << setprecision(5) << setw(7) << em2_average-max(cm2_average,cm1_average) << "\n";
                expiremantal_mouse1 = expiremantal_mouse2;
                control_mouse1 = expiremantal_mouse2;
                control_mouse2 = expiremantal_mouse2;
            } else {
                cout << "Winner: Control Group\n";
                cout << "  Score: " << std::fixed << setprecision(1) << setw(4) << ((cm2_average+cm1_average)/2) << "%\n";
                control_mouse2 = control_mouse1;
                expiremantal_mouse1 = control_mouse1;
                expiremantal_mouse2 = control_mouse1;

            }

            version++;
            cout << "  Saving New Version " << version << "\n";
            sprintf(filename,"mouse.brain.v%i",version);
            ofstream ofs(filename);
            ofs << control_mouse1;
            ofs.close();

            int mutate = (int) 100- ((cm2_average+cm1_average)/2);
            cout << "  Mutate: " << mutate << endl << endl;

            expiremantal_mouse1.mutate(mutate);
            expiremantal_mouse2.mutate(mutate);

            cm1_score_total = 0.0, cm2_score_total = 0.0, em1_score_total = 0.0, em2_score_total = 0.0;
            test_iterations = 0;


//                this_thread::sleep_for (chrono::seconds(1));
        }



    } while (1);

//
//        cout << "Average Score: " << (score_total/tests) << "\n";
//        cout << "Best Score: " << (best_score_yet) << "\n\n";
//
//        if (score_total/tests>best_score_yet) {
//            best_score_yet = score_total/tests;
//            version++;
//            cout << "\n\nSaving New Version " << version << "\n\n";
//            sprintf(filename,"mouse.brain.v%i",version);
//            ofstream ofs2(filename);
//            ofs2 << derivative;
//            ofs2.close();
//            exp = derivative;
//            std::this_thread::sleep_for (std::chrono::seconds(4));
//        }


}


int old_main() {

    Experiment exp;
    int version = 1;
    char filename[50];

    // Step 1. Find a neural network that finds the cheese at least once

    exp.set_learning_mode(true);

    for (int i=0; i<200; i++) {
        exp.setup_experiment();
        exp.run_cycle(400);

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
