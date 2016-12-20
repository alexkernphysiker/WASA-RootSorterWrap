#include <Wasa.hh>
#include <TCutG.h>
#include "math_h/functions.h"
#include "math_h/tabledata.h"
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
}
