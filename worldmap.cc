
#define SIZE 30
#include <stdio.h>
#include <math.h>

class worldmap {

  protected:

    char field[SIZE][SIZE];
    double plx,ply;
    double cx,cy;

  public:

    void move_fwd() {
        if (plx>0) plx--;
//        printf("PLX %f\n",plx);
    }
    void move_bak() {
        if (plx<SIZE-1) plx++;
//        printf("PLX %f\n",plx);
    }
    void move_lft() {
        if (ply>0) ply--;
//        printf("PLY %f\n",ply);
    }
    void move_rgt() {
        if (ply<SIZE-1) ply++;
//        printf("PLY %f\n",ply);
    }

    double get_distance_to_cheese() {
        return sqrt(pow(cx-plx,2)+pow(cy-ply,2));
    }

    double get_greatest_distance_possible() {
        return sqrt(pow(SIZE,2)+pow(SIZE,2));
    }

    double get_distance_above() {
        return fmax((plx-cx),0);
    }
    double get_distance_below() {
        return fmax((cx-plx),0);
    }
    double get_distance_left() {
        return fmax((ply-cy),0);
    }
    double get_distance_right() {
        return fmax((cy-ply),0);
    }

    void initmap() {
        plx = 20;
        ply = 15;
        cx = 2;
        cy = 27;
        for (int x=0; x<SIZE; x++) {
            for (int y=0; y<SIZE; y++) {
                if (x==plx && y==ply)
                    field[x][y]= 'M';
                else
                    field[x][y]= ' ';
            }
        }
        return;
    }

    void randomize_locations() {
        field[(int)plx][(int)ply]= ' ';
        const int edge_border = 2;
        do {
            plx = edge_border + rand() % (SIZE-edge_border*2);
            ply = edge_border + rand() % (SIZE-edge_border*2);
            cx = edge_border + rand() % (SIZE-edge_border*2);
            cy = edge_border + rand() % (SIZE-edge_border*2);
        } while (get_distance_to_cheese()<5 || std::abs(plx-cx)<4 || std::abs(ply-cy)<4);
        field[(int)plx][(int)ply]= 'M';
    }

    void movecheese() {
        cx = rand() % SIZE;
        cy = rand() % SIZE;
    }

    bool won() {
//        printf("PL %f,%f  C %f,%f \n",plx,ply,cx,cy);
        return (cx==plx && cy==ply);
    }

    void showmap() {

        printf("plx: %2f ply: %2f\n\n",plx,ply);
        printf("/");
        for (int y=0; y<SIZE; y++) {
            printf("-");
        }
        printf("\\\n");

        for (int x=0; x<SIZE; x++) {
            printf("|");
            for (int y=0; y<SIZE; y++) {
                if (x==plx && y==ply) {
                    printf("m");
                } else if (x==cx && y==cy) {
                    printf("C");
                } else {
                    printf("%c",field[x][y]);
                }
            }
            printf("|\n");
        }

        printf("\\");
        for (int y=0; y<SIZE; y++) {
            printf("-");
        }
        printf("/\n");

        return;
    }

};