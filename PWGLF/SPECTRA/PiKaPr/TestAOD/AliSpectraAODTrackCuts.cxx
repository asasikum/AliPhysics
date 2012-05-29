
/**************************************************************************
 * Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

//-----------------------------------------------------------------
//         AliSpectraAODTrackCuts class
//-----------------------------------------------------------------

#include "TChain.h"
#include "TTree.h"
#include "TLegend.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAODTrack.h"
#include "AliAODMCParticle.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliAnalysisTaskESDfilter.h"
#include "AliAnalysisDataContainer.h"
#include "AliSpectraAODTrackCuts.h"
#include "AliSpectraAODHistoManager.h"
#include <iostream>

using namespace std;

ClassImp(AliSpectraAODTrackCuts)


AliSpectraAODTrackCuts::AliSpectraAODTrackCuts(const char *name) : TNamed(name, "AOD Track Cuts"), fIsSelected(0), fTrackBits(0), fEtaCut(0), fDCACut(0), fPCut(0), fPtCut(0), fYCut(0),
  fPtCutTOFMatching(0),fQvecCutMin(0),fQvecCutMax(0), fHistoCuts(0), fTrack(0)
  
{
  // Constructor
  fHistoCuts = new TH1I("fTrkCuts", "Track Cuts", kNTrkCuts, -0.5, kNTrkCuts - 0.5);
  for(Int_t ibin=1;ibin<=kNTrkCuts;ibin++)fHistoCuts->GetXaxis()->SetBinLabel(ibin,kBinLabel[ibin-1]);
  fEtaCut = 100000.0; // default value of eta cut ~ no cut
  fDCACut = 100000.0; // default value of dca cut ~ no cut
  fPCut = 100000.0; // default value of p cut ~ no cut
  fPtCut = 100000.0; // default value of pt cut ~ no cut 
  fPtCutTOFMatching=0.6; //default value fot matching with TOF
  fQvecCutMin = -100000.0; // default value of qvec cut ~ no cut 
  fQvecCutMax = 100000.0; // default value of qvec cut ~ no cut 
  fYCut       = 100000.0; // default value of y cut ~ no cut 
  
}

//_______________________________________________________
Bool_t AliSpectraAODTrackCuts::IsSelected(AliAODTrack * track)
{
  // Returns true if Track Cuts are selected and applied
  if (!track)
    {
      printf("ERROR: Could not receive track");
      return kFALSE;
    }
  fTrack = track;
  
  if(!CheckTrackType()){
    return kFALSE;
  }
  fHistoCuts->Fill(kTrkBit);
  if(!CheckEtaCut()){
    return kFALSE;
  }
  fHistoCuts->Fill(kTrkEta);
  if(!CheckDCACut()){
    return kFALSE;
  }
  fHistoCuts->Fill(kTrkDCA);
  if(!CheckPCut()){
    return kFALSE;
  }
  fHistoCuts->Fill(kTrkP);
  if(!CheckPtCut()){
    return kFALSE;
  }
  fHistoCuts->Fill(kTrkPt);
  if(!CheckTOFMatching()){
    return kFALSE;
  }
  fHistoCuts->Fill(kAccepted);
  Printf("-------- %d,%d",kTOFMatching,kAccepted);
  return kTRUE;
}
//_________________________________________________________

Bool_t AliSpectraAODTrackCuts::CheckTrackType()
{
  // Check track cuts
  if (fTrack->TestFilterBit(fTrackBits)) return kTRUE;
  return kFALSE;
}
//________________________________________________________
Bool_t AliSpectraAODTrackCuts::CheckEtaCut()
{
   // Check eta cut
   if (fTrack->Eta() < fEtaCut && fTrack->Eta() > - fEtaCut) return kTRUE;
    return kFALSE;
}

Bool_t AliSpectraAODTrackCuts::CheckYCut(AODParticleSpecies_t species) 
{
  // check if the rapidity is within the set range
  Double_t y;
  if (species == kSpProton) { y = fTrack->Y(9.38271999999999995e-01); }
  if ( species == kSpKaon ) { y = fTrack->Y(4.93676999999999977e-01); }
  if ( species == kSpPion)  { y = fTrack->Y(1.39570000000000000e-01); }
  if (TMath::Abs(y) > fYCut || y < -998.) return kFALSE;
  return kTRUE;
}
//_______________________________________________________
Bool_t AliSpectraAODTrackCuts::CheckDCACut()
{
   // Check DCA cut
  if (TMath::Abs(fTrack->DCA()) < fDCACut) return kTRUE;
   return kFALSE;
}
//________________________________________________________
Bool_t AliSpectraAODTrackCuts::CheckPCut()
{
   // Check P cut
   if (fTrack->P() < fPCut) return kTRUE;
   return kFALSE;
}
//_______________________________________________________
Bool_t AliSpectraAODTrackCuts::CheckPtCut()
{
    // check Pt cut
//    if ((fTrack->Pt() < fPtCut) && (fTrack->Pt() > 0.3 )) return kTRUE;
   if (fTrack->Pt() < fPtCut) return kTRUE;
    return kFALSE;
}

//_______________________________________________________
Bool_t AliSpectraAODTrackCuts::CheckTOFMatching()
{
  // check Pt cut
  //    if ((fTrack->Pt() < fPtCut) && (fTrack->Pt() > 0.3 )) return kTRUE;
  
  if (fTrack->Pt() < fPtCutTOFMatching) return kTRUE;
  else{
    fHistoCuts->Fill(kTrkPtTOF);
    UInt_t status; 
    status=fTrack->GetStatus();
    if((status&AliAODTrack::kTOFout))fHistoCuts->Fill(kTrTOFout);
    if((status&AliAODTrack::kTIME))fHistoCuts->Fill(kTrTIME);
    if((status&AliAODTrack::kTOFpid))fHistoCuts->Fill(kTrTOFpid);
    
    if((status&AliAODTrack::kTOFout)==0 || (status&AliAODTrack::kTIME)==0 || (status&AliAODTrack::kTOFpid)==0){
    //if((status&AliAODTrack::kTOFpid)==0){
      return kFALSE; //tof matching and PID
    } 
    fHistoCuts->Fill(kTOFMatching);
    return kTRUE;
  }
}
//_______________________________________________________
void AliSpectraAODTrackCuts::PrintCuts() const
{
  // Print cuts
    cout << "Track Cuts" << endl;
    cout << " > TrackBit\t" << fTrackBits << endl;
    cout << " > Eta cut\t" << fEtaCut << endl;
    cout << " > DCA cut\t" << fDCACut << endl;
    cout << " > P cut\t" << fPCut << endl;
    cout << " > Pt cut \t" << fPtCut << endl;
    cout << " > Q vactor Min \t" << fQvecCutMin << endl;
    cout << " > Q vactor Max \t" << fQvecCutMax << endl;
}
//_______________________________________________________
void AliSpectraAODTrackCuts::SetTrackType(UInt_t bit)
{
   // Set the type of track to be used. The argument should be the bit number. The mask is produced automatically.
   fTrackBits = (0x1 << (bit - 1));
}
//_______________________________________________________

Long64_t AliSpectraAODTrackCuts::Merge(TCollection* list)
{
  // Merge a list of AliSpectraAODTrackCuts objects with this.
  // Returns the number of merged objects (including this).

  //  AliInfo("Merging");

  if (!list)
    return 0;

  if (list->IsEmpty())
    return 1;

  TIterator* iter = list->MakeIterator();
  TObject* obj;

  // collections of all histograms
  TList collections;

  Int_t count = 0;

  while ((obj = iter->Next())) {
    AliSpectraAODTrackCuts* entry = dynamic_cast<AliSpectraAODTrackCuts*> (obj);
    if (entry == 0) 
      continue;

    TH1I * histo = entry->GetHistoCuts();      
    collections.Add(histo);
    count++;
  }
  
  fHistoCuts->Merge(&collections);
  
  delete iter;

  return count+1;
}

