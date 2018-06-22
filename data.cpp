// this file is distributed under 
// GPL license
#include <fstream>
#include <exception>
#include <Parameters/parameters.h>
#include "data.h"
#include "reconstruction.h"
using namespace std;
RealData::RealData(bool weak):m_trigger_weak(weak),
    BeamMomenta("Time.2.PBeam",[this](){return 1000.0*fHeader->GetTimeInCycle();},[](){return INFINITY;}){
	fHeader = dynamic_cast<REventHeader*>(gDataManager->GetDataObject("REventHeader","Header"));
}
RealData::~RealData(){}
bool RealData::DataTypeSpecificEventAnalysis()const{
	CachePBeam((BeamMomenta()/1000.0)+getParameter(pbeam_corr));
	return true;
}
bool RealData::DataSpecificTriggerCheck(int n)const{
        if(m_trigger_weak)return true;
	if(0==n)return true;
	return fHeader->TriggerNumSet(n);
}
