// this file is distributed under 
// MIT license
#include <Wasa.hh>
#include <TCutG.h>
#include "math_h/functions.h"
#include "math_h/tabledata.h"
#include "trackprocessing.h"
#include "example.h"
namespace ReactionSetup{
	using namespace std;
	using namespace MathTemplates;
	using namespace TrackAnalyse;
    void prepare_example_analyse(Analysis&res){
	auto forward_count=make_shared<unsigned int>(0);
	auto central_count=make_shared<unsigned int>(0);
	
	res.Trigger(0).pre()
	    <<[forward_count](){(*forward_count)=0;return true;}
	    <<[central_count](){(*central_count)=0;return true;};
	res.Trigger(0).per_track()
	    <<(make_shared<ChainCheck>()
		<<[](WTrack&T)->bool{return T.Type()==kFDC;}
		<<[forward_count](){(*forward_count)++;return true;}
	    )
	    <<(make_shared<ChainCheck>()
		<<[](WTrack&T)->bool{return T.Type()==kFDC;}
		<<[forward_count](){(*forward_count)++;return true;}
	    );
	res.Trigger(0).post()
	    <<make_shared<Hist1D>("Example_spectra","Forward_charged_tracks",
		Axis([forward_count]()->double{return *forward_count;},-0.5,9.5,10)
	    )
	    <<make_shared<Hist1D>("Example_spectra","Central_charged_tracks",
		Axis([central_count]()->double{return *central_count;},-0.5,9.5,10)
	    );
    }
}
