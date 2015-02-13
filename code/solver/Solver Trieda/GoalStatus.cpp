#include "GoalStatus.h"


// -----------------------------------------------------------------------------------------------

GoalStatus::GoalStatus(int id, std::string name)
	: id_(id), rowName_(name), value_(-1), gap_(-1), opt_(false), runtime_(-1)
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

void GoalStatus::setIsOpt(double v)
{
	opt_ = v;
}
double GoalStatus::isOpt() const
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