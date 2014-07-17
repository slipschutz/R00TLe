rootlogon(){

  cout<<"Welcome to"<<endl;
  printf("  _____   ___   ___ _______ _          \n");
  printf(" |  __ \\ / _ \\ / _ |__   __| |         \n");
  printf(" | |__) | | | | | | | | |  | |     ___ \n");
  printf(" |  _  /| |/| | |/| | | |  | |    / _ \\\n");
  printf(" | | \\ \\| |_| | |_| | | |  | |___|  __/\n");
  printf(" |_|  \\_\\\\___/ \\___/  |_|  |______\\___|\n");
  printf("                                       \n");

  gSystem->Load("libLendaEvent.so");
  gSystem->Load("libS800.so");
  gSystem->Load("libDDASEvent.so");
  gSystem->Load("libR00TLeSettings.so");

return;
}
