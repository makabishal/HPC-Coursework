#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <mpi.h>

/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code.

  Compile with:
    mpicc -o 2int4digit 2int4digit.c -lrt -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    mpirun -n 3 ./2int4digit

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *passwords_enc[] = {
  "$6$KB$YJ2cyUdgpCT/xIjdOjTkhYBZ9ZMGhECA6s0Oa2qcqet4QG99J1KOp1kU8KM96NAv.MUefq7znOGoBPT/R/Qof1",
  "$6$KB$4lgeg0fP8KjghGn1c6KT9TiwoNLstKsYS8ROrZ5ChNMcojFJME4pcYiviMqc1mhdSwm/6i/CjN04FPSk8sE2y1",
  "$6$KB$YqPLh2YpN/aZgzNeCkgeUCiPnRS05h4sEJ/UAjD1kd7fR7Q8c6CfBoQ.xe2LxAY4fEzQ4r5bl9D0nxTCLQV/K1",
  "$6$KB$PuJYCy0SSPzl./BcdrXWaGA1gbbOBgASj91ECNz056ocfXDAoUsmAdTKgeMSGourMmgPkAwKJzlYciIJQGMk90"
};

/**
 Required by lack of standard function in C.  
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

void function_1(char *salt_and_encrypted){
  int a,b,c;    
  char salt[7];
  char plain[7];
  char *enc;
  int count = 0;

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='A';a<='M'; a++){
    for(b='A'; b<='Z'; b++){
     
	for(c=0; c<=9999; c++){

//printf("Instance 1:");
        sprintf(plain, "%c%c%04d", a,b,c);
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        }

      }
    }
  }


  printf("%d solutions explored\n", count);
}

void function_2(char *salt_and_encrypted){
  int a, b, c;
  char salt[7];
  char plain[7];
  char *enc;
  int count = 0;

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='N';a<='Z'; a++){
    for(b='A'; b<='Z'; b++){
      for(c=0; c<=9999; c++){

//printf("Instance 1:");
        sprintf(plain, "%c%c%04d",a,b,c);
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        }

      }
    }
  }

  printf("%d solutions explored\n", count);
}

int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec;
  long long int dn =  finish->tv_nsec - start->tv_nsec;

  if(dn < 0 ) {
    ds--;
    dn += 1000000000;
  }
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main() {

  struct timespec start, finish;  
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);

  int size, rank;
  int i;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 3){
    if (rank ==0){
printf("this program needs to run on exactly 3 process to run\n ");
}
}else{
 if(rank == 0){

int r;
MPI_Send(&r, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
MPI_Send(&r, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);

}else if(rank==1){
 int num;
 MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  for(i=0; i<n_passwords; i<i++){
    function_1(passwords_enc[i]);
   }
  } else{
    int num;
    MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
for(i=0; i<n_passwords; i<i++){
          function_2(passwords_enc[i]);
   }
  }
}
 MPI_Finalize();
  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
 



if (rank ==0){

printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
         (time_elapsed/1.0e9));

}
  return 0;
}
