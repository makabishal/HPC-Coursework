#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <pthread.h>

/***********************************************************************
*******
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2
uppercase
  letters and a 2 digit integer. Your personalised data set is included
in the
  code.

  Compile with:
    cc -o Thread-1e Thread-1e.c -lcrypt -pthread

  If you want to analyse the results then use the redirection operator
to send
  output to a file that you can view using an editor or the less
utility:

    ./Thread-1e > 1e-Thread_results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/
int n_passwords = 4;

char *encrypted_passwords[] = {

  "$6$KB$36REBSFzq1/dui1N6SgVpJNWbH7.H09nEmDF84hZhfeauI13ShDyaMT/d1tPMVh35NJKKkQrT6OIYDS9VLrIY0",
  "$6$KB$TShKJUQHJ/epFrqd8PVtnjB9lsjrRx6Qwv6fY/9ZOr0NHkH3sFrkc0IZlE8ricgOeQjFu56VSq9.BV2kNSpAg.",
  "$6$KB$ielcaeww2oJY5cJuYcgdrtKXuWecpbQLcbvNvI32cdGc1SW17x8csdrpeeUN/Pk8i/aXDLw1Kv4ON/obBDmcT1",
  "$6$KB$V51Vk5oaq9jlHAOGUICSX1DRt6gOTPS2N9rx8KI9UPCJtcpdSHV5rmY3c9/B5Ow1eQ62.B0x7.RIlljC9ujU71"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All
combinations
 that are tried are displayed and when the password is found, #, is put
at the
 start of the line. Note that one of the most time consuming operations
that
 it performs is the output of intermediate results, so performance
experiments
 for this kind of program should not include this. i.e. comment out the
printfs.
*/

void crack_bbm()
{
  int i;
pthread_t t1, t2;

    void *kernel_function_1();
    void *kernel_function_2();
for(i=0;i<n_passwords;i<i++) {
   
    
    pthread_create(&t1, NULL,kernel_function_1, encrypted_passwords[i]);
    pthread_create(&t2, NULL,kernel_function_2, encrypted_passwords[i]);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
 }
}

void *kernel_function_1(char *salt_and_encrypted){
  int d, e, p;     // Loop counters
  char salt[7];    // String used in hahttps://www.youtube.com/watch?v=L8yJjIGleMwshing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(d='A'; d<='Z'; d++){
    for(e='A'; e<='Z'; e++){
      for(p=0; p<=99; p++){
        sprintf(plain, "%c%c%02d", d,e,p);
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


void *kernel_function_2(char *salt_and_encrypted){
  int b, m, c;     // Loop counters
  char salt[7];    // String used in hahttps://www.youtube.com/watch?v=L8yJjIGleMwshing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(b='A'; b<='Z'; b++){
    for(m='A'; m<='Z'; m++){
      for(c=0; c<=99; c++){
        sprintf(plain, "%c%c%02d", b,m,c);
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

//Calculating time

int time_difference(struct timespec *start, struct timespec *finish, long long int *difference)
 {
	  long long int ds =  finish->tv_sec - start->tv_sec; 
	  long long int dn =  finish->tv_nsec - start->tv_nsec; 

	  if(dn < 0 ) {
	    ds--;
	    dn += 1000000000; 
  } 
	  *difference = ds * 1000000000 + dn;
	  return !(*difference > 0);
}
int main(int argc, char *argv[])
{
  	
	struct timespec start, finish;   
  	long long int time_elapsed;

  	clock_gettime(CLOCK_MONOTONIC, &start);

  	
	
    		crack_bbm();
  	
	clock_gettime(CLOCK_MONOTONIC, &finish);
	  time_difference(&start, &finish, &time_elapsed);
	  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
		                                 (time_elapsed/1.0e9)); 
  return 0;
}
