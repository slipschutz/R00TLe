{
  // what is essential is the following line:
  gDirectory->DeleteAll();

  // also want to delete the canvas
  delete (TCanvas*)gROOT->GetListOfCanvases()->FindObject("c1"); // `delete c1' should simply work...

  // issue a kind message telling you if the so file is loaded
  TString srcdir = "/user/e10003/R00TLe/users/skel/src";
  TString sofile = srcdir + "/" + "Analoop_cc.so";
  switch (gSystem->Load(sofile)) {
  case 0:
    std::cout << sofile << " was not loaded." << std::endl;
    gSystem->Unload(sofile);
    break;
  case 1:
    std::cout << sofile << " was loaded, and now being unloaded." << std::endl;
    gSystem->Unload(sofile);
    break;
  case -1:
    std::cout << sofile << "does not exist." << std::endl;
    gSystem->Unload(sofile);
    break;
  case -2:
    std::cout << "Version mismatch in " << sofile << std::endl;
    gSystem->Unload(sofile);
    break
  default:
    break;
  }
}
