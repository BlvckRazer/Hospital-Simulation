#pragma once
#include "Patient.h"
#include <vector>
using namespace std;

class Hospital
{
public:
	//The second parameter is 0, 1, or 2
	//for FCFS, SJF, and PS algorithms  respectively
	Hospital(int, int = 0);
	//Interface functions
	int getTime() const;
	int getNumBeds() const;
	bool isFreeBed() const;
	bool isHospitalEmpty() const;
	bool reception(Patient*);
	Patient* discharge();
	void printWaitingList() const;
	void printHospitalizedList() const;
	
private:
	//Utility functions
	//These functions used inside the object
	bool addPatient();
	bool removePatient();
	bool assignBed(Patient*);
	void retrieveBed(Patient*);

	void arrangeFCFS(Patient*);
	void arrangeSJF(Patient*);
	void arrangePS(Patient*);

	Patient* dischargeFCFS();
	Patient* dischargeSJF();
	Patient* dischargePS();
	//These functions sort each list of patients
	//based on three specified algorithms
	void FCFS(Patient*, Patient**, Patient**);
	void SJF(Patient*, Patient**, Patient**);
	void PS(Patient*, Patient**, Patient**);
	//This function check the waiting 
	//list and add dead patients to dead list
	void updateDeadList();

	void removeFromList(Patient**, Patient**, Patient**);

private:
	int time; //General Time
	int mode;
	int paNumbers; //Number of hospitalized patients
	int beds;
	bool freeBed;
	vector<bool> bedAvai; //Availability of beds vector

	//Hospitalized list pointers
	Patient* hospitalizedPtr;
	Patient* lastHosPtr;
	//Waiting list pointers
	Patient* waitingPtr;
	Patient* lastWaiPtr;
	//Dead list pointer
	Patient* deathPtr;
};