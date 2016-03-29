// this file is distributed under 
// MIT license
#ifndef QTBEVPHGEADUWGMY
#define QTBEVPHGEADUWGMY
#include <memory>
#include <utility>
#include <vector>
#include <TLorentzVector.h>
#include <CAnalysisModule.hh>
#include <CDataManager.hh>
#include <FDEdep2Ekin.hh>
#include <CCardWDET.hh>
#include <Wasa.hh>
#include <CAnalysisModule.hh>
#include <REventWmcHeader.hh>
#include <REventHeader.hh>
#include <WTrackBank.hh>
#include <WVertexBank.hh>
#include <FDFTHTracks.hh>
#include <CDTracksSimple.hh>
#include "trackprocessing.h"
#include "analysismodule.hh"
enum TrackType{kFDN=1,kFDC=2,kCDN=11,kCDC=12};
enum ParticleType{
	kDummy=0,kGamma=1,kElectron=2,kPositron=3,kPi0=7,kPiPlus=8,kPiMinus=9,
	kNeutron=13,kProton=14,kEta=17,kDeuteron=45,kTriton=46,kHe3=49
};
class Analysis{
protected:
	Analysis();
public:
	virtual ~Analysis();
	void ProcessEvent();

	double PBeam()const;
	class trig_rec{
	public:
		trig_rec();
		~trig_rec();
		TrackAnalyse::EventProcess&pre();
		TrackAnalyse::TrackProcess&per_track();
		TrackAnalyse::EventProcess&post();
	private:
		TrackAnalyse::TrackProcess m_per_track;
		TrackAnalyse::EventProcess m_pre,m_post;
	};
	trig_rec&Trigger(size_t n);
	
	struct Kinematic{Kinematic();double E,Th,Phi;};
	const Kinematic&FromFirstVertex(const ParticleType type)const;
	void AddParticleToFirstVertex(const ParticleType type,const double mass);
protected:
	virtual bool DataTypeSpecificEventAnalysis()const=0;
	virtual bool DataSpecificTriggerCheck(int n)const=0;
	void CachePBeam(const double value)const;
	void ForFirstVertex(std::function<void(ParticleType,double,std::shared_ptr<Kinematic>)>)const;
private:
	FDFTHTracks* TrackFinderFD;
	CDTracksSimple* CDTrackFinder;
	WTrackBank *fTrackBankFD,*fTrackBankCD;
	CCardWDET *fDetectorTable;
	
	std::vector<trig_rec> m_triggers;

	struct particle_info{
		particle_info(const ParticleType type,const double mass);
		ParticleType type;double mass;
		std::shared_ptr<Kinematic>cache;
	};
	std::vector<particle_info> first_vertex;
	struct Cache{
		double p_beam_cache;
	};
	std::shared_ptr<Cache> m_cache;
};
#endif
