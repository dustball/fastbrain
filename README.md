# fastbrain

A performance optimized neural network written in C++.

## Mouse Experiment

* An empty/blank neural network of an arbitrary size is created - anywhere from 25 nodes to 25 million nodes (or more, really)* There are four OUTPUT neurons that are hardwired to neuron number 0, 1, 2, and 3; for moving forward, back, left and right respectively.  This would be equivalent to an SNS neuron in an animal.  Whenever one of these neurons fires, it causes the mouse to move.* There are four INPUT neurons that are hardwired to simple "cheese sensors".  Neuron numbers 5, 6, 7 and 8 will fire more or less often depending on how strong the cheese smell is towards its front, back, left or right.* None of the neurons are connected at start* If a human being were to intervene, the most obvious solution is to hardwire neuron number 5 to neuron number 0, neuron 6 to neuron 0, and so forth.  The goal of this experiment is to naturally develop connections that will get the mouse to the cheese without God (that's us) stepping in and wiring things properly.* The program will create a control group consisting of two mice, and an experimental group of two additional mice  * fastbrain will create a random mutation in each of the experimental mice.  The control mice are untouched.  * Next the program will run the mice through thousands of mazes.  At first, nothing will happen.  The motor neurons have nothing connected to them.  Each iteration of experimental mutation adds neural connections that will improve or worsen the success rate.    * Many attempts will be wrong.  A pathway might get connected from the “cheese is forward” neuron to the “move backward” neuron.  Imagine a mouse that wants to eat cheese but is wired to run away from it.  The experiment logic will test the neural network and compare it to the control (“best neural network so far” in reality).   * The program runs each of the four mice through mazes over and over until enough data is gathered that the two control mice have 1% or less maze success rate  * fastbrain determines a winner: control, em1 (experimental mouse #1) or em2, and resets the experiment so that all four mice, control and experiment, are now based on this new generation.  * The program continues to build a neuron network until the mouse finds the cheese 100% of the time.* The program saves the neural network to a file after every iteration for inspection* The program generates a Graphiviz image once the experiment concludes and the neural network is complete. See below.

# Requirements

* C++ 11
* Grunt - https://www.npmjs.com/package/grunt

Run `grunt` from the command line to build.  The default target will watch the C++ files and automatically recompile the executable as you edit the code.  This is intended to make development easier and more fun.  In a seperate terminal window, run `./loop.bash` to repeatedly run the executable over and over.  If the code compiles successfully, grunt will `kill -9` the process.  

CC flags optimized for cclang (Mac OSX).

# Sample Output

```
Key:

Won: Current maze success rate (average of two control mice).
m, m1, m2: How many mouse moves the control, experimental mouse 1 & 2 made in the last maze.
Δ: The difference between the two control mice (will run until <1%)
CM1: Control Mouse 1 - Average maze success rate & last score. 
CM2: Control Mouse 2
EM1: Experimental Mouse 1
EM2: Experimental Mouse 2
1Δ: The improvement (or worsening) of experimental mouse 1 vs. control average
2Δ: The improvement (or worsening) of experimental mouse 2 vs. control average

Won:  37.9% m=073 m1=023 m2=046 Δ= 1.38093 CM1:  38.5516  43.5% CM2:  37.1707  58.1% EM1:  59.7924 100.0% EM2:  22.3320 -10.0% 1Δ=21.93125 2Δ=-15.52917 
Won:  38.3% m=177 m1=077 m2=015 Δ= 3.50227 CM1:  36.5243  -6.0% CM2:  40.0266 100.0% EM1:  58.9623  41.5% EM2:  20.9014  -9.1% 1Δ=20.68684 2Δ=-17.37404 
Won:  38.7% m=102 m1=054 m2=064 Δ= 7.15505 CM1:  35.1594   5.1% CM2:  42.3145  92.6% EM1:  56.2676  -3.0% EM2:  20.3289   7.7% 1Δ=17.53065 2Δ=-18.40803 
Won:  36.9% m=069 m1=076 m2=040 Δ= 6.56849 CM1:  33.5662  -3.1% CM2:  40.1347 -10.0% EM1:  57.4191  83.9% EM2:  19.0652 -10.0% 1Δ=20.56867 2Δ=-17.78524 
Won:  37.2% m=096 m1=017 m2=014 Δ=10.70575 CM1:  31.8236 -10.0% CM2:  42.5293 100.0% EM1:  59.1224 100.0% EM2:  17.9026 -10.0% 1Δ=21.94592 2Δ=-19.27383 
Won:  37.5% m=033 m1=018 m2=079 Δ= 6.17786 CM1:  34.4457 100.0% CM2:  40.6236  -7.0% EM1:  60.6946 100.0% EM2:  16.8294 -10.0% 1Δ=23.15991 2Δ=-20.70524 
Won:  37.9% m=059 m1=061 m2=048 Δ= 3.28224 CM1:  36.2881  84.2% CM2:  39.5703  12.2% EM1:  59.1340  18.6% EM2:  16.5971  10.6% 1Δ=21.20482 2Δ=-21.33206 
Won:  36.2% m=153 m1=090 m2=048 Δ= 3.16502 CM1:  34.6349 -10.0% CM2:  37.7999 -10.0% EM1:  60.1421  87.4% EM2:  15.6472 -10.0% 1Δ=23.92468 2Δ=-20.57020 
Won:  37.2% m=055 m1=090 m2=013 Δ= 3.09594 CM1:  35.6312  63.5% CM2:  38.7271  64.7% EM1:  61.1460  89.3% EM2:  14.7628 -10.0% 1Δ=23.96680 2Δ=-22.41632 
Won:  38.4% m=082 m1=020 m2=039 Δ= 2.07359 CM1:  37.3929  88.5% CM2:  39.4665  60.9% EM1:  62.4411 100.0% EM2:  16.0905  54.6% 1Δ=24.01136 2Δ=-22.33925 
Won:  37.4% m=139 m1=077 m2=046 Δ= 0.88745 CM1:  36.9834  24.7% CM2:  37.8708 -10.0% EM1:  63.0151  80.2% EM2:  15.2489 -10.0% 1Δ=25.58801 2Δ=-22.17825 

Winner: Experimental Mouse #1
  Score: 63.0%
  Δ=+25.14428
  Saving New Version 17
  Mutate: 62

Won:  41.9% m=075 m1=155 m2=026 Δ=77.29001 CM1:   3.2843   3.3% CM2:  80.5743  80.6% EM1: -10.0000 -10.0% EM2: -10.0000 -10.0% 1Δ=-51.92927 2Δ=-51.92927 
Won:  40.6% m=059 m1=026 m2=046 Δ=80.83839 CM1:   0.1640  -3.0% CM2:  81.0024  81.4% EM1:  12.7767  35.6% EM2:   7.1735  24.3% 1Δ=-27.80644 2Δ=-33.40966 
Won:  45.3% m=083 m1=020 m2=013 Δ=45.40607 CM1:  22.6446  67.6% CM2:  68.0507  42.1% EM1:   5.9172  -7.8% EM2:   7.0957   6.9% 1Δ=-39.43042 2Δ=-38.25189 
Won:  59.0% m=016 m1=246 m2=015 Δ=34.05456 CM1:  41.9834 100.0% CM2:  76.0380 100.0% EM1:   1.9379 -10.0% EM2:   2.8218 -10.0% 1Δ=-57.07281 2Δ=-56.18892 
Won:  67.2% m=028 m1=169 m2=015 Δ=27.24364 CM1:  53.5868 100.0% CM2:  80.8304 100.0% EM1:  -0.4497 -10.0% EM2:   2.1781  -0.4% 1Δ=-67.65825 2Δ=-65.03047 
Won:  62.4% m=066 m1=261 m2=031 Δ=21.35166 CM1:  51.7156 100.0% CM2:  73.0673 100.0% EM1:   8.5997   2.7% EM2:   4.7876  48.9% 1Δ=-53.79178 2Δ=-57.60390 
Won:  64.1% m=038 m1=036 m2=013 Δ=20.38113 CM1:  53.9104 100.0% CM2:  74.2915 100.0% EM1:  10.8650  58.4% EM2:   6.2263  36.4% 1Δ=-53.23590 2Δ=-57.87467 
Won:  63.4% m=069 m1=028 m2=013 Δ=23.92893 CM1:  51.4803  -2.0% CM2:  75.4093 100.0% EM1:  11.3051  21.0% EM2:   5.5208 -10.0% 1Δ=-52.13969 2Δ=-57.92403 

... 5000+ lines omitted ...

Won: 100.0% m=022 m1=082 m2=045 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1: 100.0000 100.0% EM2: 100.0000 100.0% 1Δ= 0.00000 2Δ= 0.00000 
Won: 100.0% m=038 m1=104 m2=040 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1: 100.0000 100.0% EM2: 100.0000 100.0% 1Δ= 0.00000 2Δ= 0.00000 
Won: 100.0% m=039 m1=059 m2=041 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1: 100.0000 100.0% EM2: 100.0000 100.0% 1Δ= 0.00000 2Δ= 0.00000 
Won: 100.0% m=036 m1=085 m2=014 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1: 100.0000 100.0% EM2: 100.0000 100.0% 1Δ= 0.00000 2Δ= 0.00000 
Won: 100.0% m=057 m1=108 m2=046 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1: 100.0000 100.0% EM2: 100.0000 100.0% 1Δ= 0.00000 2Δ= 0.00000 
Won: 100.0% m=090 m1=083 m2=097 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1: 100.0000 100.0% EM2: 100.0000 100.0% 1Δ= 0.00000 2Δ= 0.00000 
Won: 100.0% m=033 m1=033 m2=046 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1: 100.0000 100.0% EM2: 100.0000 100.0% 1Δ= 0.00000 2Δ= 0.00000 
Won: 100.0% m=102 m1=147 m2=048 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1:  98.6025  88.8% EM2: 100.0000 100.0% 1Δ=-1.39754 2Δ= 0.00000 
Won: 100.0% m=111 m1=084 m2=048 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1:  98.7577 100.0% EM2: 100.0000 100.0% 1Δ=-1.24226 2Δ= 0.00000 
Won: 100.0% m=012 m1=075 m2=066 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1:  98.8820 100.0% EM2: 100.0000 100.0% 1Δ=-1.11803 2Δ= 0.00000 
Won: 100.0% m=097 m1=102 m2=109 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1:  98.9836 100.0% EM2: 100.0000 100.0% 1Δ=-1.01639 2Δ= 0.00000 
Won: 100.0% m=075 m1=161 m2=081 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1:  99.0683 100.0% EM2: 100.0000 100.0% 1Δ=-0.93170 2Δ= 0.00000 
Won: 100.0% m=068 m1=035 m2=130 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1:  99.1400 100.0% EM2: 100.0000 100.0% 1Δ=-0.86003 2Δ= 0.00000 
Won: 100.0% m=056 m1=092 m2=124 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1:  99.2014 100.0% EM2: 100.0000 100.0% 1Δ=-0.79860 2Δ= 0.00000 
Won: 100.0% m=061 m1=059 m2=053 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1:  99.2546 100.0% EM2: 100.0000 100.0% 1Δ=-0.74536 2Δ= 0.00000 
Won: 100.0% m=065 m1=046 m2=058 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1:  99.3012 100.0% EM2: 100.0000 100.0% 1Δ=-0.69877 2Δ= 0.00000 
Won: 100.0% m=103 m1=028 m2=032 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1:  99.3423 100.0% EM2: 100.0000 100.0% 1Δ=-0.65767 2Δ= 0.00000 
Won: 100.0% m=053 m1=040 m2=046 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1:  99.3789 100.0% EM2: 100.0000 100.0% 1Δ=-0.62113 2Δ= 0.00000 
Won: 100.0% m=097 m1=118 m2=154 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1:  99.4116 100.0% EM2: 100.0000 100.0% 1Δ=-0.58844 2Δ= 0.00000 
Won: 100.0% m=082 m1=101 m2=063 Δ= 0.00000 CM1: 100.0000 100.0% CM2: 100.0000 100.0% EM1:  99.4410 100.0% EM2: 100.0000 100.0% 1Δ=-0.55902 2Δ= 0.00000 

  Experiment complete.  Mouse has found the cheese 100.0% of the time.
  Saving New Version 85  
````  

# Graphviz Output

The program will automatically generate Graphviz visualizations for the neural network.

## Simple 25 Neuron Network, 4 input neurons, 4 output neurons

![25neuron.v85](output/mouse.brain.25neuron.v85.png?raw=true "25neuron.v85")

## 500 Neuron Network, same 4 input/output neurons

This is a real, working neural network.  The mouse will find the cheese 100% of the time.  This network was built by 4,174 generations of mice.  

![500neuron.v4174](https://cdn.rawgit.com/dustball/fastbrain/master/output/mouse.brain.500neuron.v4174.svg "500neuron.v4174")

