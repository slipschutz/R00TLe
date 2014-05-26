/*
 *    Utilities.cc
 *    Created       : 2012/08/30 17:57:13
 *    Last Modified : 2014/05/21 17:24:16
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2012 by Shumpei Noji
 */
#include "Utilities.hh"

bool signal_received = false;

void signalhandler(int sig){
  if (sig == SIGINT){
    signal_received = true;
  }
}

double get_time(){
  struct timeval t;
  gettimeofday(&t, NULL);
  double d = t.tv_sec + (double) t.tv_usec/1000000;
  return d;
}

// http://www.rosshemsley.co.uk/2011/02/creating-a-progress-bar-in-c-or-any-other-console-app/
// Process has done i out of n rounds,
// and we want a bar of width w and resolution r.
void loadBar(Long64_t x, Long64_t n, Int_t resolution, Int_t width) {
  // Only update r times.
  if ( x % (n/resolution) != 0 ) return;
  // Calculuate the ratio of complete-to-incomplete.
  Float_t ratio = x/(Float_t)n;
  Int_t   c     = ratio * width;
  // Show the percentage complete.
  std::cout << std::setw(5) << std::setiosflags(std::ios::fixed) << std::setprecision(1) << (Float_t)(ratio*100.) << "% [";
  // Show the load bar.
  for (Int_t i = 0; i < c;       i++) std::cout << "=";
  for (Int_t i = c; i < width-1; i++) std::cout << " ";
  std::cout << "]";
  // dirty hack below
  if (x == n-1) {
    //    std::cout << std::endl << std::endl;
  } else {
    std::cout << "\r" << std::flush;
  }
}

void Progress(const char* cname,const char* fmt, ...)
{
   va_list argv;
   va_start(argv,fmt);
   char msg[1024];
   sprintf(msg,"Info in <%s> %s",cname,fmt);
   vprintf(msg,argv);
   fflush(stdout);
   va_end(argv);
}

void Error(const char* cname,const char* fmt, ...)
{
   va_list argv;
   va_start(argv,fmt);
   char msg[1024];
   sprintf(msg,"Error in <%s> %s",cname,fmt);
   vprintf(msg,argv);
   fflush(stdout);
   va_end(argv);
}

void Info(const char* cname,const char* fmt, ...)
{
   va_list argv;
   va_start(argv,fmt);
   char msg[1024];
   sprintf(msg,"Info in <%s> %s",cname,fmt);
   vprintf(msg,argv);
   fflush(stdout);
   va_end(argv);
}

void Warning(const char* cname,const char* fmt, ...)
{
   va_list argv;
   va_start(argv,fmt);
   char msg[1024];
   sprintf(msg,"Info in <%s> %s",cname,fmt);
   vprintf(msg,argv);
   fflush(stdout);
   va_end(argv);
}

