/*
Macro for Troubleshooting - counting muons/antimuons in the Track information for each event.

How to run:
Step 1: make
Step 2: ./trkCount_mu <filepath of .root file>

*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "TROOT.h"
#include "TSystem.h"
#include "TObject.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"

#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"

using namespace std;

int main(int argc, char* argv[])
{
    const char* inputfile;

    if(argc == 1)
    {   
        cout<<endl<<"Error: Enter path to .root file!\n"<<endl;
        return 1;
    }
    else if(argc > 2)
    {
        cout<<endl<<"Error: Too many arguments!\n"<<endl;
        return 1;
    }
    else
    {
        ifstream f(argv[1]);
        if(f.good()) inputfile = argv[1];
        else 
        {
            cout<<endl<<"Error: Given .root file does not exist!\n"<<endl;
            return 1;
        }
        f.close();
    }
    
	gSystem->Load("libDelphes");

    int mu0_event =0, mu1_event=0, mu2_event=0, mu3_event=0, mu4_event=0, mu5_event=0;
	int n_particles;
    Track* trk_item;

	TChain chain("Delphes");
    chain.Add(inputfile);

	ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
    Long64_t numberOfEntries = treeReader->GetEntries();

    TClonesArray *branchTrack = treeReader->UseBranch("Track");

    cout<<endl<<endl<<endl;
    cout<<"Counting mu+, mu- : n(Events) = "<<numberOfEntries<<endl;
    cout<<"-----------------------------------------------------"<<endl;
    
    for(Long64_t entry = 0; entry < numberOfEntries ; entry++)
    {
    	treeReader->ReadEntry(entry);
    	n_particles = branchTrack->GetEntries();

        int mu_counter=0, mu_plus_counter=0;

    	for(Long64_t count = 0; count < n_particles; count++)
    	{
    		trk_item = (Track*) branchTrack->At(count);

            if(trk_item->PID == 13) mu_counter++;
            if(trk_item->PID == -13) mu_plus_counter++;
    	}

        switch(mu_counter + mu_plus_counter)
        {
            case 0:
                mu0_event++;
                break;
            case 1:
                mu1_event++;
                break;
            case 2:
                mu2_event++;
                break;
            case 3:
                mu3_event++;
                break;
            case 4:
                mu4_event++;
                break;
            case 5:
                mu5_event++;
                break;
        }

    }
    // cout<<"0 muon events:\t"<<mu0_event<<endl;
    cout<<"\% of 0 muon events:  "<<double(mu0_event)/double(numberOfEntries)*100<<endl;
    // cout<<"1 muon events:\t"<<mu1_event<<endl;
    cout<<"\% of 1 muon events:  "<<double(mu1_event)/double(numberOfEntries)*100<<endl;
    // cout<<"2 muon events:\t"<<mu2_event<<endl;
    cout<<"\% of 2 muon events:  "<<double(mu2_event)/double(numberOfEntries)*100<<endl;
    // cout<<"3 muon events:\t"<<mu3_event<<endl;
    cout<<"\% of 3 muon events:  "<<double(mu3_event)/double(numberOfEntries)*100<<endl;
    // cout<<"4 muon events:\t"<<mu4_event<<endl;
    cout<<"\% of 4 muon events:  "<<double(mu4_event)/double(numberOfEntries)*100<<endl;
    // cout<<"5 muon events:\t"<<mu5_event<<endl;
    cout<<"\% of 5 muon events:  "<<double(mu5_event)/double(numberOfEntries)*100<<endl;
    cout<<endl;
    
    return 0;
}