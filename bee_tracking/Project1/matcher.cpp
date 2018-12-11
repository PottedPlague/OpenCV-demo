#include "matcher.h"

Matcher::Matcher()
{
	thresh_ = 10;
	threshGap_ = 5;
	frameZero_ = true;
}

Matcher::~Matcher()
{
}

void Matcher::setThresh(int th)
{
	thresh_ = th;
}

std::vector<std::vector<Track*>> Matcher::doMatch(Tracker& left, Tracker& right)
{
	//mapping
	if (frameZero_)																//frame 1
	{
		frameZero_ = !frameZero_;
		std::sort(left.tracks.begin(), left.tracks.end(), compStruct);			//up to down (small to big)
		std::sort(right.tracks.begin(), right.tracks.end(), compStruct);
		std::vector<Track*> subGroup;
		subGroupsL.push_back(subGroup);
		subGroupsR.push_back(subGroup);
		//subGroups[0].push_back(&tracks[0]);

		//left size == 1 or right size == 1
		//[!!!]code here ...

		if (left.tracks.size() > 1 && right.tracks.size() > 1)						
		{
			std::vector<int> breakpointsL;
			std::vector<int> breakpointsR;
			breakpointsL.push_back(0);
			breakpointsR.push_back(0);
			for (int i = 0; i < left.tracks.size(); i++)
			{
				subGroupsL[0].push_back(&left.tracks[i]);
				if (i == 0) continue;

				int gap = left.tracks[i].trace.back().y - left.tracks[i - 1].trace.back().y;
				if (gap > thresh_)
					breakpointsL.push_back(i);
			}
			for (int i = 0; i < right.tracks.size(); i++)
			{
				subGroupsR[0].push_back(&right.tracks[i]);
				if (i == 0) continue;

				int gap = right.tracks[i].trace.back().y - right.tracks[i - 1].trace.back().y;
				if (gap > thresh_)
					breakpointsR.push_back(i);
			}
			breakpointsL.push_back(breakpointsL.back() + 1);
			breakpointsR.push_back(breakpointsR.back() + 1);
			int p = 1;
			for (;;)
			{
				int gap = abs(left.tracks[breakpointsL[p]].trace.back().y - right.tracks[breakpointsR[p]].trace.back().y);
				if (gap <= threshGap_)
				{
					p++;
				}
				else if (left.tracks[breakpointsL[p]].trace.back().y < right.tracks[breakpointsR[p]].trace.back().y)
					breakpointsL.erase(breakpointsL.begin() + p);
				else
					breakpointsR.erase(breakpointsR.begin() + p);

				if (p >= breakpointsL.size() && p >= breakpointsR.size())
					break;
			}
			std::vector<std::vector<Track*>> newGroupsL;
			std::vector<std::vector<Track*>> newGroupsR;
			for (int i = 0; i < breakpointsL.size() - 1; i++)
			{
				std::vector<Track*> newGroupL(subGroupsL[0].begin() + breakpointsL[i], subGroupsL[0].begin() + breakpointsL[i + 1]);
				newGroupsL.push_back(newGroupL);
			}
			for (int i = 0; i < breakpointsR.size() - 1; i++)
			{
				std::vector<Track*> newGroupR(subGroupsR[0].begin() + breakpointsR[i], subGroupsR[0].begin() + breakpointsR[i + 1]);
				newGroupsR.push_back(newGroupR);
			}
			subGroupsL.clear();
			subGroupsL = newGroupsL;
			subGroupsR.clear();
			subGroupsR = newGroupsR;
		}
		else
		{
			for (int i = 0; i < left.tracks.size(); i++)
				subGroupsL[0].push_back(&left.tracks[i]);

			for (int i = 0; i < right.tracks.size(); i++)
				subGroupsR[0].push_back(&right.tracks[i]);
		}
	}
	else														//frame 2 and later
	{
		std::vector<std::vector<Track*>> newGroupsL;
		std::vector<std::vector<Track*>> newGroupsR;
		for (int i = 0; i < subGroupsL.size(); i++)				//subGroupsL.size() and subGroupsR.size() must be equal.
		{
			std::vector<int> breakpointsL;
			std::vector<int> breakpointsR;
			breakpointsL.push_back(0);
			breakpointsR.push_back(0);
			for (int j = 0; j < subGroupsL[i].size(); j++)
			{
				std::sort(subGroupsL[i].begin(), subGroupsL[i].end(), compStruct_p);
				if (j == 0) continue;

				int gap = subGroupsL[i][j]->trace.back().y - subGroupsL[i][j - 1]->trace.back().y;
				if (gap > thresh_)
					breakpointsL.push_back(j);
			}
			for (int j = 0; j < subGroupsR[i].size(); j++)
			{
				std::sort(subGroupsR[i].begin(), subGroupsR[i].end(), compStruct_p);
				if (j == 0) continue;

				int gap = subGroupsR[i][j]->trace.back().y - subGroupsR[i][j - 1]->trace.back().y;
				if (gap > thresh_)
					breakpointsR.push_back(j);
			}
			breakpointsL.push_back(breakpointsL.back() + 1);
			breakpointsR.push_back(breakpointsR.back() + 1);
			int p = 1;
			for (;;)
			{
				int gap = abs(subGroupsL[i][breakpointsL[p]]->trace.back().y - subGroupsR[i][breakpointsR[p]]->trace.back().y);
				if (gap <= threshGap_)
				{
					p++;
				}
				else if (subGroupsL[i][breakpointsL[p]]->trace.back().y < subGroupsR[i][breakpointsR[p]]->trace.back().y)
					breakpointsL.erase(breakpointsL.begin() + p);
				else
					breakpointsR.erase(breakpointsR.begin() + p);

				if (p >= breakpointsL.size() && p >= breakpointsR.size())
					break;
			}
			for (int j = 0; i < breakpointsL.size() - 1; i++)
			{
				std::vector<Track*> newGroupL(subGroupsL[i].begin() + breakpointsL[j], subGroupsL[i].begin() + breakpointsL[j + 1]);
				newGroupsL.push_back(newGroupL);
			}
			for (int j = 0; i < breakpointsR.size() - 1; i++)
			{
				std::vector<Track*> newGroupR(subGroupsR[i].begin() + breakpointsR[j], subGroupsR[i].begin() + breakpointsR[j + 1]);
				newGroupsR.push_back(newGroupR);
			}
		}
		subGroupsL.clear();
		subGroupsL = newGroupsL;
		subGroupsR.clear();
		subGroupsR = newGroupsR;
	}

	return std::vector<std::vector<Track*>>();
}
