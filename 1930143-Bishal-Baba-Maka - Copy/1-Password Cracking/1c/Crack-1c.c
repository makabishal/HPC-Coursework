#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>

/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code. 

  Compile with:
    cc -o Crack-1c Crack-1c.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./Crack-1c > 1c-results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$SBwmEVvdlRwwjM30ipUQUNGjTb2zq34CeCk4WfsfLi5Jjj.P5d2oUDMe1qNiy/099nbor7NpnFNygY0ukmJmF0",
  "$6$KB$owCbMrAHgYYHcZOJ7YqS6C14KoTNOVoxMml7PYCAyHFW6wkTiPSLta3/A1JY455BzslsWlgzORTlPnq6/BIeA.",
  "$6$KB$D19OAN3XSfvODh4G8a4DAeBp.4/ajQ/Ch4VdZWs3uWTzhsL296tJu7NxkupixxPObtVc6fQqojIkkIapRtKSy.",
  "$6$KB$r9xIoaj3bEoJnEY9O3ku6tKrIb2fISbbU7rgxAG8QVqmtSUMXOPiqdL2W8Nae3YkY4AMjBJAKrXFvcrmzdWJo/"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All combinations
 that are tried are displayed and when the password is found, #, is put at the 
 start of the line. Note that one of the most time consuming operations that 
 it performs is the output of intermediate results, so performance experiments 
 for this kind of program should not include this. i.e. comment out the printfs.
*/

void crack(char *salt_and_encrypted){
  int x, y, s, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='A'; x<='Z'; x++){
    for(y='A'; y<='Z'; y++){
	for(s='A'; s<='Z'; s++){
     	 for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%c%02d", x, y, s, z); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
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
int main(int argc, char *argv[]){
  int i;
struct timespec start, finish;   
  long long int difference;
  int account =0;
  clock_gettime(CLOCK_MONOTONIC, &start);

  
  for(i=0;i<n_passwords;i<i++) {
    crack(encrypted_passwords[i]);
  }
printf("	solutions explored\n");

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &difference);

  printf("run lasted %9.5lfs\n", difference/1000000000.0); 

  return 0;
}
