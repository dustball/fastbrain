/* v1.c - Brian Klug */

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <limits.h>
#include <list>
#include <vector>
#include <random>

#define MIN 0
#define MAX UINT_MAX
#define HALFMAX MAX / 2
#define SIZE 1024*1024
#define TESTBATCH 10
#define TARGET TESTBATCH / 2

void fire(int neuron, std::vector<unsigned int> &my_neighbors ) {
    printf("B Size %lu\n", my_neighbors.size());
    if (my_neighbors.size()<TARGET) {
        std::random_device rd;
        std::default_random_engine generator(rd());
        std::uniform_int_distribution<int> distribution(0, TESTBATCH);
        int random_int = distribution(generator);
        printf(" Adding %i\n",random_int);  
        my_neighbors.push_back(random_int);
    } 
}
	
unsigned int clamp(unsigned int f) {
	f  = f > MAX ? MAX : f;
	f  = f < MIN ? MIN : f;
	return f;
}

unsigned int v_double(unsigned int f) {
	if (f==0) return 0;
    if (f>HALFMAX) return MAX; 
	f = f>>1;
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
    //	if (__builtin_uadd_overflow(a, b, &r)) {
    //		return MAX;
    //	} else {
    //		return r;
    //	}	
	if (addition_is_safe(a,b)) {
		return a + b;
	} else {
		return MAX;
	}
}


void process(char k, int nueron, unsigned int voltage[]) {
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
	return; 
}
	

int main() {
	unsigned int *voltage = NULL;
	if ((voltage = (unsigned int *) malloc(sizeof(unsigned int) * SIZE)) == NULL) {
		printf("unable to allocate voltage memory \n");
		return -1; 
	}  
				
	char *kind = NULL;
	if ((kind = (char *) malloc(sizeof(char) * SIZE)) == NULL) {
		printf("unable to allocate kind memory \n");
		return -1; 
	} 
	
	std::vector<unsigned int> *neighbors = NULL;
	
	if ((neighbors = (std::vector<unsigned int> *) malloc(sizeof(std::vector<unsigned int>) * SIZE)) == NULL) {
		printf("unable to allocate neighbors memory \n");
		return -1; 
	}   
	
	memcpy(kind,"BFBB\0",5);
	
	//memcpy(kind,"AX",2);
	// kind[1] = 'R';
	
	voltage[0] = 5;
	voltage[1] = 3;
	
	printf("&voltage=%u\n",voltage[0]);
	//printf("&kind=%p\n",&kind[0]);
	//printf("kind=%s\n", kind);
	
	clock_t start = clock(), diff;
	
	for (unsigned long long int i=0; i<(unsigned long long int)TESTBATCH; i++) {  
		process(kind[0], 0, voltage);
	}
	
	fire(0, neighbors[0]);
	fire(0, neighbors[0]);
	fire(0, neighbors[0]);
	fire(0, neighbors[0]);
	fire(0, neighbors[0]);
	fire(0, neighbors[0]);
	fire(0, neighbors[0]);
	fire(1, neighbors[1]);
	fire(1, neighbors[1]);
	
	diff = clock() - start;
	int msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("voltage[0]=%u\n",voltage[0]);
	printf("Time taken %d seconds %d milliseconds", msec/1000, msec%1000);

	printf("\nOK\n\n");
	
	return 0;
}
