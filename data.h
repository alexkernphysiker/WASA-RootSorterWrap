// this file is distributed under 
// GPL license
#ifndef GXMEYPWAEFAIYNHM
#define GXMEYPWAEFAIYNHM
#include "analysis.h"
#include "reconstruction.h"
class RealData:public Analysis{
public:
	RealData(bool weak=false);
	virtual ~RealData();
protected:
	virtual bool DataTypeSpecificEventAnalysis()const override;
	virtual bool DataSpecificTriggerCheck(int n)const override;
private:
        bool m_trigger_weak;
	REventHeader *fHeader;
	InterpolationBasedReconstruction<> BeamMomenta;
};
#endif
