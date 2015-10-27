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

#define MIN 0
#define MAX UINT_MAX
#define HALFMAX MAX / 2 
#define HUMAN_NEURONS 100000000000 // 100 billion
#define HUMAN_CONNECTIONS 100000
#define MOUSE_NEURONS 75*1000*1000 // 75 million
#define MOUSE_CONNECTIONS 140
#define ANT_NEURONS 250000         // 250,000
#define ANT_CONNECTIONS 80

#define NEURONS 5
#define CONNECTIONS 2


std::multimap<unsigned int /* voltage */, unsigned long int /* neuron number */> queue;
std::set<unsigned long int /* neuron number */> queued;
 
void fire(unsigned long int neuron, char &kind, std::vector<unsigned int> &my_neighbors, unsigned int voltage[] ) {
    
    // printf("Nei Addy = %p\n", &my_neighbors);
    
    // printf("  Firing Neuron #%lu %c size:%lu\n", neuron, kind, my_neighbors.size());
    if (my_neighbors.size()<CONNECTIONS) { 
        // static std::random_device rd;
        // static std::mt19937 generator(rd());
        // static std::uniform_int_distribution<unsigned long int> distribution(0, NEURONS);
        // unsigned long int target_neuron = distribution(generator);
        // printf(" Connecting neuron %i -> %i \n",neuron, target_neuron);  
        unsigned long int target_neuron = rand() % NEURONS;
        my_neighbors.push_back(target_neuron);
    }  
    
    for(std::vector<unsigned int>::iterator it = my_neighbors.begin(); it != my_neighbors.end(); ++it) {
        unsigned long int n = *it;
        unsigned int v = voltage[n];
        printf("  EQ eV=%imV #%lu\n",-v,n);
        if (queued.find(n)==queued.end()) {
            queue.insert(std::make_pair(v,n));
            queued.insert(n);
        } else {
            printf("    * Dup\n");
        }
    }

    voltage[neuron] /= 2;

}

void maybe_fire(unsigned long int neuron, char &kind, std::vector<unsigned int> &my_neighbors, unsigned int voltage[]) {
    
    unsigned int *ev = &voltage[neuron];
    
     printf("Checking Neuron #%lu %c size:%lu eV:%u\n", neuron, kind, my_neighbors.size(), *ev);
    
    if (*ev>10) {
        fire(neuron, kind, my_neighbors, voltage);
    }
    *ev += 1;
    
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


void process(char k, unsigned long int nueron, std::vector<unsigned int> &my_neighbors, unsigned int voltage[]) {
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
	
	maybe_fire(nueron, k, my_neighbors, voltage);
	   
	return; 
}
	

void init_brain(char kind[], unsigned int voltage[], std::vector<unsigned int> *neighbors) {
    for (unsigned long int i=0; i<NEURONS; i++) {
        voltage[i] = 5;
        kind[i] = 'C'; 
       
    }
    // Todo: why doesn't this work
    // neighbors = new std::vector<unsigned int>[NEURONS];
}

int main() { 
	unsigned int *voltage = NULL;
	if ((voltage = (unsigned int *) malloc(sizeof(unsigned int) * NEURONS)) == NULL) {
		printf("unable to allocate voltage memory \n");
		return -1; 
	}  
				
	char *kind = NULL;
	if ((kind = (char *) malloc(sizeof(char) * NEURONS)) == NULL) {
		printf("unable to allocate kind memory \n");
		return -1; 
	} 
	
	std::vector<unsigned int> *neighbors = NULL;
	neighbors =   new std::vector<unsigned int>[NEURONS];

    // memcpy(kind,"BFBB\0",5);
	
	
	
    init_brain(kind,voltage,neighbors);
	
	
    // memcpy(kind,"AX",2);
    // kind[1] = 'R';
    // 	voltage[0] = 5;
    // 	voltage[1] = 3;
	
	clock_t start = clock(), diff;
	
	for (int ml=0; ml<7; ml++) {
	   
    	for (unsigned long int i=0; i<(unsigned long int)NEURONS; i++) {  
    	   process(kind[i], i, neighbors[i], voltage);
    	}	
    	diff = clock() - start;
        
        
        while (!queue.empty()) {
            
            std::map<unsigned int, unsigned long int>::reverse_iterator it = queue.rbegin();
                        
            unsigned int v = it->first;
            unsigned long int n = it->second;
            
            printf("DQ eV=%imV #%lu ..\n",v, n);
            
            queued.erase(n);
            queue.erase(std::next(it).base());
            process(kind[n], n, neighbors[n], voltage);          
            
            // Restart queue from end again
            it = queue.rbegin();
           
        }
        queue.clear();

	}

	int msec = diff * 1000 / CLOCKS_PER_SEC;
	//printf("voltage[0]=%u\n",voltage[0]);
	printf("\nTime taken %d seconds %d milliseconds", msec/1000, msec%1000);
	printf("\nOK\n\n");
	
	free(voltage);
	free(kind);

	
	return 0;
}



