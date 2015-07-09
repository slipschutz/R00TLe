

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include <fstream>

#include "TObject.h"

using namespace std;

class InputManager : public TObject {

public:
  
  InputManager();
  InputManager(string);
  ~InputManager();
  
  bool LoadInputs(int,char ** );
  bool LoadInputs(vector <string> &);
  void PrintValues();

  double GetDoubleValue(string flag);
  int GetIntValue(string flag);
  bool GetBoolValue(string flag);
  string GetStringValue(string flag); 

private:
  vector <string> split (const string &s, char delim, vector<string> &elems);
  vector <string> split (const string &s, char delim);

  map <string ,double> ValidDoubleInputs; //Map to hold doulbe type inputs
  map <string ,int> ValidIntegerInputs; //Map to hold interger type inputs
  map <string, bool> ValidBoolInputs; //Map to hold bool type inputs
  map <string,string>ValidStringInputs; //Map to hold string type inputs
  map <string,string> CommentMap; //Map to hold help message information

  void BuildInputMap(); //Method to load file and build input maps
  string lowerCase(string);

  string TheFileName; //The name of the file that holds the flag information

  bool checkValues();

  ifstream InFile;
  ifstream OutFile;

  void SetUpFiles();

  bool fileExists(const string &);

  bool ResolveBoolArgument(string);



  void PrintHelp();
  void PrintLine(char c,int width);//Print Line of Char

  bool SetUpSuccess;
  int width;
  
  ClassDef(InputManager,1);
};
