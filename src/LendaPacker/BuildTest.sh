#!/bin/bash


g++ PackerTest.C -o PackerTest -L../../lib -lLendaEvent -lDDASEvent -lLendaPacker -lLendaFilter -I../include/ `root-config --cflags` `root-config --libs` -Wl,-rpath,../../lib -std=c++11