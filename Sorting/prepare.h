// this file is distributed under 
// MIT license
#ifndef ___________prepare___h_____
#	define ___________prepare___h_____
#include "ReconstructionFit/reconstruction_types.h"
#include "Experiment/experiment_conv.h"
#include "Kinematics/reactions.h"
#include "analysis.h"
#include "trackprocessing.h"
#include "detectors.h"
#include "reconstruction.h"

namespace ReactionSetup{
	enum AnalysisModification{forData,forMC};
	const Reaction&He3eta();
	TrackAnalyse::Axis Q_axis(const Analysis&res);
	Analysis*Prepare(const AnalysisModification mode);
}
#endif