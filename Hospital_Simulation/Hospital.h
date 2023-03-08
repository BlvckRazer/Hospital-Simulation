#pragma once
#include "Patient.h"
#include <vector>
using namespace std;

class Hospital
{
public:
	Hospital(int, int = 0);
	int getTime() const;
	int getNumBeds() const;
	bool isFreeBed() const;
	bool isHospitalEmpty() const;
	bool reception(Patient*);
	Patient* discharge();
	void printWaitingList() const;
	void printHospitalizedList() const;
	
private:
	bool addPatient();
	bool removePatient();
	bool assignBed(Patient*);
	void retrieveBed(Patient*);

	void arrangeFCFS(Patient*);
	void arrangeSJF(Patient*);
	void arrangePS(Patient*);

	Patient* dischargeFCFS()		;
	Patient* dischargeSJF();
	Patient* dischargePS();

	void FCFS(Patient*, Patient**, Patient**);
	void SJF(Patient*, Patient**, Patient**);
	void PS(Patient*, Patient**, Patient**);

	void updateDeadList();
	void removeFromList(Patient**, Patient**, Patient**);

private:
	int time;
	int mode;
	int paNumbers;
	int beds;
	bool freeBed;
	vector<bool> bedAvai;
	Patient* hospitalizedPtr;
	Patient* lastHosPtr;
	Patient* waitingPtr;
	Patient* lastWaiPtr;
	Patient* deathPtr;
	//int patientID;
};

