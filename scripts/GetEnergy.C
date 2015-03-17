






#include <iostream>
using namespace std;
double GetEnergy(double TOF,double fp){

  double distInMeters=fp;

  double c = 299792458.0;

  cout<<TOF<<endl;
  double v = distInMeters/(TOF* 0.000000001);

  double beta = v/c;

  double gamma = 1/(sqrt(1-beta*beta));
  
  return (gamma-1)*939.565378;

}
