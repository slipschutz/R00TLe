


#include "InputManager.hh"
#include <vector>
#include <string>
#include <iomanip>

#include <sys/stat.h> //For file checking
//


bool InputManager::fileExists(const std::string& filename)
{
  struct stat buf;
  if (stat(filename.c_str(), &buf) != -1)
    {
      return true;
    }
  return false;
}

InputManager::InputManager(string name) : TheFileName(name),
					  SetUpSuccess(false),width(12){

  SetUpFiles();
  BuildInputMap();
  
}


InputManager::InputManager(){
  InputManager("MyInputFile.txt");
    
}

void InputManager::SetUpFiles(){

 if ( fileExists(TheFileName) ){
   InFile.open(TheFileName.c_str());
 } else {
   cout<<"Input File "<<TheFileName<<" Not Found"<<endl;
   cout<<"Make an Input file containing the valid options"<<endl;
   throw 1;
 }
}

InputManager::~InputManager()
{
  if (InFile.is_open()){ //Close input file
    InFile.close();
  }

  if (OutFile.is_open()){//Close output file
    OutFile.close();
  }

}

bool InputManager::ResolveBoolArgument(string s){

  string sc=lowerCase(s);
  
  if (sc =="yes" || sc == "1" || sc=="true"){
    return true;
  }else {
    return false;
  }
}



void InputManager::BuildInputMap(){
  
  if ( ! InFile.good() ){
    cout<<"Something terrible has happend"<<endl;
    throw 1; // something terrible has happened
  }
  
  ///////////////////////////////////////////////
  // format of the files should be 	       //
  // Type Identifier Flag default help message //
  ///////////////////////////////////////////////

  string type, flag,value,message; 

  while (true){
    if (InFile.eof()){
      break;
    }

    InFile>>type>>flag>>value;
    getline(InFile,message);
    
    if ( lowerCase(type) =="i"){//Integer Flag
      ValidIntegerInputs[lowerCase(flag)]=atoi(value.c_str());
    }else if (lowerCase(type)=="s"){ //String Flag
      ValidStringInputs[lowerCase(flag)]=value;
    }else if (lowerCase(type)=="b"){//bool Flag
      ValidBoolInputs[lowerCase(flag)]=ResolveBoolArgument(value);

    }else if (lowerCase(type)=="d"){//Double Flag
      ValidDoubleInputs[lowerCase(flag)]=double(atof(value.c_str()));
    }else {
      cout<<"Found "<<type<<" in file \""<<TheFileName<<"\" for type specifer"<<endl;
      cout<<"Only i (int), s (string), b (bool), d (double) are allowed"<<endl;
      throw 2;
    }
    //What ever is left on the line put into the help message
    CommentMap[lowerCase(flag)]=message;    
    

  }

  SetUpSuccess=true;
}

bool InputManager::LoadInputs(int argc,char** argv ){
  vector<string> inputs(argc);
  for (int i=0;i<argc;i++){
    inputs[i]=string(argv[i]);
  }

  return LoadInputs(inputs);
}

bool InputManager::LoadInputs(vector <string> & inputs){

  try {

    if (!SetUpSuccess){
      ///////////////////////////////////////////
      // Now Open up the File containing flags //
      ///////////////////////////////////////////

      SetUpFiles();
      
      ////////////////////////
      // Now Build the Maps //
      ////////////////////////
      BuildInputMap();
    }
    
    /////check to see if any argument was help
    /////if so print the options
    for (int i=0;i<(int)inputs.size();i++){
      if (lowerCase(inputs[i]) == "help"){
	PrintHelp();
	return false;
      }
    }
  
    

    
    vector <string> Flags;
    vector <string> Arguments;
    

    /////////////////////////////////////////////////////////
    // Parse the command line arguments for flag:value 	   //
    // structures and put them into vectors for processing //
    /////////////////////////////////////////////////////////
    vector <string> temp(2,"");
    for (int i =1;i<(int) inputs.size();++i){
      temp = split(inputs[i],':');
      if (temp.size() != 2 ){//input was not a:v
	cout<<"***Warning input \""<<inputs[i]<<"\" not recognized***"<<endl;
	throw 1;
	return false;
      } else{
	Flags.push_back(lowerCase(temp[0]));
	Arguments.push_back(lowerCase(temp[1]));
      }
    }

  
    
    for ( int i=0;i<(int)Flags.size();i++){
      if (ValidBoolInputs.find(Flags[i])!=ValidBoolInputs.end()){
	////////////Bool option found//////////////////
	ValidBoolInputs[Flags[i]]=ResolveBoolArgument(Arguments[i]);
      } else if ( ValidDoubleInputs.find(Flags[i]) != ValidDoubleInputs.end()){
	////////////////// Numerical option found//////////////////
	(ValidDoubleInputs[Flags[i]])=atof(Arguments[i].c_str());
      
      } else if ( ValidIntegerInputs.find(Flags[i]) != ValidIntegerInputs.end()){
	///////////////// Found Integer input//////////////////
	(ValidIntegerInputs[Flags[i]])=atoi(Arguments[i].c_str());

      } else if (ValidStringInputs.find(Flags[i]) != ValidStringInputs.end()){
	//a string input
	(ValidStringInputs[Flags[i]]) = Arguments[i];
      
      } else {
	cout<<"Unkown option: \""<<Flags[i]<<"\""<<endl;
	throw 1;
      }

      
    }

  } catch (...){
    cout<<"Fatal Error.  Program Exiting"<<endl;
    exit(1);
  }

  return checkValues();
}

string InputManager::lowerCase(string data){
  std::transform(data.begin(), data.end(), data.begin(), ::tolower);
  return data;
}

vector <string>  InputManager::split (const string &s, char delim, vector<string> &elems) {
  stringstream ss(s);
  string item;
  while(getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}


vector <string> InputManager::split(const string &s, char delim) {
  vector<string> elems;
  return split(s, delim, elems);
}







bool InputManager::checkValues(){
  return true;
}



void InputManager::PrintValues(){
  
  cout<<"Here are the current settings"<<endl;
  PrintLine('#',5*width);cout<<"\n";

  for (auto & ii : ValidDoubleInputs ){
    cout<<"Flag: "<<left<<setw(width)<<ii.first<<" Value: "<<setw(width)<<ii.second<<endl;
  }
  
  for (auto & ii : ValidIntegerInputs ){
    cout<<"Flag: "<<left<<setw(width)<<ii.first<<" Value: "<<setw(width)<<ii.second<<endl;
  }
   for (auto & ii : ValidBoolInputs ){
    cout<<"Flag: "<<left<<setw(width)<<ii.first<<" Value: "<<setw(width)<<ii.second<<endl;
  }
   for (auto & ii : ValidStringInputs ){
    cout<<"Flag: "<<left<<setw(width)<<ii.first<<" Value: "<<setw(width)<<ii.second<<endl;
  }
  

  PrintLine('#',5*width);cout<<"\n";  

}
void InputManager::PrintLine(char c,int width){
  cout<<"\n";
  for (int i=0;i<width;i++){
    cout<<c;
  }
  
}

void InputManager::PrintHelp(){


  cout<<setw(width)<<"\nWelcome to the Help Message\n"<<endl;
  PrintLine('#',10*width);cout<<"\n";

  for (auto & ii : ValidIntegerInputs ){
    cout<<"Flag: "<<left<<setw(width)<<ii.first<<" Default Value: "<<setw(width)<<ii.second<<" Description: ";
    if ( CommentMap.count(ii.first)!=0){
      cout<<CommentMap[ii.first]<<endl;
    }else {
      cout<<"Place Nice Description Here"<<endl;
    }
  }
  for (auto & ii : ValidDoubleInputs ){
    cout<<"Flag: "<<left<<setw(width)<<ii.first<<" Default Value: "<<setw(width)<<ii.second<<" Description: ";
    if ( CommentMap.count(ii.first)!=0){
      cout<<CommentMap[ii.first]<<endl;
    }else {
      cout<<"Place Nice Description Here"<<endl;
    }
  }
  for (auto & ii : ValidBoolInputs ){
    cout<<"Flag: "<<left<<setw(width)<<ii.first<<" Default Value: "<<setw(width)<<ii.second<<" Description: ";
    if ( CommentMap.count(ii.first)!=0){
      cout<<CommentMap[ii.first]<<endl;
    }else {
      cout<<"Place Nice Description Here"<<endl;
    }
  }
  for (auto & ii : ValidStringInputs ){
    cout<<"Flag: "<<left<<setw(width)<<ii.first<<" Default Value: "<<setw(width)<<ii.second<<" Description: ";
    if ( CommentMap.count(ii.first)!=0){
      cout<<CommentMap[ii.first]<<endl;
    }else {
      cout<<"Place Nice Description Here"<<endl;
    }
  }

  PrintLine('#',10*width);cout<<"\n";
}



double InputManager::GetDoubleValue(string flag){
  if (ValidDoubleInputs.count(flag)!=0){
    return ValidDoubleInputs[flag];
  } else {
    cout<<"Flag "<<flag<<" can not be found"<<endl;
    throw 1;
    return 0;
  }
}

int InputManager::GetIntValue(string flag){
  if (ValidIntegerInputs.count(flag)!=0){
    return ValidIntegerInputs[flag];
  }else{
    cout<<"Flag "<<flag<<" can not be found"<<endl;
    throw 1;
    return 0;
  }    
}

bool InputManager::GetBoolValue(string flag){
  if (ValidBoolInputs.count(flag)!=0){
    return ValidBoolInputs[flag];
  }else{
    cout<<"Flag "<<flag<<" can not be found"<<endl;
    throw 1;
    return 0;
  }    
}
string InputManager::GetStringValue(string flag){
  if (ValidStringInputs.count(flag)!=0){
    return ValidStringInputs[flag];
  }else{
    cout<<"Flag "<<flag<<" can not be found"<<endl;
    throw 1;
    return 0;
  }    
}
