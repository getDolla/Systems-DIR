#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

char handle_prefix(long int size, long int * ret) {
    long int s = size;
    while (s >= 1000) {
        s /= 1000;
    }
    *ret = s;
    if (size < 1e6) return 'k';
    if (size < 1e9) return 'M';
    if (size < 1e12) return 'G';
    if (size < 1e15) return 'T';
    if (size < 1e18) return 'P';
    if (size < 1e21) return 'E';
    if (size < 1e24) return 'Z';
    if (size < 1e27) return 'Y';
    return '!';
}

void fmt_SI( long int size, char unit_char, char* buf ) {
    if (size < 1000) {
        sprintf(buf, "%li B\0", size);
    } else if (size < 1e27) {
        long int displayval;
        char prefix = handle_prefix(size, &displayval);
        sprintf(buf, "%li %c%c\0", displayval, prefix, unit_char);
    } else {
        sprintf(buf, "%li Y%c\0", size / (long int) 1e24, unit_char);
    }
}


void ListDirByType( char * s, unsigned char type ) {
  struct dirent *dInfo;
  DIR *dirStrm;
  
  dirStrm = opendir(s);
  if (dirStrm == NULL) {
     printf ("Cannot open: '%s'\n", s);
     return;
  }

  // . refers to directory itself and .. refers to the parent directory

  if( type == DT_DIR)
      printf( "Directories:\n" );
  else if( type == DT_REG )
      printf( "Regular Files:\n" );
  
  while ((dInfo = readdir(dirStrm)) != NULL)
      if( dInfo->d_type == type )
        printf ("\t%s\n", dInfo->d_name);
  
  closedir(dirStrm);
}

long int getTotalSize( char * s ) {
  struct dirent *dInfo;
  DIR *dirStrm;
  
  dirStrm = opendir(s);
  if (dirStrm == NULL) {
     printf ("Cannot open: '%s'\n", s);
     return;
  }
    
  struct stat filestat;
  long int sum = 0;
  char path[100]; //gets the begining of the path to the file
  strcpy( path, s );
  strcat( path, "/" );
  
  
    while ((dInfo = readdir(dirStrm)) != NULL) {
         char * temp = (char*) malloc( sizeof(path) ); // "temp" string
         strcpy( temp, path );
         strcat( temp, dInfo->d_name ); //gets filename
        
         if ( stat( temp, &filestat ) == 0 ) {
             sum += filestat.st_size;
             //printf( "%li\n", sum );
         }
        
         else {
             printf("File Error %d: %s\n", errno, strerror(errno));
         }
        
         free( temp );
    }
  
  return sum;
}

void main() {
  printf( "Folder: testFolder\n" );

  char p[256];
  long int total = getTotalSize( "testFolder" );  
  
  fmt_SI(total, 'B', p);
  printf("Total Size: %s\n", p);
  
  ListDirByType( "testFolder", DT_DIR );
  ListDirByType( "testFolder", DT_REG );
}
