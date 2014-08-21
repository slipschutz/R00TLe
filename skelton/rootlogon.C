rootlogon(){
  cout<<"Welcome to"<<endl;
  printf("\033[22;31m");
  printf("  _____   ___   ___ _______ _          \n");
  printf(" |  __ \\ / _ \\ / _ |__   __| |         \n");
  printf(" | |__) | | | | | | | | |  | |     ___ \n");
  printf(" |  _  /| |/| | |/| | | |  | |    / _ \\\n");
  printf(" | | \\ \\| |_| | |_| | | |  | |___|  __/\n");
  printf(" |_|  \\_\\\\___/ \\___/  |_|  |______\\___|\n");
  printf("\033[0m\n");
  printf("\033[22;34m");
  printf("                                    TM \n");
  printf(" Nothing works, everything is broken. \n");
  printf("\033[0m\n");

  gSystem->Load("libLendaEvent.so");
  gSystem->Load("libS800.so");
  gSystem->Load("libDDASEvent.so");
  gSystem->Load("libLendaPacker.so");

return;
}
