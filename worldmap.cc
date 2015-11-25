
#define SIZE 30

class worldmap {

 protected:
 
 char field[SIZE][SIZE];
 int plx,ply;
 
 public:
 
 void move_fwd() {
  if (plx>0) plx--;  
  printf("PLX %i\n",plx);
 }
 
 void initmap() {
    plx = 20;
    ply = 15;
    for (int x=0; x<SIZE; x++) {      
      for (int y=0; y<SIZE; y++) {    
         field[x][y]= ' '; 
      }
    }
    return;
}
    
    
 void showmap() {
    
    printf("/");
    for (int y=0; y<SIZE; y++) {    
       printf("-");
    }
    printf("\\\n");


      
    for (int x=0; x<SIZE; x++) {
      printf("|");
      for (int y=0; y<SIZE; y++) {    
         if (x==plx && y==ply) {
             printf("M"); 
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