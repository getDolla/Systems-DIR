#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

void printDir( char * s ) {
  struct dirent *dInfo;
  DIR *dirStrm;
  
  dirStrm = opendir(s);
  if (dirStrm == NULL) {
     printf ("Cannot open: '%s'\n", s);
     return;
  }

  // . refers to directory itself and .. refers to the parent directory

  printf( "Files in %s:\n", s );
  while ((dInfo = readdir(dirStrm)) != NULL) {
    if( dInfo->d_type == DT_DIR)
      printf( "Directory:" );
    else if( dInfo->d_type == DT_REG )
      printf( "Regular File:" );
  
      printf ("\t%s\n", dInfo->d_name);
  }
  
  closedir(dirStrm);
}

void main() {

  printDir( "testFolder" );
}
