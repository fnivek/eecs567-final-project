#ifndef MATLAB_COMMS_H
#define MATLAB_COMMS_H

#include "helpers.h"


void SetupMatlabComms(void);

void MatlabCommsSendAngles(float* angles, int size);
void MatlabCommsSendPos(Point3 pos);






#endif // MATLAB_COMMS_H

