




/**Plots trace from a ddaschannel object.  Scale will be subtracted
   from each trace point.
   This script is meant to be run from within the ROOT Interpreter
 */
void PlotTrace(ddaschannel *theChannel,int scale=0){
  
  int size = theChannel->GetTrace().size();
  //  cout<<"THE Size "<<size<<endl;
  int *x = (int*)malloc(size*sizeof(int));
  int *y = (int*)malloc(size*sizeof(int));
  // int x[size];
  // int y[size];
  // cout<<"THE SIZE IS "<<size<<endl;
  for (int i=0;i<size;i++){
    x[i]=i;
    y[i]=theChannel->GetTrace()[i]-scale;
  }
   
  int slot = theChannel->GetSlotID();
  int chan = theChannel->GetChannelID();

   
  TGraph * theGraph = new TGraph(size,x,y);
  
  
  stringstream ss;
  ss<<"slot "<<slot<<" channel "<<chan;
  //  cout<<ss.str()<<endl;
  theGraph->SetTitle(ss.str().c_str());
  
  theGraph->Draw("AL*");



}
/**Plots traces from an entry in the rawtree.  Entry is the 
   entry number in the tree.  Num specifies which channel in the event
   to plot (not the ddas channel number). If num=-1 will plot
   all the channels in the event.  Scale will be subtracted 
   from each trace point.
 */
void PlotTrace(TTree* theTree,long entry,int num=0,int scale=0){
  DDASEvent *e = new DDASEvent();
  
  theTree->SetBranchAddress("ddasevent",&e);
  theTree->GetEntry(entry);

  int size = e->GetData().size();
  cout<<"THERE are "<<size <<" traces in this event"<<endl;
  if (num >= size ){
    cout<<"There are only "<<size<<" trace in this event"<<endl;
    return;
  } else if (num == -1){
    //plot all the traces
    TCanvas * c1;
    TCanvas * c2;
    TCanvas * c3;
    TCanvas * c4;
    TCanvas * c5;
    TCanvas * c6;
    TCanvas * c7;

    if (size <= 4){
      c1 = new TCanvas("c1");
      c1->Divide(1,4);
    }else if (size >4 && size <=8){
      c1 = new TCanvas("c1");
      c1->Divide(1,4);
      c2 = new TCanvas("c2");
      c2->Divide(1,4);
    }else if (size >8 && size <=12){
      c1 = new TCanvas("c1");
      c1->Divide(1,4);
      c2 = new TCanvas("c2");
      c2->Divide(1,4);
      c3 = new TCanvas("c3");
      c3->Divide(1,4);
    }else if (size >12 && size <=16){
      c1 = new TCanvas("c1");
      c1->Divide(1,4);
      c2 = new TCanvas("c2");
      c2->Divide(1,4);
      c3 = new TCanvas("c3");
      c3->Divide(1,4);
      c4 = new TCanvas("c4");
      c4->Divide(1,4); 
    }else if (size >16 && size <=20){
      c1 = new TCanvas("c1");
      c1->Divide(1,4);
      c2 = new TCanvas("c2");
      c2->Divide(1,4);
      c3 = new TCanvas("c3");
      c3->Divide(1,4);
      c4 = new TCanvas("c4");
      c4->Divide(1,4); 
      c5 = new TCanvas("c5");
      c5->Divide(1,4); 
    }else if (size >20 && size <=24){
      c1 = new TCanvas("c1");
      c1->Divide(1,4);
      c2 = new TCanvas("c2");
      c2->Divide(1,4);
      c3 = new TCanvas("c3");
      c3->Divide(1,4);
      c4 = new TCanvas("c4");
      c4->Divide(1,4); 
      c5 = new TCanvas("c5");
      c5->Divide(1,4); 
      c6 = new TCanvas("c6");
      c6->Divide(1,4); 
    } else if (size >24 && size <=28){
      c1 = new TCanvas("c1");
      c1->Divide(1,4);
      c2 = new TCanvas("c2");
      c2->Divide(1,4);
      c3 = new TCanvas("c3");
      c3->Divide(1,4);
      c4 = new TCanvas("c4");
      c4->Divide(1,4); 
      c5 = new TCanvas("c5");
      c5->Divide(1,4); 
      c6 = new TCanvas("c6");
      c6->Divide(1,4); 
      c7 = new TCanvas("c7");
      c7->Divide(1,4); 
    }

    
    for (int i=0;i<size;i++){
      if (i <=3 ){
	c1->cd(i+1);
      }else if ( i > 3 && i <=7){
	c2->cd(i+1 - 4);
      }else if (i >7 && i<=11){
	c3->cd(i+1 -8);
      }else if (i>11 && i<=15){
	c4->cd(i+1 -12);
      }else if (i >15 && i<=19){
	c5->cd(i+1-16);
      }else if (i >19 && i<=23){
	c6->cd(i+1-20);
      }else if ( i>23 && i<=27){
	c7->cd(i+1-24);
      }else{
	cout<<"TOO MANY TRACES"<<endl;
      }
      

      ddaschannel *theChannel = e->GetData()[i];
      PlotTrace(theChannel,scale);
    }

  }else {

  ddaschannel *theChannel = e->GetData()[num];
  
  PlotTrace(theChannel,scale);
  }

}
