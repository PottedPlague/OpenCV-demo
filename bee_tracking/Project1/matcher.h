#pragma once
#include "tracker.h"

class Matcher
{
public:
	Matcher();
	~Matcher();
	void setThresh(int th);
	std::vector<std::vector<Track*>> subGroupsL;
	std::vector<std::vector<Track*>> subGroupsR;
	struct compStruct {
		bool operator() (Track i, Track j) { return (i.trace.back().y < j.trace.back().y); }
	} compStruct;
	struct compStruct_p {
		bool operator() (Track *i, Track *j) { return (i->trace.back().y < j->trace.back().y); }
	} compStruct_p;

	std::vector<std::vector<Track*>> doMatch(Tracker& left, Tracker& right);
	std::vector<std::vector<Track*>> matchedPair;
	int clrCounter;
private:
	int thresh_;
	int threshGap_;
	bool frameZero_;
};