// this file is distributed under 
// MIT license
#include "math_h/functions.h"
#include "math_h/tabledata.h"
#include "X_2gamma.h"
namespace ReactionSetup{
	using namespace TrackAnalyse;
	using namespace MathTemplates;
	void SearchGammaTracks(Analysis&res){
		auto data=make_shared<vector<particle_kinematics>>();
		res.Trigger(17).pre()<<[data](){data->clear(); return true;};
		res.Trigger(17).per_track()<<(make_shared<ChainCheck>()
		<<[](WTrack&T)->bool{return T.Type()==kCDN;}
		<<[data](WTrack&T)->bool{
			data->push_back({.particle=Particle::gamma(),.E=T.Edep(),.theta=T.Theta(),.phi=T.Phi()});
			return true;
		}
		);
		auto im_val=make_shared<double>(INFINITY);
		res.Trigger(17).post()
		<< [data,im_val](){
			(*im_val)=INFINITY;
			SortedPoints<double> table;
			for(size_t i=0;i<data->size();i++)
				for(size_t j=i+1;j<data->size();j++){
					double im=InvariantMass({data->operator[](i),data->operator[](j)});
					double rest=0;
					for(size_t k=0;k<data->size();k++)if((k!=i)&&(k!=j))
						rest+=data->operator[](k).E;
					table<<point<double>(rest,im);
				}
				if(table.size()>0)
					if(table[0].X()<0.050)//noise threshold
						(*im_val)=table[0].Y();
					return true;
		}
		<< make_shared<SetOfHists1D>("CentralGammas","InvMass2Gamma",Q_axis(res),Axis([im_val]()->double{return *im_val;},0.0,0.8,800))
		<< make_shared<Hist1D>("CentralGammas","neutral_tracks_count",Axis([data]()->double{return data->size();},-0.5,9.5,10));
	}
	
}