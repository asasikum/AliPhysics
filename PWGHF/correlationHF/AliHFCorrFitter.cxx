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

/*____________________________________________________________
| Class for performing the fit of azimuthal correlations           |      
| Example of its usage in the macro PWGHF/correlationHF/FitPlots.C | 
|
|  Author: Somnath Kar (somnath.kar@cern.ch), 
|          Andrea Rossi (andrea.rossi@cern.ch)
|_____________________________________________________________*/


#include <TMath.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TF1.h>
#include "AliHFCorrelationUtils.h"
#include "AliHFCorrFitter.h"
#include <Riostream.h>
#include <TBufferFile.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TPaveText.h>
#include <TPaveLabel.h>
#include <TVirtualPad.h>
#include <TMath.h>
#include <TLatex.h>
#include <TColor.h>
#include <TClass.h>
#include <sstream>

//Correlation histogram should be normalised per bin width and No. of trigger----------

using std::cout;
using std::endl;
using std::stringstream;
ClassImp(AliHFCorrFitter)

//______________________________|Default Constructor|___________________________
AliHFCorrFitter::AliHFCorrFitter():
  fHist(0x0),
  fFit(0x0),
  fGausNS(0x0),
  fGausNS2(0x0),
  fGausAS(0x0),
  fGausAS2(0x0),
  fPed(0x0),
  fMin(0),
  fMax(0),
  fFixBase(0),
  fFixMean(0),
  fBaseline(-999.),
  fErrbaseline(-999.),
  fNsybc(0),
  fEnsybc(0),
  fAsybc(0),
  fEasybc(0),
  fMinBaselineRange(0),
  fMaxBaselineRange(-1),
  fTypeOfFitfunc(kTwoGausPeriodicity),
  fDmesonType(AliHFCorrelationUtils::kDaverage)
{
  //Default Constructor......... fix me

}

 //_____________________________|Standard Constructor|________________________________________
AliHFCorrFitter::AliHFCorrFitter(const TH1F* histoToFit, Double_t min, Double_t max):
fHist(0x0),
fFit(0x0),
fGausNS(0x0),
fGausNS2(0x0),
fGausAS(0x0),
fGausAS2(0x0),
fPed(0x0),
fMin(0),
fMax(0),
fFixBase(0),
fFixMean(0),
fBaseline(-999.),
fErrbaseline(-999.),
fNsybc(0),
fEnsybc(0),
fAsybc(0),
fEasybc(0),
fMinBaselineRange(0),
fMaxBaselineRange(-1),
fTypeOfFitfunc(kTwoGausPeriodicity),
fDmesonType(AliHFCorrelationUtils::kDaverage)
{
  fHist=(TH1F*)histoToFit->Clone("fHist");
  fMin=min; 
  fMax=max;
}

//___________________________|Copy Constructor|___________________
AliHFCorrFitter::AliHFCorrFitter(const AliHFCorrFitter &source):
  fHist(source.fHist),
  fFit(source.fFit),
  fGausNS(source.fGausNS),
  fGausNS2(source.fGausNS2),
  fGausAS(source.fGausAS),
  fGausAS2(source.fGausAS2),
  fPed(source.fPed),
  fMin(source.fMin),
  fMax(source.fMax),
  fFixBase(source.fFixBase),
  fFixMean(source.fFixMean),
  fBaseline(source.fBaseline),
  fErrbaseline(source.fErrbaseline),
  fNsybc(source.fNsybc),
  fEnsybc(source.fEnsybc),
  fAsybc(source.fAsybc),
  fEasybc(source.fEasybc),
  fMinBaselineRange(source.fMinBaselineRange),
  fMaxBaselineRange(source.fMaxBaselineRange),
  fTypeOfFitfunc(source.fTypeOfFitfunc),
  fDmesonType(source.fDmesonType)
{
  //copy constructor
}


//_________________________|Destructor|___________________
AliHFCorrFitter::~AliHFCorrFitter()
{
  Info("AliHFCorrFitter.cxx","Destructor is calling");

  if (fHist) {delete fHist; fHist = 0;}
  if (fFit) {delete fFit; fFit = 0;}
  if (fGausNS) {delete fGausNS; fGausNS = 0;}
  if (fGausNS2) {delete fGausNS2; fGausNS2 = 0;}
  if (fPed) {delete fPed; fPed = 0;}
  
  //destructor
}

//_______________________|Assignment Operator|____________________
AliHFCorrFitter& AliHFCorrFitter::operator=(const AliHFCorrFitter &cfit)
{
  if(&cfit == this) return *this;

  fHist=cfit.fHist;
  fFit=cfit.fFit;
  fGausNS=cfit.fGausNS;
  fGausNS2=cfit.fGausNS2;
  fGausAS=cfit.fGausAS;
  fGausAS2=cfit.fGausAS2;
  fPed=cfit.fPed;
  fMin=cfit.fMin;
  fMax=cfit.fMax;
  fFixBase=cfit.fFixBase;
  fFixMean=cfit.fFixMean;
  fBaseline=cfit.fBaseline;
  fErrbaseline=cfit.fErrbaseline;
  fNsybc=cfit.fNsybc;
  fEnsybc=cfit.fEnsybc;
  fAsybc=cfit.fAsybc;
  fEasybc=cfit.fEasybc;
  fMinBaselineRange=cfit.fMinBaselineRange;
  fMaxBaselineRange=cfit.fMaxBaselineRange;
  fTypeOfFitfunc=cfit.fTypeOfFitfunc;
  fDmesonType=cfit.fDmesonType;

  
  return *this;

}

//________________ |Class Functios Implementation|___________________________________

void AliHFCorrFitter::SetHisto(const TH1F *histoToFit){

  fHist = new TH1F(*histoToFit);
}



//_________________________|Setting functios to fit|___________________
void AliHFCorrFitter::SetFunction()
{

/*|____________________________________________________________________________________________________|
| ->fitFunc=1: const+ G NS + G AS (w/o periodicity)
|          =2: const+ G NS + G AS  (w/ periodicity)
|          =3: const+ yieldNS*[fact*(G NS)+(1- fact)*(G2 NS)] + yieldAS*(G AS)  (w/ periodicity)
|          =4: const +yieldNS*(G NS) + yieldAS*[fact*(G AS)+(1- fact)*(G2 AS)]   (w/ periodicity)
|          =5: v2 modulation (no gaussian terms)
|          =6: v2 modulation + G NS + G AS  (w/ periodicity)
|______________________________________________________________________________________________________|*/
  if(fFit){
    delete fFit;
    delete fGausNS;
    delete fGausNS2;
    delete fGausAS;
    delete fGausAS2;
    delete fPed;
  }
  switch(fTypeOfFitfunc){
  case 1:
    fFit=new TF1("ConstwoGaus","[0]+[1]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x-[2])*(x-[2])/2./([3]*[3]))+[4]/TMath::Sqrt(2.*TMath::Pi())/[6]*TMath::Exp(-(x-[5])*(x-[5])/2./([6]*[6]))",fMin,fMax);
    fGausNS=new TF1("fGausNS","[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-[1])*(x-[1])/2./([2]*[2]))",fMin,fMax);
    fGausAS=new TF1("fGausAS","[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-[1])*(x-[1])/2./([2]*[2]))",fMin,fMax);
    fPed=new TF1("fPed","[0]",fMin,fMax);  
    
    fFit->SetParLimits(0,0,9999.);
    fFit->SetParLimits(1,0,999.);
    fFit->SetParLimits(2,-1,1);
    fFit->SetParLimits(3,0,3.5);
    fFit->SetParLimits(4,0,999.);
    fFit->SetParLimits(5,2.,4);
    fFit->SetParLimits(6,0,3.5);
    //fFit->SetParLimits(2,2.14,4.14);
    
    fFit->SetParameter(0,3);
    fFit->SetParameter(1,2);
    fFit->SetParameter(2,0.);
    fFit->SetParameter(3,0.3);
    fFit->SetParameter(4,2);
    fFit->SetParameter(5,3.14);
    fFit->SetParameter(6,0.3);

    //fFit->SetParameters(1,0.1,0.,1,0.1,TMath::Pi(),0.3);   
    
   
    fFit->SetParName(0,"ped");
    fFit->SetParName(1,"NS Y");
    fFit->SetParName(2,"NS mean");
    fFit->SetParName(3,"NS #sigma");
    fFit->SetParName(4,"AS Y");
    fFit->SetParName(5,"AS mean");
    fFit->SetParName(6,"AS #sigma");
    break;
    
    /*case 2:
    fFit=new TF1("Gausperiodicfunc","[1]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x-[2])*(x-[2])/2./([3]*[3]))+[1]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x-2.*TMath::Pi()-[2])*(x-2.*TMath::Pi()-[2])/2./([3]*[3]))+[1]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x+2.*TMath::Pi()-[2])*(x+2.*TMath::Pi()-[2])/2./([3]*[3]))+[4]/TMath::Sqrt(2.*TMath::Pi())/[6]*TMath::Exp(-(x-[5])*(x-[5])/2./([6]*[6]))",fMin,fMax);
    
    fFit->SetParLimits(0,0,9999.);
    fFit->SetParLimits(1,0,999.);
    fFit->SetParLimits(2,-1,1);
    fFit->SetParLimits(4,0,999.);
    fFit->SetParLimits(5,0,999.);
    fFit->SetParLimits(6,0,3.5);
    //fFit->SetParLimits(2,2.14,4.14);

    fFit->SetParameter(0,3.);
    fFit->SetParameter(1,0.);  
    fFit->SetParameter(2,0.3);
    fFit->SetParameter(4,2);
    fFit->SetParameter(5,3.14);
    fFit->SetParameter(6,0.3);

    fFit->SetParName(0,"ped");
    fFit->SetParName(1,"NS Y");
    fFit->SetParName(2,"NS mean");
    fFit->SetParName(3,"NS #sigma");
    fFit->SetParName(4,"AS Y");
    fFit->SetParName(5,"AS mean");
    fFit->SetParName(6,"AS #sigma");
   
    break;
    */ 
  case 2:
    fFit=new TF1("TwoGausPeriodicity","[0]+[1]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x-[2])*(x-[2])/2./([3]*[3]))+[4]/TMath::Sqrt(2.*TMath::Pi())/[6]*TMath::Exp(-(x-[5])*(x-[5])/2./([6]*[6]))+[1]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x-2.*TMath::Pi()-[2])*(x-2.*TMath::Pi()-[2])/2./([3]*[3]))+[1]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x+2.*TMath::Pi()-[2])*(x+2.*TMath::Pi()-[2])/2./([3]*[3]))+[4]/TMath::Sqrt(2.*TMath::Pi())/[6]*TMath::Exp(-(x+2.*TMath::Pi()-[5])*(x+2.*TMath::Pi()-[5])/2./([6]*[6]))+[4]/TMath::Sqrt(2.*TMath::Pi())/[6]*TMath::Exp(-(x-2.*TMath::Pi()-[5])*(x-2.*TMath::Pi()-[5])/2./([6]*[6]))",fMin,fMax);
    fGausNS=new TF1("fGausNSper","[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-[1])*(x-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-2.*TMath::Pi()-[1])*(x-2.*TMath::Pi()-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x+2.*TMath::Pi()-[1])*(x+2.*TMath::Pi()-[1])/2./([2]*[2]))",fMin,fMax);
    fGausAS=new TF1("fGausASper","[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-[1])*(x-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-2.*TMath::Pi()-[1])*(x-2.*TMath::Pi()-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x+2.*TMath::Pi()-[1])*(x+2.*TMath::Pi()-[1])/2./([2]*[2]))",fMin,fMax);
    fPed=new TF1("fPed","[0]",fMin,fMax);  
    

    fFit->SetParLimits(0,0.,999.);
    fFit->SetParLimits(1,0,999.);
    fFit->SetParLimits(2,-0.55,0.55);
    fFit->SetParLimits(3,0,3.5); 
    fFit->SetParLimits(4,0,999.);
    fFit->SetParLimits(5,2.85,3.55);   
    fFit->SetParLimits(6,0,3.5);   

    fFit->SetParameter(0,0.6);
    fFit->SetParameter(1,3);
    fFit->SetParameter(2,0.);
    fFit->SetParameter(3,0.3);
    fFit->SetParameter(4,2);
    fFit->SetParameter(5,TMath::Pi());
    fFit->SetParameter(6,0.3);

    
    fFit->SetParName(0,"ped");
    fFit->SetParName(1,"NS Y");
    fFit->SetParName(2,"NS mean");
    fFit->SetParName(3,"NS #sigma");
    fFit->SetParName(4,"AS Y");
    fFit->SetParName(5,"AS mean");
    fFit->SetParName(6,"AS #sigma");
    break;
    
  case 3:
    fFit =new TF1("ConstThreeGausPeriodicity","[0]+[1]*([7]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x-[2])*(x-[2])/2./([3]*[3]))+[7]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x-2.*TMath::Pi()-[2])*(x-2.*TMath::Pi()-[2])/2./([3]*[3]))+[7]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x+2.*TMath::Pi()-[2])*(x+2.*TMath::Pi()-[2])/2./([3]*[3]))+(1.-[7])/TMath::Sqrt(2.*TMath::Pi())/[8]*TMath::Exp(-(x-[2])*(x-[2])/2./([8]*[8]))+(1.-[7])/TMath::Sqrt(2.*TMath::Pi())/[8]*TMath::Exp(-(x-2.*TMath::Pi()-[2])*(x-2.*TMath::Pi()-[2])/2./([8]*[8]))+(1.-[7])/TMath::Sqrt(2.*TMath::Pi())/[8]*TMath::Exp(-(x+2.*TMath::Pi()-[2])*(x+2.*TMath::Pi()-[2])/2./([8]*[8])))+[4]/TMath::Sqrt(2.*TMath::Pi())/[6]*TMath::Exp(-(x-[5])*(x-[5])/2./([6]*[6]))+[4]/TMath::Sqrt(2.*TMath::Pi())/[6]*TMath::Exp(-(x+2.*TMath::Pi()-[5])*(x+2.*TMath::Pi()-[5])/2./([6]*[6]))+[4]/TMath::Sqrt(2.*TMath::Pi())/[6]*TMath::Exp(-(x-2.*TMath::Pi()-[5])*(x-2.*TMath::Pi()-[5])/2./([6]*[6]))",fMin,fMax);
    fGausNS=new TF1("fGausNSper","[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-[1])*(x-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-2.*TMath::Pi()-[1])*(x-2.*TMath::Pi()-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x+2.*TMath::Pi()-[1])*(x+2.*TMath::Pi()-[1])/2./([2]*[2]))",fMin,fMax);
    fGausNS2=new TF1("fGausNS2per","[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-[1])*(x-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-2.*TMath::Pi()-[1])*(x-2.*TMath::Pi()-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x+2.*TMath::Pi()-[1])*(x+2.*TMath::Pi()-[1])/2./([2]*[2]))",fMin,fMax);
    fGausAS=new TF1("fGausASper","[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-[1])*(x-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-2.*TMath::Pi()-[1])*(x-2.*TMath::Pi()-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x+2.*TMath::Pi()-[1])*(x+2.*TMath::Pi()-[1])/2./([2]*[2]))",fMin,fMax);
    fPed=new TF1("fPed","[0]",fMin,fMax); 

    fFit->SetParLimits(0,0.,999.);
    fFit->SetParLimits(1,0.,999.);
    fFit->SetParLimits(2,-0.55,0.55);
    fFit->SetParLimits(3,0,3.5);
    fFit->SetParLimits(4,0,999.);
    fFit->SetParLimits(5,2.85,3.55);
    fFit->SetParLimits(6,0,3.5);
    fFit->SetParLimits(7,0.5,1.);// the range is set from 0.5 to 1 to guarantee that the first gaussian is the one with the highest yield
    fFit->SetParLimits(8,0,3.5);

    fFit->SetParameter(0,0.6);
    fFit->SetParameter(1,3); 
    fFit->SetParameter(2,0.);
    fFit->SetParameter(3,0.3);
    fFit->SetParameter(4,2);
    fFit->SetParameter(5,TMath::Pi());
    fFit->SetParameter(6,0.3);
    fFit->SetParameter(7,0.8);
    fFit->SetParameter(8,0.3);

    
    fFit->SetParName(0,"ped");
    fFit->SetParName(1,"NS Y");   
    fFit->SetParName(2,"NS mean 1g");
    fFit->SetParName(3,"NS #sigma 1g");
    fFit->SetParName(4,"AS Y");
    fFit->SetParName(5,"AS mean");
    fFit->SetParName(6,"AS #sigma");
    fFit->SetParName(7,"fract 1g");  
    fFit->SetParName(8,"NS #sigma 2g");
    break;

case 4:
    fFit =new TF1("ConstThreeGausPeriodicityAS","[0]+[1]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x-[2])*(x-[2])/2./([3]*[3]))+[1]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x+2.*TMath::Pi()-[2])*(x+2.*TMath::Pi()-[2])/2./([3]*[3]))+[1]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x-2.*TMath::Pi()-[2])*(x-2.*TMath::Pi()-[2])/2./([3]*[3]))+[4]*([7]/TMath::Sqrt(2.*TMath::Pi())/[6]*TMath::Exp((-(x-[5])*(x-[5])/2./([6]*[6]))+[7]/TMath::Sqrt(2.*TMath::Pi())/[6]*TMath::Exp(-(x-2.*TMath::Pi()-[5])*(x-2.*TMath::Pi()-[5]))/2./([6]*[6]))+[7]/TMath::Sqrt(2.*TMath::Pi())/[6]*TMath::Exp(-(x+2.*TMath::Pi()-[5])*(x+2.*TMath::Pi()-[5])/2./([6]*[6]))+(1.-[7])/TMath::Sqrt(2.*TMath::Pi())/[8]*TMath::Exp(-(x-[5])*(x-[5])/2./([8]*[8]))+(1.-[7])/TMath::Sqrt(2.*TMath::Pi())/[8]*TMath::Exp(-(x-2.*TMath::Pi()-[5])*(x-2.*TMath::Pi()-[5])/2./([8]*[8]))+(1.-[7])/TMath::Sqrt(2.*TMath::Pi())/[8]*TMath::Exp(-(x+2.*TMath::Pi()-[5])*(x+2.*TMath::Pi()-[5])/2./([8]*[8])))",fMin,fMax);

    fGausNS=new TF1("fGausNSper","[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-[1])*(x-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-2.*TMath::Pi()-[1])*(x-2.*TMath::Pi()-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x+2.*TMath::Pi()-[1])*(x+2.*TMath::Pi()-[1])/2./([2]*[2]))",fMin,fMax);   
    fGausAS=new TF1("fGausASper","[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-[1])*(x-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-2.*TMath::Pi()-[1])*(x-2.*TMath::Pi()-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x+2.*TMath::Pi()-[1])*(x+2.*TMath::Pi()-[1])/2./([2]*[2]))",fMin,fMax);
    fGausAS2=new TF1("fGausASper","[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-[1])*(x-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-2.*TMath::Pi()-[1])*(x-2.*TMath::Pi()-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x+2.*TMath::Pi()-[1])*(x+2.*TMath::Pi()-[1])/2./([2]*[2]))",fMin,fMax);
    fPed=new TF1("fPed","[0]",fMin,fMax); 


    fFit->SetParLimits(0,0.,999.);
    fFit->SetParLimits(1,0.,999.);
    fFit->SetParLimits(2,-0.55,0.55);
    fFit->SetParLimits(3,0,3.5);
    fFit->SetParLimits(4,0,999.);
    fFit->SetParLimits(5,2.85,3.55);
    fFit->SetParLimits(6,0,3.5);
    fFit->SetParLimits(7,0.5,1.); // the range is set from 0.5 to 1 to guarantee that the first gaussian is the one with the highest yield
    fFit->SetParLimits(8,0,3.5);

    fFit->SetParameter(0,0.6);
    fFit->SetParameter(1,3); 
    fFit->SetParameter(2,0.);
    fFit->SetParameter(3,0.3);
    fFit->SetParameter(4,2);
    fFit->SetParameter(5,TMath::Pi());
    fFit->SetParameter(6,0.3);
    fFit->SetParameter(7,0.8); 
    fFit->SetParameter(8,0.3);

    
    fFit->SetParName(0,"ped");
    fFit->SetParName(1,"NS Y");   
    fFit->SetParName(2,"NS mean 1g");
    fFit->SetParName(3,"NS #sigma 1g");
    fFit->SetParName(4,"AS Y");
    fFit->SetParName(5,"AS mean");
    fFit->SetParName(6,"AS #sigma");
    fFit->SetParName(7,"fract 1g");  
    fFit->SetParName(8,"AS #sigma 2g");
    break;
          
    case 5: // v2 modulation
      fFit=new TF1("v2Modulation","[0]*(1+2*[1]*[2](1+TMath::TMath::Cos(2*x)))",fMin,fMax);
      fPed=new TF1("fPedv2Mod","[0]*(1+2*[1]*[2](1+TMath::TMath::Cos(2*x))",fMin,fMax); 

      fFit->SetParLimits(0,0.,999.);
      fFit->SetParLimits(1,-1,1);
      fFit->SetParLimits(2,-1,1);          
      fFit->SetParName(0,"ped");
      fFit->SetParName(1,"v_{2} hadron");
      fFit->SetParName(2,"v_{2} D meson");        

      break;
      
      case 6: // case 2 + v2 modulation
	fFit=new TF1("TwoGausPeriodicityPlusV2modulation","[1]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x-[2])*(x-[2])/2./([3]*[3]))+[4]/TMath::Sqrt(2.*TMath::Pi())/[6]*TMath::Exp(-(x-[5])*(x-[5])/2./([6]*[6]))+[1]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x-2.*TMath::Pi()-[2])*(x-2.*TMath::Pi()-[2])/2./([3]*[3]))+[1]/TMath::Sqrt(2.*TMath::Pi())/[3]*TMath::Exp(-(x+2.*TMath::Pi()-[2])*(x+2.*TMath::Pi()-[2])/2./([3]*[3]))+[4]/TMath::Sqrt(2.*TMath::Pi())/[6]*TMath::Exp(-(x+2.*TMath::Pi()-[5])*(x+2.*TMath::Pi()-[5])/2./([6]*[6]))+[4]/TMath::Sqrt(2.*TMath::Pi())/[6]*TMath::Exp(-(x-2.*TMath::Pi()-[5])*(x-2.*TMath::Pi()-[5])/2./([6]*[6]))+[0]*(1+2*[7]*[8](1+TMath::TMath::Cos(2*x)))",fMin,fMax);
        
    fGausNS=new TF1("fGausNSper","[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-[1])*(x-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-2.*TMath::Pi()-[1])*(x-2.*TMath::Pi()-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x+2.*TMath::Pi()-[1])*(x+2.*TMath::Pi()-[1])/2./([2]*[2]))",fMin,fMax);
    fGausNS2=new TF1("fGausNS2per","[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-[1])*(x-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-2.*TMath::Pi()-[1])*(x-2.*TMath::Pi()-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x+2.*TMath::Pi()-[1])*(x+2.*TMath::Pi()-[1])/2./([2]*[2]))",fMin,fMax);
    fGausAS=new TF1("fGausASper","[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-[1])*(x-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x-2.*TMath::Pi()-[1])*(x-2.*TMath::Pi()-[1])/2./([2]*[2]))+[0]/TMath::Sqrt(2.*TMath::Pi())/[2]*TMath::Exp(-(x+2.*TMath::Pi()-[1])*(x+2.*TMath::Pi()-[1])/2./([2]*[2]))",fMin,fMax);
    fPed=new TF1("fPedv2Mod","[0]*(1+2*[1]*[2](1+TMath::TMath::Cos(2*x))",fMin,fMax); 
    
    fFit->SetParLimits(0,0.,999.);
    fFit->SetParLimits(1,0,999.);
    fFit->SetParLimits(2,-0.55,0.55);
    fFit->SetParLimits(3,0,3.5);
    fFit->SetParLimits(4,0,999.);
    fFit->SetParLimits(5,2.85,3.55);
    fFit->SetParLimits(6,0,3.5);
    fFit->SetParLimits(7,-1,1);
    fFit->SetParLimits(8,-1,1);
    
    fFit->SetParameter(0,0.6);
    fFit->SetParameter(1,3);
    fFit->SetParameter(2,0.);
    fFit->SetParameter(3,0.3);
    fFit->SetParameter(4,2);
    fFit->SetParameter(5,TMath::Pi());
    fFit->SetParameter(6,0.3);
    fFit->SetParameter(7,0);
    fFit->SetParameter(8,0);
    
    
    fFit->SetParName(0,"ped");
    fFit->SetParName(1,"NS Y");
    fFit->SetParName(2,"NS mean");
    fFit->SetParName(3,"NS #sigma");
    fFit->SetParName(4,"AS Y");
    fFit->SetParName(5,"AS mean");
    fFit->SetParName(6,"AS #sigma");
    fFit->SetParName(7,"v_{2} hadron");
    fFit->SetParName(8,"v_{2} D meson");
    break;
    
  }
  
}


//_______________________________________________________________________________

void AliHFCorrFitter::Fitting(Bool_t drawSplitTerm)
{
/*|________________________________________________________________________|
| -> fFixBase=0 : baseline free
|           =1 : fix the baseline to the minimum of the histogram
|           <0 : fix the baseline to the weighted average of the abs(fFixBaseline) lower points
|           =2 : zyam at pi/2. Fix the baseline averaging the 2 points around +-pi/2 value
|           =4 : zyam at pi/2. Fix the baseline averaging the 4 points around +-pi/2 value
|           =5 : pedestal fixed at avarage of the points in the transverse region |(pi/4 - pi2)
|           =6 : pedestal fixed to an external value (assumed to be already set before calling this method!!)
| -> fFixMean=0 : NS & AS mean free
|           =1 : NS mean fixed to 0, AS mean free
|           =2 : AS mean fixed to pi, NS mean free
|           =3 : NS mean fixed to 0, AS mean to pi
|___________________________________________________________________________|*/
    
  //_________________________________________ fFixBase 0
  if(fFixBase==6){
    fFit->FixParameter(0,fBaseline);
  }
    
  //_________________________________________ fFixBase 1
  if(fFixBase==1){
    Double_t min=1.e10;
    Int_t k=-1;
    for(Int_t j=1;j<=fHist->GetNbinsX();j++){
      if(fHist->GetBinContent(j)<min){
	min=fHist->GetBinContent(j);
	k=j;
      }
    }
    fFit->FixParameter(0,min);
    fBaseline=min;
    fErrbaseline=fHist->GetBinError(k);      
  }
    
  //_________________________________________ fFixBase <0
  if(fFixBase<0){
    Int_t npointsAv=TMath::Abs(fFixBase);
    Int_t *ind=new Int_t[fHist->GetNbinsX()];
    Float_t *hval=fHist->GetArray();
    Double_t errAv=0.,av=0.;
    TMath::Sort(fHist->GetNbinsX(),&hval[1],ind,kFALSE);// need to exclude under and over flow bins, KFALSE->increasing order
    // Average of abs(fFixBase) lower points
    for(Int_t k=0;k<npointsAv;k++){
      
      av+=(fHist->GetBinContent(ind[k]+1)/(fHist->GetBinError(ind[k]+1)*fHist->GetBinError(ind[k]+1)));
      //printf("havl: %f, hist :%f+-%f \n",hval[ind[k]+1],h->GetBinContent(ind[k]+1),h->GetBinError(ind[k]+1));
      errAv+=1./(fHist->GetBinError(ind[k]+1)*fHist->GetBinError(ind[k]+1));	  
    }
    av/=errAv;
    errAv=TMath::Sqrt(1./errAv);
    printf("Average fBaseline: %f +- %f \n",av,errAv);
    fFit->FixParameter(0,av);      
    fBaseline=av;
    fErrbaseline=errAv;
  }
    
  //_________________________________________ fFixBase 2
  if(fFixBase==2){// ZYAM, USE 2 POINTS AT +- pi/2
    Double_t errAv=0.,av=0.;
    // Average of abs(fFixBase) lower points
    Int_t binPhi=fHist->FindBin(TMath::Pi()/2.);
    av+=fHist->GetBinContent(binPhi)/(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));
    //	printf("havl: %f, hist :%f+-%f \n",hval[ind[k]+1],h->GetBinContent(ind[k]+1),h->GetBinError(ind[k]+1));
    errAv+=1./(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));	  

    binPhi=fHist->FindBin(-TMath::Pi()/2.);
    if(binPhi<1)binPhi=1;
    av+=fHist->GetBinContent(binPhi)/(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));
    //	printf("havl: %f, hist :%f+-%f \n",hval[ind[k]+1],h->GetBinContent(ind[k]+1),h->GetBinError(ind[k]+1));
    errAv+=1./(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));	
   
    av/=errAv;
    errAv=TMath::Sqrt(1./errAv);
    printf("Average fBaseline: %f +- %f \n",av,errAv);
    fFit->FixParameter(0,av);      
    fBaseline=av;
    fErrbaseline=errAv;
    
  }
  
    //_________________________________________ fFixBase 4
 if(fFixBase==4){// ZYAM, USE 4 points around +- pi/2
   Double_t errAv=0.,av=0.;
   // Average of abs(fFixBase) lower points
   Int_t binPhi=fHist->FindBin(TMath::Pi()/2.);
   av+=fHist->GetBinContent(binPhi)/(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));
   errAv+=1./(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));	  
    
   Double_t binCentreCloseL=fHist->GetBinCenter(binPhi-1);
   Double_t binCentreCloseR=fHist->GetBinCenter(binPhi+1);
   
   if(TMath::Abs(binCentreCloseR-TMath::Pi()/2.)<TMath::Abs(binCentreCloseL-TMath::Pi()/2.))binPhi++;
   else  binPhi--;
    av+=fHist->GetBinContent(binPhi)/(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));
    errAv+=1./(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));	  
    
    binPhi=fHist->FindBin(-TMath::Pi()/2.);
    if(binPhi<1)binPhi=fHist->GetNbinsX();
    av+=fHist->GetBinContent(binPhi)/(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));
    errAv+=1./(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));	  
    
    Int_t binphiL,binphiR;
    if(binPhi==1){
      binphiL=fHist->GetNbinsX();
      binCentreCloseL=fHist->GetBinCenter(fHist->GetNbinsX())-TMath::Pi()*2.;
    }
    else {
      binCentreCloseL=fHist->GetBinCenter(binPhi-1);
      binphiL=binPhi-1;
    }
    
    if(binPhi==fHist->GetNbinsX()){
      binphiR=1;
      binCentreCloseR=fHist->GetBinCenter(1);
    }
    else {
      binphiR=binPhi+1;
      binCentreCloseR=fHist->GetBinCenter(binPhi+1);
    }
    
    if(TMath::Abs(binCentreCloseR+TMath::Pi()/2.)<TMath::Abs(binCentreCloseL+TMath::Pi()/2.)){
      binPhi=binphiR;
    }
    else {
      binPhi=binphiL;      
    }
    av+=fHist->GetBinContent(binPhi)/(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));
    errAv+=1./(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));	  
    
    av/=errAv;
    errAv=TMath::Sqrt(1./errAv);
    printf("Average fBaseline: %f +- %f \n",av,errAv);
    fFit->FixParameter(0,av);      
    fBaseline=av;
    fErrbaseline=errAv;
 }
    
    //_________________________________________ fFixBase 5
 /*if(fFixBase==5){// use fit range
      Double_t errAv=0.,av=0.;
      
      
        for(Int_t binPhi =0; binPhi<fHist->GetNbinsX();binPhi++){
	  
	  if(fHist->GetBinLowEdge(binPhi)>=-0.5*TMath::Pi() && fHist->GetBinLowEdge(binPhi+1)<=-0.25*TMath::Pi()){
	      cout << "iBin = " << binPhi << endl;
	      av+=fHist->GetBinContent(binPhi)/(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));
	      errAv+=1./(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));
	  }
	  
	  if(fHist->GetBinLowEdge(binPhi)>=0.25*TMath::Pi() && fHist->GetBinLowEdge(binPhi+1)<=0.5*TMath::Pi()){
	    cout << "iBin = " << binPhi << endl;
            av+=fHist->GetBinContent(binPhi)/(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));
            errAv+=1./(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));
	  }
        }
        
        av/=errAv;
        errAv=TMath::Sqrt(1./errAv);
	//  av/=2;
	//  errAv/=2;
        printf("Average fBaseline: %f +- %f \n",av,errAv);
        fFit->FixParameter(0,av);
        fBaseline=av;
        fErrbaseline=errAv;
	
    }*/
    if(fFixBase==5){// use fit range
        Double_t errAv=0.,av=0.;
        
        
        for(Int_t binPhi =1; binPhi<=fHist->GetNbinsX();binPhi++){
            
            if(fHist->GetBinLowEdge(binPhi)>=-1.*fMaxBaselineRange && fHist->GetBinLowEdge(binPhi+1)<=-1.*fMinBaselineRange){
                cout << "iBin = " << binPhi << endl;
                av+=fHist->GetBinContent(binPhi)/(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));
                errAv+=1./(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));
            }
            
            if(fHist->GetBinLowEdge(binPhi)>=fMinBaselineRange && fHist->GetBinLowEdge(binPhi+1)<=fMaxBaselineRange){
                cout << "iBin = " << binPhi << endl;
                av+=fHist->GetBinContent(binPhi)/(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));
                errAv+=1./(fHist->GetBinError(binPhi)*fHist->GetBinError(binPhi));
            }
        }
        
        av/=errAv;
        errAv=TMath::Sqrt(1./errAv);
        //  av/=2;
        //  errAv/=2;
        printf("Average fBaseline: %f +- %f \n",av,errAv);
        fFit->FixParameter(0,av);
        fBaseline=av;
        fErrbaseline=errAv;
        
    }
  
    
 if(fFixMean==1||fFixMean==3){
   fFit->FixParameter(2,0.);
 }
 if(fFixMean==2||fFixMean==3){
   if(fTypeOfFitfunc!=0)fFit->FixParameter(5,TMath::Pi());
 }

 SetFunction();
 fHist->Fit(fFit,"REMI","",fMin,fMax);
 if(fFixBase==0){
   fBaseline=fFit->GetParameter(0);
   fErrbaseline=fFit->GetParError(0);
 }
 fHist->SetTitle(";#Delta#varphi (rad); #frac{1}{N_{D}}#frac{dN^{assoc}}{d#Delta#varphi} (rad^{-1}");
 SetSingleTermsForDrawing(drawSplitTerm);
}
 
//________________________________________________________________
void AliHFCorrFitter::YieldErrAboveBaseline()
{
  Double_t errAvns=0 ; Double_t errAvas=0;
  // Double_t base=fFit->GetParameter(0);
  Double_t base=fBaseline;
  Double_t ebase=fErrbaseline;
  //cout<<base<<"$$$$$$$$"<<endl;
  cout<<ebase<<"<---------"<<endl;

  Int_t binMinNS=fHist->FindBin(-1.5);// slightly more than -pi/2
  if(binMinNS<1)binMinNS=1;
  Int_t binMaxNS=fHist->FindBin(1.5);// slightly less than +pi/2
  
  Int_t binMinAS=fHist->FindBin(1.6);// slightly more than +pi/2
  cout<<binMinAS<<"<-----"<<endl;
  Int_t binMaxAS=fHist->FindBin(3.14+1.5);// slightly less than +3pi/2
  if(binMaxAS>fHist->GetNbinsX())binMaxNS=fHist->GetNbinsX();
  cout<<binMaxAS<<"<-----"<<endl;
  for(Int_t bmNS=binMinNS;bmNS<binMaxNS;bmNS++){
    fNsybc+=(fHist->GetBinContent(bmNS)-base)*fHist->GetBinWidth(bmNS);
    fEnsybc+=((fHist->GetBinError(bmNS)*fHist->GetBinError(bmNS))-(ebase*ebase))*fHist->GetBinWidth(bmNS);
    //fEnsybc+=TMath::Sqrt(fNsybc+(binMaxNS-binMinNS+1)*ebase*ebase)*fHist->GetBinWidth(bmNS);
    
  }
  errAvns=TMath::Sqrt(1./fEnsybc);

  for(Int_t bmAS=binMinAS;bmAS<binMaxAS;bmAS++){
    fAsybc+=(fHist->GetBinContent(bmAS)-base)*fHist->GetBinWidth(bmAS);
    fEasybc+=((fHist->GetBinError(bmAS)*fHist->GetBinError(bmAS))-(ebase*ebase))*fHist->GetBinWidth(bmAS);

    // fEasybc+=TMath::Sqrt(fAsybc+(binMaxAS-binMinAS+1)*ebase*ebase)*fHist->GetBinWidth(bmAS);

  }
  errAvas=TMath::Sqrt(1./fEasybc);

  printf("Bin counting results: NS y= %f |______| yerr=- %f \n AS y: %f |______| yerr=- %f \n",fNsybc,errAvns,fAsybc,errAvas);
  
}


//___________________________________________________________
void AliHFCorrFitter::SetSingleTermsForDrawing(Bool_t draw)
{
  Double_t *par=0;
  if(fTypeOfFitfunc==1||fTypeOfFitfunc==2){
    par=new Double_t[7];
  }
  else   if(fTypeOfFitfunc==3||fTypeOfFitfunc==4||fTypeOfFitfunc==6){
    par=new Double_t[9];
  }
  else if(fTypeOfFitfunc==5){
    par=new Double_t[3];
  }
  else {
    Printf("AliHFCorrFitter::SetSingleTermsForDrawing, wrong type of function");
    return;
  }
  
  fFit->GetParameters(par);

  fPed->SetParameter(0,par[0]);
  fPed->SetLineColor(6);//pink
  fPed->SetLineStyle(2);

  if(fTypeOfFitfunc!=5){   
    fGausNS->SetParameter(0,par[1]);
    fGausNS->SetParameter(1,par[2]);
    fGausNS->SetParameter(2,par[3]);
    fGausAS->SetParameter(0,par[4]);
    fGausAS->SetParameter(1,par[5]);
    fGausAS->SetParameter(2,par[6]);

    fGausNS->SetLineStyle(2);
    fGausNS->SetLineColor(kBlue);
    fGausAS->SetLineStyle(2);
    fGausAS->SetLineColor(kGreen);


  }
  else{
    fPed->SetParameter(1,par[1]);
    fPed->SetParameter(2,par[2]);
  }

  if(fTypeOfFitfunc==3){
    fGausNS->SetParameter(0,par[1]*par[7]);
    fGausNS2->SetParameter(0,par[1]*(1.-par[7]));
    fGausNS2->SetParameter(1,par[2]);
    fGausNS2->SetParameter(2,par[8]);
    fGausNS2->SetLineStyle(2);
    fGausNS2->SetLineColor(kCyan);
  
  }
  else if(fTypeOfFitfunc==4){
    fGausAS->SetParameter(0,par[4]*par[7]);
    fGausAS2->SetParameter(0,par[4]*(1.-par[7]));
    fGausAS2->SetParameter(1,par[5]);
    fGausAS2->SetParameter(2,par[8]);
  }
  else if(fTypeOfFitfunc==6){
    fPed->SetParameter(1,par[7]);
    fPed->SetParameter(2,par[8]);
  }

  if(draw){// it could be simplified, but in this way we draw in the proper order for better visualization
    fPed->Draw("same");
    if(fTypeOfFitfunc!=5){
      if(fTypeOfFitfunc==4){
	fGausAS2->Draw("same");
      }
      fGausAS->Draw("same");
      if(fTypeOfFitfunc==3){
	fGausNS2->Draw("same");
      }
      fGausNS->Draw("same");
    }
  }

}

//_______________________________________
TH1F* AliHFCorrFitter::SubtractBaseline()
{
  TH1F* hc=(TH1F*)fHist->Clone("hc");

  Double_t baseData=fFit->GetParameter(0);
  cout<<"baseline is = "<<baseData<<endl;
  for(Int_t j=1;j<=fHist->GetNbinsX();j++){
    cout<<"bincontent is "<<hc->GetBinContent(j)<<endl;
    hc->SetBinContent(j,fHist->GetBinContent(j)-baseData);
  }
  hc->Draw("same");
  return hc;
}
//__________________________________________
void AliHFCorrFitter::DrawLegendWithParameters(){

    TLatex *tlTitle= NULL;
    if(fDmesonType == AliHFCorrelationUtils::kDaverage) tlTitle= new TLatex(0.15,0.85,"D meson-charged particle azimuthal correlations");
    if(fDmesonType == AliHFCorrelationUtils::kDzero) tlTitle= new TLatex(0.15,0.85,"D^{0}-charged particle azimuthal correlations");
    if(fDmesonType == AliHFCorrelationUtils::kDplus) tlTitle= new TLatex(0.15,0.85,"D^{+}-charged particle azimuthal correlations");
    if(fDmesonType == AliHFCorrelationUtils::kDstar) tlTitle= new TLatex(0.15,0.85,"D^{*+}-charged particle azimuthal correlations");
    tlTitle->SetNDC();
    tlTitle->Draw();
    tlTitle->SetTextSize(0.025);
    
    Int_t nsy=fFit->GetParNumber("NS Y");
    Int_t asy=fFit->GetParNumber("AS Y");
    Int_t nss=fFit->GetParNumber("NS #sigma");
    Int_t ass=fFit->GetParNumber("AS #sigma");
    Int_t bas=fFit->GetParNumber("ped");
    TPaveText *pvStatTests1=new TPaveText(0.51,0.6,0.85,0.82,"NDC");
    pvStatTests1->SetFillStyle(0);
    pvStatTests1->SetBorderSize(0);
    TText* t1=pvStatTests1->AddText(0.,0.87,Form("#chi^{2}/ndf = %.1f/%d ",fFit->GetChisquare(),fFit->GetNDF()));
    TText* t2=pvStatTests1->AddText(0.,0.69,Form("NS Y = %.2f#pm%.2f ",fFit->GetParameter(nsy),fFit->GetParError(nsy)));
    TText* t3=pvStatTests1->AddText(0.,0.51,Form("NS #sigma = %.2f#pm%.2f ",fFit->GetParameter(nss),fFit->GetParError(nss)));
    TText* t4=pvStatTests1->AddText(0.,0.33,Form("AS Y = %.2f#pm%.2f ",fFit->GetParameter(asy),fFit->GetParError(asy)));
    TText* t5=pvStatTests1->AddText(0.,0.15,Form("AS #sigma = %.2f#pm%.2f ",fFit->GetParameter(ass),fFit->GetParError(ass)));
    t1->SetTextSize(0.02);
    t2->SetTextSize(0.02);
    t3->SetTextSize(0.02);
    t4->SetTextSize(0.02);
    t5->SetTextSize(0.02);
    if(fBaseline<-998.){
      TText* t6=pvStatTests1->AddText(0.,0.,Form("baseline = %.2f#pm%.2f ",fFit->GetParameter(bas),fFit->GetParError(bas)));
      t6->SetTextSize(0.02);
      
    }
    else
      {
	TText* t6=pvStatTests1->AddText(0.,0.,Form("baseline = %.2f#pm%.2f ",fBaseline,fErrbaseline));
	t6->SetTextSize(0.02);
	
      }
    pvStatTests1->SetTextColor(kBlack);
    pvStatTests1->SetTextFont(42);
    //  pvStatTests1->SetNDC();
    pvStatTests1->SetTextSize(0.05);
    
    pvStatTests1->Draw("same");
}

