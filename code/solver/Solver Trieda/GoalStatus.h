#ifndef _GOAL_STATUS_H_
#define _GOAL_STATUS_H_

#include <string>

class GoalStatus
{
public:
	GoalStatus(int id, std::string name);
	~GoalStatus(void);
	
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

	virtual bool operator < (GoalStatus const & right) const;
	virtual bool operator == (GoalStatus const & right) const;
	virtual bool operator != (GoalStatus const & right) const;

private:
	const int id_;
	const std::string rowName_;
	double value_;
	double gap_;
	bool opt_;
	int runtime_;		// seconds
};

#endif