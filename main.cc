
#include "fastbrain.cc"
#include <iostream>
using namespace std;

int main() {
    Experiment exp;
   
    exp.setup_experiment();

//    exp.run_cycle(50000);  
//    exp.show_status();
        
    for (int i=0; i<20; i++) {
        exp.setup_experiment();
        exp.run_cycle(3000);   
        if (exp.won()) {
            cout << i << " won!\n";
            // exp.show_status();
            // break;
        } else {
            cout << i << " lost.\n";
            //exp.show_status();
        }
           
    }
}
