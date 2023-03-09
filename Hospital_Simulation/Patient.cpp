#include "Patient.h"

Patient::Patient(int id, int entranceTime,
	int hospitalizationTime, int deathTime)
	:nextPtr(0), entrance(entranceTime), paID(id),
	hospitalization(hospitalizationTime),
	death(deathTime)
{
	paEntranceTime = entranceTime;
	paHospitalizationTime = hospitalizationTime;
	paDeathTime = deathTime;
	paBed = 0;
	alive = true;
}

Patient::Patient(Patient* copy)
	:entrance(copy->entrance), paID(copy->paID),
	hospitalization(copy->hospitalization),
	death(copy->death)
{
	this->paEntranceTime =
		copy->getPaEntranceTime();
	this->paHospitalizationTime =
		copy->getPaHospitalizationTime();
	this->paDeathTime = copy->getPaDeathTime();
	this->paBed = copy->getPaBed();
}
		
void Patient::setPaBed(int num)
{
	paBed = num;
}

int Patient::getPaBed() const
{
	return paBed;
}

int Patient::getPaID() const
{
	return paID;
}

int Patient::getPaEntranceTime() const
{
	return paEntranceTime;
}

int Patient::getPaHospitalizationTime() const
{
	return paHospitalizationTime;
}

int Patient::getPaDeathTime() const
{
	return paDeathTime;
}

bool Patient::isAlive() const
{
	return alive;
}
