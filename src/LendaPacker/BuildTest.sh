#!/bin/bash


# g++ PackerTest.C -o PackerTest -L../../lib -lLendaEvent -lDDASEvent -lLendaPacker -lLendaFilter -I../include/ `root-config --cflags` `root-config --libs` -Wl,-rpath,../../lib -std=c++11

# g++ PackerTest2.C -o PackerTest2 -L../../lib -lLendaEvent -lDDASEvent -lLendaPacker -lLendaFilter -I../include/ `root-config --cflags` `root-config --libs` -Wl,-rpath,../../lib -std=c++11

# g++ PackerNameTest.C -o PackerNameTest -L../../lib -lLendaEvent -lDDASEvent -lLendaPacker -lLendaFilter -I../include/ `root-config --cflags` `root-config --libs` -Wl,-rpath,../../lib -std=c++11

# g++ PackerTestMem.C -o PackerTestMem -L../../lib -lLendaEvent -lDDASEvent -lLendaPacker -lLendaFilter -I../include/ `root-config --cflags` `root-config --libs` -Wl,-rpath,../../lib -std=c++11

g++ PackerFileTest.C -o PackerFileTest -L../../lib -lR00TLeSettings -lLendaEvent -lDDASEvent -lLendaPacker -lLendaFilter -I../include/ `root-config --cflags` `root-config --libs` -Wl,-rpath,../../lib -std=c++11