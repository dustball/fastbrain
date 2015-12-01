
#include "fastbrain.cc"

int main() {
    Experiment exp;
   
    exp.setup_experiment();
    exp.run_cycle(1000);
}
