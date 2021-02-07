#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int size, rank;
int entered_number;
char a[256], b[9];
int match(char*, char*);


void getUserInfo(){
    printf("Enter some text\n");
    gets(a);

    printf("Enter a string to find\n");
    gets(b);
}

void parallelism(){
    int position;
    
    int process_per_self=256/4+8;//katar-1
    
    int j;
        if (rank == 0) { // Master process will send the data to all processes including itself
            
            getUserInfo();
            
            for (j = 0; j < size; j++) {
                
                int index = j*process_per_self;
                MPI_Send(&a[index], process_per_self, MPI_INT, j, 0, MPI_COMM_WORLD);
            }
        }
    
    
    //char* local_arr = (char*)malloc(sizeof(int)*process_per_self);
    
    int index = j*process_per_self;
    MPI_Recv(&a[index], process_per_self, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    
    int l_match = match(&a[index],&a[process_per_self]);
  
    if (l_match != -1) {
      printf("Found at location: %d\n", l_match + 1);
    }
    else {
      printf("Not found.\n");
    }
    
  
    MPI_Send(&l_match, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  
    
    if (rank == 0) {
       // int* min_arr = (int*)malloc(sizeof(int)*process_sayisi);
        for (j = 0; j < size; j++) {
            
            MPI_Recv(&l_match, 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        }
    
    }
    
}


int main(int argc, char **argv) {
    
   
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    parallelism();
   
    MPI_Finalize();
    return 0;
}


int match(char *a, char *b)
{
   int c;
   int position = 0;
   char *x, *y;
   
   x = a;
   y = b;
   
   while(*a)
   {
      while(*x==*y)
      {
         x++;
         y++;
         if(*x=='\0'||*y=='\0')
            break;
      }
      if(*y=='\0')
         break;
         
      a++;
      position++;
      x = a;
      y = b;
   }
   if(*a)
      return position;
   else
      return -1;
}
