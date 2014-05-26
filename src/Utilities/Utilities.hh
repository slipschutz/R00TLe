/*
 *    Utilities.h
 *    Created       : 2012/08/30 17:57:13
 *    Last Modified : 2014/05/16 17:55:15
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2012 by Shumpei Noji
 */
#include <csignal>
#include <sys/time.h>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <TSystem.h>

extern bool signal_received;
void signalhandler(int sig);
double get_time();
void loadBar(Long64_t x, Long64_t n, Int_t resolution, Int_t width);
void Progress(const char* cname,const char* fmt, ...);

void Error(const char* cname,const char* fmt, ...);
void Info(const char* cname,const char* fmt, ...);
void Warning(const char* cname,const char* fmt, ...);
