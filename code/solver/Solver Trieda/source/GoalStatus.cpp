#include "GoalStatus.h"


// -----------------------------------------------------------------------------------------------

GoalStatus::GoalStatus(int id, std::string name)
	: id_(id), rowName_(name), value_(-1), gap_(-1), opt_(false), runtime_(-1), 
	percFixedFinal_(0), stopCriteria_(GoalStatus::Unknown),
	objValueAtend_(-1), objValueNrTurmas_(-1), objValueDesloc_(-1), objValueFasesDias_(-1), objValueGap_(-1)
{
}

GoalStatus::~GoalStatus(void)
{}

bool GoalStatus::operator < (GoalStatus const & right) const
{
	return (id_ < right.getId());
}
bool GoalStatus::operator == (GoalStatus const & right) const
{
	return (!(*this < right) && !(right < *this));
}
bool GoalStatus::operator != (GoalStatus const & right) const
{
	return !(*this == right);
}

	
// --------------------------------------------------------------
// General

int GoalStatus::getId() const
{
	return id_;
}
	
std::string GoalStatus::getGoalName() const
{
	return rowName_;
}

void GoalStatus::setValue(double v)
{
	value_ = v;
}
double GoalStatus::getValue() const
{
	return value_;
}

void GoalStatus::setGap(double v)
{
	gap_ = v;
}
double GoalStatus::getGap() const
{
	return gap_;
}

void GoalStatus::setIsOpt(bool v)
{
	opt_ = v;
}
bool GoalStatus::isOpt() const
{
	return opt_;
}	

void GoalStatus::setRunTime(int v)
{
	runtime_ = v;
}
int GoalStatus::getRunTime() const
{
	return runtime_;
}

void GoalStatus::setPercFixedFinal(int v)
{
	percFixedFinal_ = v;
}
int GoalStatus::getPercFixedFinal() const
{
	return percFixedFinal_;
}

void GoalStatus::setStopCriteria(int v)
{
	stopCriteria_ = v;
}
int GoalStatus::getStopCriteria() const
{
	return stopCriteria_;
}
std::string GoalStatus::getStopCriteriaStr() const
{
	switch(stopCriteria_)
	{
	case GoalStatus::Unknown:
		return "Unknown";
	case GoalStatus::TimeLimit:
		return "TimeLimit";
	case GoalStatus::TimeNoImprov:
		return "TimeNoImprov";
	case GoalStatus::AllFree:
		return "AllFree";
	case GoalStatus::GlobalOpt:
		return "GlobalOpt";		
	case GoalStatus::NoNeedOfPolish:
		return "NoNeedOfPolish";
	default:
		return "Error";
	}	
}

	
// --------------------------------------------------------------
// Value for each goal
	
void GoalStatus::setValueAtend(double v)
{
	objValueAtend_ = v;
}
double GoalStatus::getValueAtend() const
{
	return objValueAtend_;
}

void GoalStatus::setValueNrTurmas(double v)
{
	objValueNrTurmas_ = v;
}
double GoalStatus::getValueNrTurmas() const
{
	return objValueNrTurmas_;
}

void GoalStatus::setValueDesloc(double v)
{
	objValueDesloc_ = v;
}	
double GoalStatus::getValueDesloc() const
{
	return objValueDesloc_;
}

void GoalStatus::setValueFasesDias(double v)
{
	objValueFasesDias_ = v;
}
double GoalStatus::getValueFasesDias() const
{
	return objValueFasesDias_;
}
	
void GoalStatus::setValueGap(double v)
{
	objValueGap_ = v;
}
double GoalStatus::getValueGap() const
{
	return objValueGap_;
}