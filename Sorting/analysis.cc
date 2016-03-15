// this file is distributed under 
// MIT license
#include <iostream>
#include <functional>
#include "math_h/error.h"
#include "analysis.h"
#define static_right(A) (static_cast<decltype(A)&&>(A))
using namespace std;
Analysis::Analysis(){
	TrackFinderFD = dynamic_cast<FDFTHTracks*>(gDataManager->GetAnalysisModule("FDFTHTracks","default"));
	if(TrackFinderFD!=0) fTrackBankFD = TrackFinderFD->GetTrackBank();
	CDTrackFinder = dynamic_cast<CDTracksSimple*>(gDataManager->GetAnalysisModule("CDTracksSimple","default"));
	if (CDTrackFinder!=0) fTrackBankCD = CDTrackFinder->GetTrackBank();
	fDetectorTable = dynamic_cast<CCardWDET*>(gParameterManager->GetParameterObject("CCardWDET","default")); 
	m_cache=make_shared<Cache>();
	m_cache->m_count=0;
	m_chain.push_back(make_pair(kFDC,TrackAnalyse::TrackProcess()));
	m_chain.push_back(make_pair(kFDN,TrackAnalyse::TrackProcess()));
	m_chain.push_back(make_pair(kCDC,TrackAnalyse::TrackProcess()));
	m_chain.push_back(make_pair(kCDN,TrackAnalyse::TrackProcess()));
}
Analysis::~Analysis(){}

TrackAnalyse::TrackProcess& Analysis::TrackTypeProcess(const TrackType type){
	for(TrackTypeRec& rec:m_chain)
		if(type==rec.first)
			return rec.second;
	throw MathTemplates::Exception<Analysis>("Cannot find track type");
}
TrackAnalyse::EventProcess& Analysis::EventPreProcessing(){
	return m_pre_event_proc;
}
TrackAnalyse::EventProcess& Analysis::EventPostProcessing(){
	return m_post_event_proc;
}

bool Analysis::Trigger(int n)const{
	return DataSpecificTriggerCheck(n);
}
void Analysis::ProcessEvent(){
	m_cache->p_beam_cache=INFINITY;
	m_cache->m_count++;
	if(m_cache->m_count%1000==0)
		cout<<m_cache->m_count+" events";
	if(DataTypeSpecificEventAnalysis()){
		if(m_pre_event_proc.Process()){
			vector<WTrackBank*> BANK;
			BANK.push_back(fTrackBankCD);
			BANK.push_back(fTrackBankFD);
			for(WTrackBank*bank:BANK){
				WTrackIter iterator(bank);
				while(WTrack* track = dynamic_cast<WTrack*> (iterator.Next()))
					for(const TrackTypeRec& tt:m_chain)
						if(track->Type()==tt.first)
							tt.second.Process(*track);
			}
			m_post_event_proc.Process();
		}
	}
}
double Analysis::PBeam()const{return m_cache->p_beam_cache;}
void Analysis::CachePBeam(const double value)const{
	if(value>0)m_cache->p_beam_cache=value;
	else throw MathTemplates::Exception<Analysis>("Wrong beam momentum value");
}



Analysis::Kinematic::Kinematic(){
	E=INFINITY;Th=INFINITY;Phi=INFINITY;
}
Analysis::particle_info::particle_info(const ParticleType t,const double m){
	type=t;mass=m;cache=make_shared<Kinematic>();
}
void Analysis::AddParticleToFirstVertex(const ParticleType type,const double mass){
	first_vertex.push_back(particle_info(type,mass));
}
const Analysis::Kinematic& Analysis::FromFirstVertex(const ParticleType type)const{
	for(const particle_info&info:first_vertex)
		if(info.type==type)
			return *info.cache;
	throw MathTemplates::Exception<Analysis>("Particle not found in the vertex");
}
void Analysis::ForFirstVertex(function<void(ParticleType,double,shared_ptr<Kinematic>)> cyclebody)const{
	for(const particle_info&info:first_vertex)
		cyclebody(info.type,info.mass,info.cache);
}