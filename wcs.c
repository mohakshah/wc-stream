/* 
 * http://www.arnowagner.info/tools/index.html
 * "wc-stream": wc with incremental output every second or so. 
 * Line positioning is done like in dd_rescue with "optimistic 
 * positioning" i.e. the hope that we have a terminal that 
 * understands vt100 positioning.
 *
 * (c) Arno Wagner <arno@wagner.name> 2012, 2014. Distributed under
 * The GNU public license v2 or later at your choice.
 *
 * Version 1.2.3
 *
 * Compile with "cc -O3 -o wcs wcs.c"
 *
 * Revision history:
 * 1.2.3  Fixed bug that displays negative speed on very short
 *        transfers. I think this may be caused by slightly
 *        deviating per-core clocks.
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

const char* up = "\x1b[A"; //] 
const char* down = "\n";
const char* right = "\x1b[C"; //]

char * usage() {
  return("\n"
"  Prints out running count and rate statistics about the data\n"
"  read on stdin to stderr. Does use cursor positioning, which\n" 
"  should work on most terminals (vt100 or later).\n"
"\n"
"  stdin is copied through to stdout, much like in tee.\n"
" \n" 
"  This programm does not support any commandline arguments.\n"  
"\n"
"  (c) Arno Wagner <arno@wagner.name> 2012, 2014. \n"
"  Distributed under the GNU public license v2 or later at your choice.\n"
"  Version 1.2.3\n" 
"\n\n");
}




void printlong(long long l) {
      if (l < 1000000L) 
        fprintf(stderr, "%7.3f kB", l/1000.0);
      else if (l < 1000000000) 
        fprintf(stderr, "%7.3f MB", l/1000000.0);
      else if (l < 1000000000000LL) 
        fprintf(stderr, "%7.3f GB", l/1000000000.0);
      else 
        fprintf(stderr, "%7.3f TB", l/1000000000000.0);
}  



int main(int argc, char ** argv) {
  char buf[BUFSIZ];
  long long cnt = 0;
  time_t to, tn, ts, dt;
  int read_count;
  double rate;

  if (argc > 1) {
    fprintf(stderr, "%s", usage());
    exit(1);
  }

  to = time(NULL);
  ts = to;
  fprintf(stderr, "%s", down);

  while (1) {
    read_count = read (0, buf, sizeof(buf));
    if (read_count < 0 && errno == EINTR)  continue; // not an error
    if (read_count < 0) {
      perror("Abnormal condition in read from stdin:");
      exit(1);
    }

    if (read_count > 0) {
      cnt += read_count;
      if (fwrite (buf, 1, read_count, stdout) != read_count) {
        perror("Error in wcs writing to stdout:");  
        exit(1);
      }
    }

    tn = time(NULL);
    if (tn != to || read_count == 0) {
      to = tn;
      fprintf(stderr, "%s read: ", up);
      printlong(cnt);
      fprintf(stderr, " [ %12lld B]    avg: ", cnt);
        
      // Calculate the rate
      dt = tn - ts;
      if (dt <= 0) dt = 1; // sometimes the difference is even negative
                           // possibly slightly different per-core times
      rate = cnt / (double)dt;
      printlong((long long) rate);
      fprintf(stderr, "/sec [ %5d sec]%s", (int)dt, down);
    }
    if (read_count == 0)  break;                     // we are done 
  }
  fprintf(stderr, "\n");
  return(0);
}
    
