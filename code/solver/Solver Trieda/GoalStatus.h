#ifndef _GOAL_STATUS_H_
#define _GOAL_STATUS_H_

#include <string>

class GoalStatus
{
public:
	GoalStatus(int id, std::string name);
	~GoalStatus(void);
	
	// --------------------------------------------------------------
	// General

	int getId() const;	
	std::string getGoalName() const;

	void setValue(double v);
	double getValue() const;
	
	void setGap(double v);
	double getGap() const;
	
	void setIsOpt(double v);
	double isOpt() const;
	
	void setRunTime(int v);
	int getRunTime() const;

	void setPercFixedFinal(int v);
	int getPercFixedFinal() const;

	void setStopCriteria(int v);
	int getStopCriteria() const;
	std::string getStopCriteriaStr() const;
	
	// --------------------------------------------------------------
	// Value for each goal
	
	void setValueAtend(double v);
	double getValueAtend() const;

	void setValueNrTurmas(double v);
	double getValueNrTurmas() const;
	
	void setValueDesloc(double v);
	double getValueDesloc() const;

	void setValueFasesDias(double v);
	double getValueFasesDias() const;
	
	void setValueGap(double v);
	double getValueGap() const;

	virtual bool operator < (GoalStatus const & right) const;
	virtual bool operator == (GoalStatus const & right) const;
	virtual bool operator != (GoalStatus const & right) const;

	enum StopCriteria
	{
		Unknown,
		TimeLimit,
		TimeNoImprov,
		AllFree,
		GlobalOpt,
		NoNeedOfPolish
	};

private:

	// General
	const int id_;
	const std::string rowName_;
	double value_;
	double gap_;
	bool opt_;
	int runtime_;			// seconds
	int percFixedFinal_;
	int stopCriteria_;

	// Value for each goal
	double objValueAtend_;
	double objValueNrTurmas_;
	double objValueDesloc_;
	double objValueFasesDias_;
	double objValueGap_;
};

#endif