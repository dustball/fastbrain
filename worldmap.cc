
#define SIZE 30

class worldmap {

 protected:
 
 char field[SIZE][SIZE];
 int plx,ply;
 
 public:
 
 void initmap() {
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
         printf("%c",field[x][y]); 
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