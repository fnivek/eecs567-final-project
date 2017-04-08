#include <string.h>

#include "matlab_comms.h"
#include "usb.h"
#include "arm.h"

uint8_t _packetBuffs[2][100];
uint8_t _currBuff;
uint8_t _readState;
uint8_t _readLen;
uint8_t _readCount;

void _MatlabCommsCallback(void* buff, uint16_t len) {
	uint8_t* raw = buff;

	for (int q = 0; q < len; q++) {
		switch (_readState) {
			case 0:
				// Awaiting 0xAA
				if (raw[q] == 0xAA) {
					_readState = 1;
				}
				break;
			case 1:
				// Awaiting sequential 0x55
				if (raw[q] == 0x55) {
					_readState = 2;
				}
				else {
					_readState = 0;
				}
				break;
			case 2:
				// Get the number of data bytes
				_readLen = raw[q];
				_readCount = 0;

				if (_readLen == 0) {
					_readState = 0;
				}
				else {
					_readState = 3;
				}
				break;
			case 3:
				_packetBuffs[_currBuff][_readCount] = raw[q];

				if (++_readCount == _readLen) {
					// Done reading
					_readState = 0;
					_currBuff = (_currBuff) ? 0 : 1;
				}
				break;
			default:
				break;
		}
	}
}

void SetupMatlabComms(void) {
	_currBuff = 0;
	_readState = 0;
	UsbAddReadCallback(_MatlabCommsCallback);
}

void MatlabCommsSendAngles(void) {
	uint8_t outBuff[100];
	float angles[ARM_NJOINTS];

	// Get current angles
	ArmGetJointAnglesAll(angles);

	outBuff[0] = 0xAA;
	outBuff[1] = 0x55;
	outBuff[2] = 1 + ARM_NJOINTS * sizeof(float);
	outBuff[3] = 0x10;

	memcpy((void*)(outBuff + 4), angles, ARM_NJOINTS * sizeof(float));

	UsbWrite(outBuff, 4 + ARM_NJOINTS * sizeof(float));
}

void MatlabCommsSendPos(Point3 pos) {
	uint8_t outBuff[100];
	union {
		uint8_t bytes[3 * sizeof(float)];
		Point3 pos;
	} outgoing;

	outgoing.pos = pos;

	outBuff[0] = 0xAA;
	outBuff[1] = 0x55;
	outBuff[2] = 1 + 3 * sizeof(float);
	outBuff[3] = 0x11;

	memcpy((void*)(outBuff + 4), outgoing.bytes, 3 * sizeof(float));

	UsbWrite(outBuff, 4 + 3 * sizeof(float));
}

