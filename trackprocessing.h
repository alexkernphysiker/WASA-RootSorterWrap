// this file is distributed under 
// GPL license
#ifndef xoyoyptv
#define xoyoyptv
#include <functional>
#include <vector>
#include <memory>
#include <string>
#include <WTrack.hh>
#include <TH1F.h>
#include <TH2F.h>
namespace TrackAnalyse{
	//WTrack cannot be transfered as const because
	//it does not contain const methods 
	//(even those ones that really should be)
	typedef std::function<double()> ValueIndependent;
	typedef std::function<double(WTrack&)> ValueTrackDependent;
	typedef std::function<bool()> ConditionIndependent;
	typedef std::function<bool(WTrack&)> ConditionTrackDependent;
	class Axis{
	public:
		Axis(ValueTrackDependent v,double f, double t,unsigned int b);
		Axis(ValueIndependent v,double f, double t,unsigned int b);
		Axis(const Axis&source);
		Axis(ValueTrackDependent v,const Axis&source);
		Axis(ValueIndependent v,const Axis&source);
		~Axis();
		double left()const;
		double right()const;
		unsigned int count()const;
		double operator()(WTrack&T)const;
		ValueTrackDependent valuegetter()const;
		double bin_width()const;
		double bin_center(size_t i)const;
		bool FindBinIndex(unsigned int&output,WTrack&T)const;
	private:
		void CheckCorrectness()const;
		ValueTrackDependent value;
		double from;
		double to;
		unsigned int bins;
	};
	class ITrackProcess{
	public:
		virtual bool Process(WTrack&)const=0;
		virtual ~ITrackProcess(){}
	};
	class ITrackAnalyse:public ITrackProcess{
	public:
		virtual bool Process(WTrack&)const final;
		virtual ~ITrackAnalyse(){}
	protected:
		virtual void Analyse(WTrack&)const =0;
	};
	
	class Hist1D:public ITrackAnalyse{
	public:
		Hist1D(const std::string&dir,const std::string&name,const Axis&x);
		virtual ~Hist1D();
	protected:
		virtual void Analyse(WTrack&)const override;
	private:
		TH1F *hist;
		Axis X;
	};
	class SetOfHists1D:public ITrackAnalyse{
	public:
		SetOfHists1D(const std::string&dir,const std::string&name,const Axis&binning,const Axis&x);
		virtual ~SetOfHists1D();
	protected:
		virtual void Analyse(WTrack&)const override;
	private:
		Axis Z,X;
		TH1F *All,*OutOfBorder;
		std::vector<TH1F*> Bins;
	};

	class Hist2D:public ITrackAnalyse{
	public:
		Hist2D(const std::string&dir,const std::string&name,const Axis&x,const Axis&y);
		virtual ~Hist2D();
	protected:
		virtual void Analyse(WTrack&)const override;
	private:
		TH2F *hist;
		Axis X,Y;
	};
	class SetOfHists2D:public ITrackAnalyse{
	public:
		SetOfHists2D(const std::string&dir,const std::string&name,const Axis&binning,const Axis&x,const Axis&y);
		virtual ~SetOfHists2D();
	protected:
		virtual void Analyse(WTrack&)const override;
	private:
		Axis Z,X,Y;
		TH2F *All,*OutOfBorder;
		std::vector<TH2F*> Bins;
	};
	class Condition:public ITrackProcess{
	public:
		Condition(ConditionTrackDependent func);
		Condition(ConditionIndependent func);
		virtual ~Condition();
		virtual bool Process(WTrack&)const override;
	private:
		ConditionTrackDependent condition;
	};
	class AbstractChain:public ITrackProcess{
	protected:
		AbstractChain();
	public:
		virtual ~AbstractChain();
		AbstractChain&operator<<(std::shared_ptr<ITrackProcess>element);
	protected:
		const std::vector<std::shared_ptr<ITrackProcess>>&chain()const;
	private:
		std::vector<std::shared_ptr<ITrackProcess>> m_chain;
	};
	std::shared_ptr<AbstractChain>operator<<(std::shared_ptr<AbstractChain>ch,std::shared_ptr<ITrackProcess>v);
	inline std::shared_ptr<AbstractChain>operator<<(std::shared_ptr<AbstractChain>ch,ConditionTrackDependent f){
		return ch<<std::dynamic_pointer_cast<ITrackProcess>(std::make_shared<Condition>(f));
	}
	inline std::shared_ptr<AbstractChain>operator<<(std::shared_ptr<AbstractChain>ch,ConditionIndependent f){
		return ch<<std::dynamic_pointer_cast<ITrackProcess>(std::make_shared<Condition>(f));
	}
	class Chain:public AbstractChain{
	public:
		Chain(){}
		virtual ~Chain(){}
		virtual bool Process(WTrack&)const override;
	};
	class ChainCheck:public AbstractChain{
	public:
		ChainCheck(){}
		virtual ~ChainCheck(){}
		virtual bool Process(WTrack&)const override;
	};
	class ChainAnd:public AbstractChain{
	public:
		ChainAnd(){}
		virtual ~ChainAnd(){}
		virtual bool Process(WTrack&)const override;
	};
	class ChainOr:public AbstractChain{
	public:
		ChainOr(){}
		virtual ~ChainOr(){}
		virtual bool Process(WTrack&)const override;
	};
	class ChainBinner:public AbstractChain{
	public:
		ChainBinner(const Axis&source);
		virtual ~ChainBinner();
		virtual bool Process(WTrack&)const override;
	private:
		Axis m_axis;
	};
	class TrackProcess{
	public:
		TrackProcess(){}
		~TrackProcess(){}
		TrackProcess&operator<<(std::shared_ptr<ITrackProcess>element);
		void Process(WTrack&T)const;
	private:
		std::vector<std::shared_ptr<ITrackProcess>> m_proc;
	};
	std::shared_ptr<TrackProcess>operator<<(std::shared_ptr<TrackProcess>ch,std::shared_ptr<ITrackProcess>v);
	inline TrackProcess&operator<<(TrackProcess&ch,ConditionTrackDependent f){
		return ch<<std::make_shared<Condition>(f);
	}
	inline TrackProcess&operator<<(TrackProcess&ch,ConditionIndependent f){
		return ch<<std::make_shared<Condition>(f);
	}
	class EventProcess{
	public:
		EventProcess(){}
		~EventProcess(){}
		EventProcess&operator<<(std::shared_ptr<ITrackProcess>element);
		bool Process()const;
	private:
		std::vector<std::shared_ptr<ITrackProcess>> m_proc;
	};
	std::shared_ptr<EventProcess>operator<<(std::shared_ptr<EventProcess>ch,std::shared_ptr<ITrackProcess>v);
	inline EventProcess&operator<<(EventProcess&ch,ConditionTrackDependent f){
		return ch<<std::make_shared<Condition>(f);
	}
	inline EventProcess&operator<<(EventProcess&ch,ConditionIndependent f){
		return ch<<std::make_shared<Condition>(f);
	}
}
#endif 
