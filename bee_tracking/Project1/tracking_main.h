#pragma once
#include "KalmanFilter.h"
#include "tracker.h"
#include "track.h"
#include "detector.h"
#include "matcher.h"
#include "sim_coorCalc.h"
#include "visualise3d.h"
#include "tools.h"

int trackingMain(
	int maxSeparation = 50, 
	int maxFrameLoss = 20, 
	int maxTraceLength = 2340, 
	int idCounter = 100
);
