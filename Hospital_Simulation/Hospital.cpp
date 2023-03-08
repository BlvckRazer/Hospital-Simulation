#include "Hospital.h"
#include "Patient.h"
#include <iostream>
#include <iomanip>
using namespace std;

Hospital::Hospital(int bedNum, int m)
	:hospitalizedPtr(0), lastHosPtr(0),
	waitingPtr(0), lastWaiPtr(0),
	deathPtr(0), bedAvai(bedNum)
{
	time = 0;
	mode = m;
	paNumbers = 0;
	beds = bedNum;
	freeBed = true;

	for (int bed = 0; bed < getNumBeds(); ++bed)
		bedAvai[bed] = true;
}

int Hospital::getTime() const
{
	return time;
}

int Hospital::getNumBeds() const
{
	return beds;
}

bool Hospital::isFreeBed() const
{
	return freeBed;
}

bool Hospital::isHospitalEmpty() const
{
	return ((hospitalizedPtr == 0 && waitingPtr == 0) ? true : false);
}

bool Hospital::addPatient()
{
	if (isFreeBed())
		++paNumbers;
	else
		return false;

	if (paNumbers == getNumBeds())
		freeBed = false;
	return true;
}

bool Hospital::removePatient()
{
	if (paNumbers == 0)
		return false;
	else
		--paNumbers;

	freeBed = true;
	return true;
}

bool Hospital::assignBed(Patient* person)
{
	for(int bed = 0; bed < getNumBeds(); ++bed)
		if (bedAvai[bed] == true)
		{
			person->setPaBed(bed + 1);
			bedAvai[bed] = false;
			return true;
		}
	return false;
}

void Hospital::retrieveBed(Patient* person)
{
	bedAvai[person->getPaBed() - 1] = true;
}

bool Hospital::reception(Patient* person)
{
	switch (mode)
	{
	case 0:
		arrangeFCFS(person);
		break;
	case 1:
		arrangeSJF(person);
		break;
	case 2:
		arrangePS(person);
		break;
	default:
		return false;
	}

	return true;
}

void Hospital::arrangeFCFS(Patient* person)
{
	if (person->getPaEntranceTime() == 0 && addPatient())
	{
		FCFS(person, &hospitalizedPtr, &lastHosPtr);
		assignBed(person);
	}
	else
		FCFS(person, &waitingPtr, &lastWaiPtr);
}

void Hospital::arrangeSJF(Patient* person)
{
	if (person->getPaEntranceTime() == 0 && addPatient())
	{
		SJF(person, &hospitalizedPtr, &lastHosPtr);
		assignBed(person);
	}
	else
	{
		SJF(person, &hospitalizedPtr, &lastHosPtr);

		if (lastHosPtr != person)
		{
			retrieveBed(lastHosPtr);
			assignBed(person);
		}
		
		Patient* temp = hospitalizedPtr;
		while (temp->nextPtr->nextPtr != 0)
			temp = temp->nextPtr;

		SJF(temp->nextPtr, &waitingPtr, &lastWaiPtr);
		temp->nextPtr = 0;
		lastHosPtr = temp;
	}
}

void Hospital::arrangePS(Patient* person)
{
	if (addPatient() && person->getPaEntranceTime() == 0)
	{
		PS(person, &hospitalizedPtr, &lastHosPtr);
		assignBed(person);
	}
	else
	{
		PS(person, &hospitalizedPtr, &lastHosPtr);

		if (lastHosPtr != person)
		{
			assignBed(person);
			retrieveBed(lastHosPtr);
		}

		Patient* temp = hospitalizedPtr;
		while (temp->nextPtr->nextPtr != 0)
			temp = temp->nextPtr;

		PS(temp->nextPtr, &waitingPtr,& lastWaiPtr);
		temp->nextPtr = 0;
		lastHosPtr = temp;
	}
}

void Hospital::FCFS(Patient* person,
	Patient** startQueue, Patient** endQueue)
{
	if (*startQueue == 0)
	{
		*startQueue = person;
		*endQueue = person;
	}
	else
	{
		Patient* parent = *startQueue;
		while (parent->nextPtr != 0)
			parent = parent->nextPtr;

		parent->nextPtr = person;
		*endQueue = parent->nextPtr;
	}
}

void Hospital::SJF(Patient* person,
	Patient** startQueue, Patient** endQueue)
{
	if (*startQueue == 0)
	{
		*startQueue = person;
		*endQueue = person;
	}
	else if((*startQueue)->nextPtr == 0)
	{
	
		if ((*startQueue)->getPaEntranceTime() ==
			person->getPaEntranceTime() &&
			(*startQueue)->getPaHospitalizationTime() >
			person->getPaHospitalizationTime())
		{
			person->nextPtr = *startQueue;
			*startQueue = person;
			*endQueue = (*startQueue)->nextPtr;
		}
		else
		{
			(*startQueue)->nextPtr = person;
			*endQueue = (*startQueue)->nextPtr;
		}
	}
	else
	{
		Patient* temp = *startQueue;
		while(temp->nextPtr != 0)
		{
			if (temp->nextPtr->getPaEntranceTime() ==
				person->getPaEntranceTime())
			{
				if(temp->nextPtr->getPaHospitalizationTime() >
					person->getPaHospitalizationTime())
					break;
				else if (temp->nextPtr->nextPtr != 0 &&
					(temp->nextPtr->nextPtr->getPaEntranceTime() !=
					person->getPaEntranceTime()))
				{
					temp = temp->nextPtr;
					break;
				}
			}	
			temp = temp->nextPtr;
		}
		person->nextPtr = temp->nextPtr;
		temp->nextPtr = person;

		if (person->nextPtr == 0)
			*endQueue = person;
	}
}

void Hospital::PS(Patient* person, Patient** startQueue,
	Patient** endQueue)
{
	if (*startQueue == 0)
	{
		*startQueue = person;
		*endQueue = person;
	}
	else if ((*startQueue)->nextPtr == 0)
	{
		if ((*startQueue)->getPaEntranceTime() ==
			person->getPaEntranceTime() &&
			(*startQueue)->getPaDeathTime() >
			person->getPaDeathTime())
		{
			person->nextPtr = *startQueue;
			*endQueue = *startQueue;
			*startQueue = person;
		}
		else
		{
			(*startQueue)->nextPtr = person;
			*endQueue = (*startQueue)->nextPtr;
		}
	}
	else
	{
		Patient* temp = *startQueue;
		while (temp->nextPtr != 0)
		{
			if (temp->nextPtr->getPaEntranceTime() ==
				person->getPaEntranceTime())
			{
				if(temp->nextPtr->getPaDeathTime() >
					person->getPaDeathTime())
					break;
				else if (temp->nextPtr->nextPtr->getPaDeathTime() !=
					person->getPaDeathTime())
				{
					temp = temp->nextPtr;
					break;
				}
			}
			temp = temp->nextPtr;
		}
		person->nextPtr = temp->nextPtr;
		temp->nextPtr = person;

		if (person->nextPtr == 0)
			*endQueue = person;
	}
}

Patient* Hospital::discharge()
{
	switch (mode)
	{
	case 0:
		return dischargeFCFS();
		break;
	case 1:
		return dischargeSJF();
		break;
	case 2:
		return dischargePS();
		break;
	default:
		return 0;
	}
}


Patient* Hospital::dischargeFCFS()
{
	Patient* nextPatient = 0;
	Patient* person = 0;

	if (hospitalizedPtr != 0 ||
		waitingPtr != 0)
	{
		++time;
		Patient* temp1 = waitingPtr;
		while (temp1 != 0)
		{
			if (temp1->paEntranceTime != 0)
				--temp1->paEntranceTime;

			--temp1->paDeathTime;
			if (temp1->getPaDeathTime() == 0)
			{
				temp1->alive = false;
				updateDeadList();
			}
			temp1 = temp1->nextPtr;
		}

		Patient* temp2 = hospitalizedPtr;
		while (temp2 != 0)
		{
			--temp2->paHospitalizationTime;
			if (temp2->getPaHospitalizationTime() == 0)
			{
				retrieveBed(temp2);
				removePatient();
				Patient* dischargePa = temp2;
				temp2 = temp2->nextPtr;
				removeFromList(&dischargePa, &hospitalizedPtr, &lastHosPtr);
				if (person == 0)
				{
					person = dischargePa;
					nextPatient = person;
				}
				else
				{
					nextPatient->nextPtr = dischargePa;
					nextPatient = nextPatient->nextPtr;
				}
			}
			else
				temp2 = temp2->nextPtr;
		}

		while (waitingPtr != 0 && isFreeBed() &&
			waitingPtr->getPaEntranceTime() == 0)
		{
			addPatient();
			assignBed(waitingPtr);
			Patient* newPa = waitingPtr;
			waitingPtr = waitingPtr->nextPtr;
			newPa->nextPtr = 0;
			FCFS(newPa, &hospitalizedPtr,
				&lastHosPtr);
		}
	}
	return person;
}

Patient* Hospital::dischargeSJF()
{
	Patient* nextPatient = 0;
	Patient* person = 0;

	if (hospitalizedPtr != 0 ||
		waitingPtr != 0)
	{
		++time;
		Patient* temp1 = waitingPtr;
		while (temp1 != 0)
		{
			if (temp1->paEntranceTime != 0)
				--temp1->paEntranceTime;

			--temp1->paDeathTime;
			if (temp1->getPaDeathTime() == 0)
			{
				temp1->alive = false;
				updateDeadList();
			}
			temp1 = temp1->nextPtr;
		}

		Patient* temp2 = hospitalizedPtr;
		while (temp2 != 0)
		{
			--temp2->paHospitalizationTime;
			if (temp2->getPaHospitalizationTime() == 0)
			{
				retrieveBed(temp2);
				removePatient();
				Patient* dischargePa = temp2;
				temp2 = temp2->nextPtr;
				removeFromList(&dischargePa, &hospitalizedPtr, &lastHosPtr);
				if (person == 0)
				{
					person = dischargePa;
					nextPatient = person;
				}
				else
				{
					nextPatient->nextPtr = dischargePa;
					nextPatient = nextPatient->nextPtr;
				}
			}
			else
				temp2 = temp2->nextPtr;
		}
	}

	while (waitingPtr != 0 && isFreeBed() &&
		waitingPtr->getPaEntranceTime() == 0)
	{
		addPatient();
		assignBed(waitingPtr);
		Patient* newPa = waitingPtr;
		waitingPtr = waitingPtr->nextPtr;
		newPa->nextPtr = 0;
		SJF(newPa, &hospitalizedPtr,
			&lastHosPtr);
	}

	return person;
}

Patient* Hospital::dischargePS()
{
	while (hospitalizedPtr != 0 ||
		waitingPtr != 0)
	{
		++time;
		Patient* temp1 = waitingPtr;
		while (temp1 != 0)
		{
			if (waitingPtr->paEntranceTime != 0)
				--temp1->paEntranceTime;
			temp1 = temp1->nextPtr;
		}
	}

	Patient* temp2 = hospitalizedPtr;
	while (temp2 != 0)
	{
		--temp2->paHospitalizationTime;
		if (temp2->getPaHospitalizationTime() == 0)
		{
			retrieveBed(temp2);
			if (waitingPtr->getPaEntranceTime() == 0)
			{
				assignBed(waitingPtr);
				Patient* newPa = waitingPtr;
				waitingPtr = waitingPtr->nextPtr;
				newPa->nextPtr = 0;
				PS(waitingPtr, &hospitalizedPtr,
					&lastHosPtr);
			}
		}
		temp2 = temp2->nextPtr;
	}

	Patient* temp3 = waitingPtr;
	while (temp3->getPaEntranceTime() == 0)
	{
		if (lastHosPtr->getPaDeathTime() >
			temp3->getPaDeathTime())
		{
			PS(temp3, &hospitalizedPtr, &lastHosPtr);
			PS(temp3, &hospitalizedPtr, &lastHosPtr);
			retrieveBed(lastHosPtr);
			assignBed(temp3);

			Patient* stoppedParent = hospitalizedPtr;
			while (stoppedParent->nextPtr->nextPtr != 0)
				stoppedParent = stoppedParent->nextPtr;

			PS(stoppedParent->nextPtr, &waitingPtr, 0);
			stoppedParent->nextPtr = 0;
			lastHosPtr = stoppedParent;

		}
		temp3 = temp3->nextPtr;
	}
	return 0;
}

void Hospital::updateDeadList()
{
	if (!waitingPtr->isAlive())
	{
		if (deathPtr == 0)
		{
			deathPtr = waitingPtr;
			waitingPtr = waitingPtr->nextPtr;
			deathPtr->nextPtr = 0;
		}
		else
		{
			Patient* deadParent = deathPtr;
			while (deadParent->nextPtr != 0)
				deadParent = deadParent->nextPtr;
			deadParent->nextPtr = waitingPtr;
			waitingPtr = waitingPtr->nextPtr;
			deadParent->nextPtr->nextPtr = 0;
		}
	}
	else
	{
		Patient* waitedPatient = waitingPtr;
		while (waitedPatient->nextPtr != 0  &&
			waitedPatient->nextPtr->isAlive())
			waitedPatient = waitedPatient->nextPtr;

		if (waitedPatient->nextPtr == 0)
			;
		else
		{
			Patient* deadParent = deathPtr;
			while (deadParent->nextPtr != 0)
				deadParent = deadParent->nextPtr;
			deadParent->nextPtr = waitedPatient->nextPtr;
			waitedPatient->nextPtr = waitedPatient->nextPtr->nextPtr;
			deadParent->nextPtr->nextPtr = 0;
		}
	}
}

void Hospital::removeFromList(Patient** person, Patient** head, Patient** tail)
{
	if ((*head)->nextPtr == 0)
		*head = *tail = 0;
	else
	{
		if ((*head)->getPaID() == (*person)->getPaID())
		{
			*head = (*head)->nextPtr;
			(*person)->nextPtr = 0;
		}
		else
		{
			Patient* temp = *head;
			while (temp->nextPtr != 0)
			{
				if (temp->nextPtr->getPaID() == (*person)->getPaID())
					break;
				temp = temp->nextPtr;
			}
			if (temp->nextPtr->nextPtr == 0)
				*tail = temp;
			temp->nextPtr = temp->nextPtr->nextPtr;
			(*person)->nextPtr = 0;
		}
	}
}

void Hospital::printWaitingList() const
{
	cout << "ID" << '\t'
		<< "Status" << '\t'
		<< "Entrance Time" << "\t"
		<< "Hospitalization Time" << '\t' 
		<< "Time to Death"<< endl;

	Patient* temp = waitingPtr;

	while (temp != 0)
	{
		cout << setw(2) << setfill('0') << temp->getPaID()
			<< "\t"
			<< (temp->isAlive() ? "Alive \t" : "Dead  \t")
			<< setw(2) << setfill('0') << temp->entrance << "           \t"
			<< setw(2) << setfill('0') << temp->getPaHospitalizationTime()
			<< "                  \t" << setw(2) << setfill('0')
			<< temp->getPaDeathTime() << endl;

		temp = temp->nextPtr;
	}

	temp = deathPtr;

	while (temp != 0)
	{
		cout << setw(2) << setfill('0') << temp->getPaID()
			<< "\t" << (temp->isAlive() ? "Alive \t" : "Dead  \t")
			<< setw(2) << setfill('0') << temp->entrance << "           \t"
			<< setw(2) << setfill('0') << temp->getPaHospitalizationTime()
			<< "                  \t" << setw(2) << setfill('0')
			<< temp->getPaDeathTime() << endl;

		temp = temp->nextPtr;
	}
}

void Hospital::printHospitalizedList() const
{
	cout << "Patient ID" << '\t'
		<< "Hospitalization Time (Remaining)" << '\t'
		<< "Time to Death (Remaining)" << endl;

	Patient* temp = hospitalizedPtr;

	while (temp != 0)
	{
		cout << setw(2) << setfill('0') << temp->getPaID()
			<< "        \t" << setw(2) << setfill('0') 
			<< temp->hospitalization << '('<< setw(2) << setfill('0')
			<< temp->getPaHospitalizationTime() << ')'
			<< "                          \t" << setw(2) << setfill('0')
			<< temp->death << '(' << setw(2) << setfill('0')
			<< temp->getPaDeathTime() << ')' << endl;
		temp = temp->nextPtr;
	}
}