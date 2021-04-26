/*
Macro for Troubleshooting - pt, eta distributions of muons at the Track level for each event.

How to run:
Step 1: make
Step 2: ./trkmu_ptEta <filepath of .root file>

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
#include "TH1.h"
#include "TCanvas.h"

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

    int mu0_event=0, mu1_event=0, mu2_event=0;
    int mu3_event=0, mu4_event=0, mu5_event=0;
	int n_particles;
    Track* trk_item;

	TChain chain("Delphes");
    chain.Add(inputfile);

	ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
    Long64_t numberOfEntries = treeReader->GetEntries();

    TClonesArray *branchTrack = treeReader->UseBranch("Track");

    TH1 *h_mu_pt = new TH1F("muon_pT", "pT distribution - muons",
                            100, 0.0, 100.0);
    TH1 *h_mu_eta = new TH1F("muon_eta", "eta dsitribution - muon",
                            100, 0.0, 3.5);
    TH1 *h_mu_pt10 = new TH1F("muon_pT10", "pT dist. muons - pT < 10 GeV", 
                            10, 0.0, 10.0); 
    TH1 *h_mu1_pt = new TH1F("muon1_pt", "pt distribution of leading muon",
                            100, 0.0, 100.0);
    TH1 *h_mu2_pt = new TH1F("muon2_pt", "pt distribution of subleading muon",
                            100, 0.0, 100.0);

    cout<<endl<<endl<<endl;
    cout<<"> Generating Histograms - Track Level"<<endl;
    
    for(Long64_t entry = 0; entry < numberOfEntries ; entry++)
    {
    	treeReader->ReadEntry(entry);
    	n_particles = branchTrack->GetEntries();

        Double_t pt_lead=-1.0, pt_sublead=-1.0;
        int muon_counter = 0;

    	for(Long64_t count = 0; count < n_particles; count++)
    	{
    		trk_item = (Track*) branchTrack->At(count);

            if(trk_item->PID == 13)
            {
                muon_counter++;

                h_mu_pt->Fill(trk_item->PT);
                h_mu_eta->Fill(trk_item->Eta);
                if(trk_item->PT < 10.0)  h_mu_pt10->Fill(trk_item->PT);

                if(pt_lead < trk_item->PT)
                {
                    pt_sublead = pt_lead;
                    pt_lead = trk_item->PT;
                }

                if(trk_item->PT > pt_sublead && trk_item->PT < pt_lead)
                {
                    pt_sublead = trk_item->PT;
                }
            }
            
            if(trk_item->PID == -13)
            {
                muon_counter++;

                h_mu_pt->Fill(trk_item->PT);
                h_mu_eta->Fill(trk_item->Eta);
                if(trk_item->PT < 10.0)  h_mu_pt10->Fill(trk_item->PT);

                if(pt_lead < trk_item->PT)
                {
                    pt_sublead = pt_lead;
                    pt_lead = trk_item->PT;
                }
                
                if(trk_item->PT > pt_sublead && trk_item->PT < pt_lead)
                {
                    pt_sublead = trk_item->PT;
                }

            }
    	}

        if(muon_counter > 1)
        {
            h_mu1_pt->Fill(pt_lead);
            h_mu2_pt->Fill(pt_sublead);
        }
        else if(muon_counter == 1)
        {
            h_mu1_pt->Fill(pt_lead);
        }

    }
   
    cout<<"\tStoring pT Histogram."<<endl;
    auto pt_canvas = new TCanvas();
    h_mu_pt->Draw();
    pt_canvas->Print("muon_pt.pdf");

    cout<<"\tStoring eta Histogram."<<endl;  
    auto eta_canvas = new TCanvas();
    h_mu_eta->Draw();
    eta_canvas->Print("muon_eta.pdf");
    
    cout<<"\tStoring pT < 10 GeV Histogram."<<endl;
    auto pt10_canvas = new TCanvas();
    h_mu_pt10->Draw();
    pt10_canvas->Print("muon_pt10.pdf");
    
    cout<<"\tStoring leading muon pT Histogram."<<endl;
    auto pt1_canvas = new TCanvas();
    h_mu1_pt->Draw();
    pt1_canvas->Print("leadingmuon_pt.pdf");

    cout<<"\tStoring subleading muon pT Histogram."<<endl;
    auto pt2_canvas = new TCanvas();
    h_mu2_pt->Draw();
    pt2_canvas->Print("subleadingmuon_pt.pdf");

    cout<<"> Done!"<<endl;
    
    return 0;
}
