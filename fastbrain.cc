/* fastbrain.cc - Brian Klug */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <list>
#include <vector>
#include <random>
#include <queue>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm> 
#include "worldmap.cc"
#include "webserver.cc"

using namespace std;

#define MIN 0
#define MAX UINT_MAX
#define HALFMAX MAX / 2
#define HUMAN_NEURONS 100000000000 // 100 billion
#define HUMAN_CONNECTIONS 100000
#define MOUSE_NEURONS 75*1000*1000 // 75 million
#define MOUSE_CONNECTIONS 140
#define ANT_NEURONS 250000         // 250,000
#define ANT_CONNECTIONS 80

#define NEURONS 25
#define CONNECTIONS 5

class Experiment {

  protected:

    std::multimap<unsigned int /* voltage */, unsigned long int /* neuron number */> queue;
    std::set<unsigned long int /* neuron number */> queued;
    worldmap wm;
    webserver ws;
    int last, lastr;
    bool haswon;
    clock_t start, diff;
    std::vector<unsigned int> *neighbors;
    unsigned int *voltage;
    char *kind;
    bool learning;
    int moves;
    double initial_distance;
    bool debug;

    void fire(unsigned long int neuron, char &kind, std::vector<unsigned int> &my_neighbors) {

        // printf("Nei Addy = %p\n", &my_neighbors);

//        printf("  Firing Neuron #%lu %c size:%lu\n", neuron, kind, my_neighbors.size());
        if (learning) {        
            if (my_neighbors.size()<CONNECTIONS) {
                // static std::random_device rd;
                // static std::mt19937 generator(rd());
                // static std::uniform_int_distribution<unsigned long int> distribution(0, NEURONS);
                // unsigned long int target_neuron = distribution(generator);
                unsigned long int target_neuron = rand() % NEURONS;
//                printf(" Connecting neuron %lu -> %lu \n",neuron, target_neuron);
                my_neighbors.push_back(target_neuron);
            } else {
//                printf(" Unconnecting neuron %lu -> .. \n",neuron);
                my_neighbors.erase(my_neighbors.begin() + rand() % CONNECTIONS);
            }
            
        }

        for(std::vector<unsigned int>::iterator it = my_neighbors.begin(); it != my_neighbors.end(); ++it) {
            unsigned long int n = *it;
            unsigned int v = voltage[n];
            // printf("EQ eV=%imV #%lu\n",-v,n);
            if (queued.find(n)==queued.end() && queue.find(n)==queue.end()) {
                queue.insert(std::make_pair(v,n));
                queued.insert(n);
            } else {
                // printf("    * Dup\n");
            }
        }

        voltage[neuron] /= 4;  // BK /= 4

        if (neuron<4) {
            moves++;
        }
        if (neuron==0) {
            wm.move_fwd();
        }
        if (neuron==1) {
            wm.move_bak();
        }
        if (neuron==2) {
            wm.move_lft();
        }
        if (neuron==3) {
            wm.move_rgt();
        }

    }

    void make_connection(unsigned long int from, std::vector<unsigned int> &my_neighbors, unsigned long int to) {

        if (my_neighbors.size()<CONNECTIONS) {
            // printf(" make_connection %lu -> %lu \n",from, to);
            unsigned long int target_neuron = rand() % NEURONS;
            my_neighbors.push_back(to);
        }

    }

    void maybe_fire(unsigned long int neuron, char &kind, std::vector<unsigned int> &my_neighbors) {

        unsigned int *ev = &voltage[neuron];

        if (neuron==5) {
            *ev += (unsigned int) wm.get_distance_above();
        }
        if (neuron==6) {
            *ev += (unsigned int) wm.get_distance_below();
        }
        if (neuron==7) {
            *ev += (unsigned int) wm.get_distance_left();
        }
        if (neuron==8) {
            *ev += (unsigned int) wm.get_distance_right();
        }

        // printf("Checking Neuron #%lu %c size:%lu eV:%u\n", neuron, kind, my_neighbors.size(), *ev);

        if (*ev>20) {
            fire(neuron, kind, my_neighbors);
        }

        if (neuron<5 || neuron>8) {
            *ev += 2;   // BK +=2 
        }



        voltage[neuron] = *ev;
    }

    unsigned int clamp(unsigned int f) {
        f  = f > MAX ? MAX : f;
        f  = f < MIN ? MIN : f;
        return f;
    }

    unsigned int v_double(unsigned int f) {
        if (f==0) return 0;
        if (f>HALFMAX) return MAX;
        f = f<<1;
        f = f == 0 ? MAX : f;
        f = clamp(f);
        return f;
    }

    int addition_is_safe(uint32_t a, uint32_t b) {
        if (a > INT_MAX - b) return 0;
        return 1;
    }

    unsigned int v_add(unsigned int a, unsigned int b) {
        unsigned int r;
        // doesn't work with -O2, could use -fwrapv instead to not skip overflow checks
        // 	if (__builtin_uadd_overflow(a, b, &r)) {
        // 		return MAX;
        // 	} else {
        // 		return r;
        // 	}
        if (addition_is_safe(a,b)) {
            return a + b;
        } else {
            return MAX;
        }
    }


    void process(char k, unsigned long int nueron, std::vector<unsigned int> &my_neighbors) {
        unsigned int f;
        // printf("voltage[%i]=%u\n",nueron,voltage[nueron]);

        switch (k) {
            case 'A':
                voltage[nueron] = v_double(voltage[nueron]);
                break;
            case 'B':
                voltage[nueron] = v_add(voltage[nueron],3);
                break;
        }

        maybe_fire(nueron, k, my_neighbors);

        return;
    }


    void init_brain() {

        if ((voltage = (unsigned int *) malloc(sizeof(unsigned int) * NEURONS)) == NULL) {
            printf("unable to allocate voltage memory \n");
            return;
        }
        if ((kind = (char *) malloc(sizeof(char) * NEURONS)) == NULL) {
            printf("unable to allocate kind memory \n");
            return;
        }


        for (unsigned long int i=0; i<NEURONS; i++) {
            voltage[i] = 1; //BK 5
            kind[i] = 'A' + (random() % 3);

        }


        neighbors =   new std::vector<unsigned int>[NEURONS];

    }


  public:

    void show_neurons() {

        for (unsigned int j=0; j<NEURONS; j+=25) {
            for (unsigned int i=0; i<25; i++) {
                printf("%7i ",voltage[j+i]);
            }
            printf("\n");
        }
        printf("\n");

    }


    void setup_experiment() {

        // ws.start_server();
        haswon = false;

        wm.initmap();
        init_brain();

        start = clock();
        last = 0;
        lastr= 0;
        moves = 0;
        initial_distance = wm.get_distance_to_cheese();

        learning = false;
        debug = false;

    }

    void set_debug(bool in) {
        debug = in;
    }

    void set_learning_mode(bool mode) {
        learning = mode;
    }

    int get_moves() {
        return moves;
    }


    void reset_board() {

        wm.initmap();
        // wm.movecheese();

        haswon = false;

        start = clock();
        last = 0;
        lastr= 0;
        moves = 0;

    }

    void run_cycle(int cycles) {

        for (int ml=0; ml<cycles; ml++ ) {


            for (unsigned long int i=5; i<(unsigned long int)9; i++) {
                //process(kind[i], i, neighbors[i], voltage);
                char k = 'Z';
                maybe_fire(i, k, neighbors[i]);
            }

            // printf("\n\nDistance to cheese: %f\n\n",wm.get_distance_to_cheese());

            unsigned long int nueron = (rand() % (NEURONS-4))+4; // ws.process_request();
            // rand() or ml


            // printf("Process %ld\n",nueron);
            process(kind[nueron], nueron, neighbors[nueron]);

            while (!queue.empty()) {

                std::map<unsigned int, unsigned long int>::reverse_iterator it = queue.rbegin();

                unsigned int v = it->first;
                unsigned long int n = it->second;

                // printf("DQ eV=%imV #%lu ..\n",v, n);

                // queued.erase(n);
                queue.erase(std::next(it).base());
                process(kind[n], n, neighbors[n]);

                // Restart queue from end again
                it = queue.rbegin();

            }
            queue.clear();
            queued.clear();

            if (learning) {
                int a = rand() % NEURONS;
                int b = rand() % NEURONS;
                make_connection(a,neighbors[a],b);
            }

            diff = clock() - start;
            int msec = diff * 1000 / CLOCKS_PER_SEC;

//            if (false && !haswon && ml%1000==0 and lastr!=ml) {
//                printf("\n[%i] Resetting map\n\n", ml);
//                lastr = ml;
//
//                // Wheeeee!!!!
//
//                free(voltage);
//                free(kind);
//
//                if ((voltage = (unsigned int *) malloc(sizeof(unsigned int) * NEURONS)) == NULL) {
//                    printf("unable to allocate voltage memory \n");
//                    return;
//                }
//
//                if ((kind = (char *) malloc(sizeof(char) * NEURONS)) == NULL) {
//                    printf("unable to allocate kind memory \n");
//                    return;
//
//                }
//                neighbors =   new std::vector<unsigned int>[NEURONS];
//                wm.initmap();
//                init_brain();
//                queue.clear();
//                queued.clear();
//            }

            diff = clock() - start;
            msec = diff * 1000 / CLOCKS_PER_SEC;

            if (last==0) {
                last = msec/1000;
            }

            if (debug || (false && msec%1000==0 && last!=msec/1000)) {
                printf("\n[%i] Time taken %d seconds %d milliseconds\n\n", ml, msec/1000, msec%1000);
                last = msec/1000;
                show_status();
            }


            if (wm.won()) {
                diff = clock() - start;
                int msec = diff * 1000 / CLOCKS_PER_SEC;
                // printf("\n[%i] Time taken %d seconds %d milliseconds.", ml, msec/1000, msec%1000);
//                printf("   Map won in %i ticks.\n",ml);
//                wm.initmap();
//                wm.movecheese();
                //wm.showmap();
                haswon=true;
                return;
            }

            if (false && haswon) {
                show_neurons();
                wm.showmap();
                printf("\n[%i] Paused for network\n",ml);
                nueron =  ws.process_request();
            }



        }

    }

    bool won() {
        return haswon;
    }

    void show_status() {
        show_neurons();
        wm.showmap();
        cout << "Init : " << initial_distance << endl;
        cout << "Dist : " << wm.get_distance_to_cheese() << endl;
        cout << "Score: " << get_score() << endl;
        cout << "Moves: " << moves << endl;
    }

    void clean_up() {

        free(voltage);
        free(kind);

        return;
    }

    float get_score() {
//        cout << "II: " <<initial_distance << endl;
//        return max(initial_distance-wm.get_distance_to_cheese(),0.0) / initial_distance * 100;
        float score = (initial_distance-wm.get_distance_to_cheese()) / initial_distance * 100;
        
        return std::max(score,-10.0f);
    }
    
    void randomize_locations() {
        wm.randomize_locations();
        initial_distance = wm.get_distance_to_cheese();
    }

    //    std::multimap<unsigned int /* voltage */, unsigned long int /* neuron number */> queue;
    //    std::set<unsigned long int /* neuron number */> queued;
    //    worldmap wm;
    //    webserver ws;
    //    int since, score, last, lastr;
    //    bool haswon;
    //    clock_t start, diff;
    //    std::vector<unsigned int> *neighbors;
    //    unsigned int *voltage;
    //    char *kind;
    //    bool learning;
    //

    // overload == operator
    Experiment& operator=(const Experiment& src) {
        init_brain();
        for (int i=0; i<NEURONS; i++) {
            voltage[i] = src.voltage[i];
            kind[i] = src.kind[i];
            for(std::vector<unsigned int>::iterator it = src.neighbors[i].begin(); it != src.neighbors[i].end(); ++it) {
                unsigned long int n = *it;
                neighbors[i].push_back(n);
            }

        }
        learning = false;
        debug = false;
        return *this;
    }

    // copy constructor
    Experiment(const Experiment& src) {
        *this = src;
    }

    Experiment() {
        learning = false;
        debug = false;
    }


    // Load
    friend std::istream& operator>>(std::istream& is, Experiment& e) {

        e.init_brain();

        for (int i=0; i<NEURONS; i++) {
            int n;
            char label[40];

            is >> label >> n;
            if (strcmp(label,"Neuron")!=0) return is;
            if (i!=n) return is;
            is >> label >> e.kind[i];
            if (strcmp(label,"Kind")!=0) return is;
            is >> label >> e.voltage[i];
            if (strcmp(label,"Voltage")!=0) return is;

//            printf("Neuron #%i %c %ieV\n",n, e.kind[i], e.voltage[i]);

            do {
                unsigned int target;
                is >> label;
                // cout << " DW " << label << "\n";

                if (strcmp(label,"EndNeuron")==0) break;
                if (strcmp(label,"Connection")!=0) return is;
                is >> target;
                e.neighbors[i].push_back(target);
//                cout << " Connection -> " << target << "\n";
            } while (strcmp(label,"Connection")==0);

//            cout << "\n";

        }

        return is;
    }


    // Save
    friend std::ostream& operator<<(std::ostream& os, const Experiment& e) {

        for (int i=0; i<NEURONS; i++) {

            os << "Neuron " << i << "\n";
            os << "Kind " << e.kind[i] << "\n";
            os << "Voltage " << e.voltage[i] << "\n";

            for(std::vector<unsigned int>::iterator it = e.neighbors[i].begin(); it != e.neighbors[i].end(); ++it) {
                unsigned long int n = *it;
                os << "Connection " << n << "\n";
            }

            os << "EndNeuron\n\n";
        }
        return os;
    }

    void mutate(int deltas) {
        for (int i=0; i<deltas; i++) {
            unsigned long int neuron = rand() % NEURONS;
            std::vector<unsigned int> *my_neighbors = &neighbors[neuron];
            if (my_neighbors->size()>=CONNECTIONS/2) {
//                cout << "Removing connection from neuron " << neuron << "\n";
                my_neighbors->erase(my_neighbors->begin() + rand() % my_neighbors->size());
                my_neighbors->erase(my_neighbors->begin() + rand() % my_neighbors->size());
            } else {
//                cout << "Adding connection to neuron " << neuron << "\n";
                my_neighbors->push_back(rand() % NEURONS);
            }

        }

    }

};



