// this file is distributed under 
// MIT license
#include <functional>
#include <math_h/error.h>
#include "analysismodule.hh"
#include "config.h"
#include "reactions.h"
using namespace std;
using namespace MathTemplates;
string type;
void SetAnalysisType(std::string t){
	type=t;
}

ClassImp(AnalysisModule);
AnalysisModule::AnalysisModule(){}
AnalysisModule::AnalysisModule(const char* name):CAnalysisModule(name){
	using namespace ReactionSetup;
	m_data=nullptr;
	if("RE_He3eta"==type)
		m_data= He3_X_reconstruction(forEta);
	if(
		("RE_He3pi0"==type)||
		("RE_He3pi0pi0"==type)||
		("RE_He3pi0pi0pi0"==type)
	)
		m_data=He3_X_reconstruction(forPi0);;
	if("MC_He3eta"==type)
		m_data=He3_X_analyse(forEta);
	if(
		("MC_He3pi0"==type)||
		("MC_He3pi0pi0"==type)||
		("MC_He3pi0pi0pi0"==type)
	)
		m_data=He3_X_analyse(forPi0);
	if("Data_He3"==type)
		m_data=He3_X_analyse(forData);
	if(nullptr==m_data)
		throw Exception<AnalysisModule>("Cannot create analysis module");
}
AnalysisModule::~AnalysisModule(){
	if(m_data)
		delete m_data;
}
void AnalysisModule::ProcessEvent(){
	if (fProcessed) return;
	fProcessed = kTRUE;
	m_data->ProcessEvent();
}
void AnalysisModule::Clear(Option_t *option){fProcessed=kFALSE;}
void AnalysisModule::Print(Option_t *option){}
void AnalysisModule::UserCommand(CCommand * command){}
