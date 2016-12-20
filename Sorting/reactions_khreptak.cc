#include <Wasa.hh>
#include <TCutG.h>
#include <vector>
#include "math_h/functions.h"
#include "math_h/functions.h"
#include "math_h/vectors.h"
#include "ReconstructionFit/reconstruction_types.h"
#include "Experiment/experiment_conv.h"
#include "Kinematics/reactions.h"
#include "trackprocessing.h"
#include "detectors.h"
#include "reconstruction.h"
#include "data.h"

#include "reactions_khreptak.h"
namespace Khreptak{
    using namespace std;
    using namespace MathTemplates;
    using namespace TrackAnalyse;
    const Reaction& He3eta(){
	static Reaction res(Particle::p(),Particle::d(),{Particle::he3(),Particle::eta()});
	return res;
    }
    Axis Q_axis(const Analysis&res){return Axis([&res]()->double{return 1000.0*He3eta().P2Q(res.PBeam());},-70.0,30.0,40);}
    void SetupForPPNsp(Analysis&res){
	        auto count_ch_CD=make_shared<long>(0);
        auto count_ch_FD=make_shared<long>(0);
        auto count_compl=make_shared<long>(0);

        auto E=make_shared<vector<double>>();
        auto Theta=make_shared<vector<double>>();
        auto Phi=make_shared<vector<double>>();

        res.Trigger(0).pre()
	    <<[count_ch_CD,count_ch_FD,E,Theta,Phi](){
		(*count_ch_CD)=0;
		(*count_ch_FD)=0;
		E->clear();
		Theta->clear();
		Phi->clear();
		return true;
	    };

        res.Trigger(0).per_track()
	    <<[count_ch_CD, count_ch_FD, E, Theta, Phi](WTrack&T){
		if (T.Type()==kCDC)
		    (*count_ch_CD)++;
		if (T.Type()==kFDC)
		    (*count_ch_FD)++;
		
		if ((T.Type()==kCDC) || (T.Type()==kFDC)){
		    E->push_back(T.GetEkin);
		    Theta->push_back(T.GetTheta);
		    Phi->push_back(T.GetPhi);
		}
		return true;
	    };

        res.Trigger(0).post()
	    <<make_shared<Hist1D>("ppn","ChargedCentral",Axis([count_ch_CD]()->double{return *count_ch_CD;},-0.5,9.5,10))
	    <<make_shared<Hist1D>("ppn","ChargedForward",Axis([count_ch_FD]()->double{return *count_ch_FD;},-0.5,9.5,10))
	    <<[E, Theta, Phi,count_compl](){
		(*count_compl)=0;
		vector<Vector3<double>> P;
		for(size_t i=0; i<E->size(); i++){
		    P.push_back(Vector3<double>::Polar(1, Theta->operator[](i), Phi->operator[](i)));
		}
		for(size_t i=0; i<P.size(); i++){
		    for(size_t j=i+1; j<P.size(); j++){
			const auto B=Vector3<double>::basis_z();
			const double compl=B.VecP(P[i])*P[j];
			if(pow(compl,2)<0.1){
			    (*count_compl)++;
			}
		    }
		}
	    }
	    <<make_shared<Hist1D>("ppn","Complanarity",Axis([count_compl]()->double{return *count_compl;},-0.5,9.5,10));
    }
}
