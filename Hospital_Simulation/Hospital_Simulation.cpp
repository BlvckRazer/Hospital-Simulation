// Hospital_Simulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "Hospital.h"
#include "Patient.h"
using namespace std;

void inputDataForSetup(int&, int&, int&);
void patienceReception(Hospital&, int);
void patienceDischarge(Hospital&);
void printPatience(Patient*);
bool anotherRound();
void printDashLine();
void printAsterikLine();

int main()
{
	bool start = true;
	while (start)
	{
		int mode, beds, inputMethod;
		inputDataForSetup(mode, beds, inputMethod);
		Hospital myHospital(beds, mode - 1);
		patienceReception(myHospital, inputMethod);

		while (!myHospital.isHospitalEmpty())
			patienceDischarge(myHospital);

		start = anotherRound();
	}
}

void inputDataForSetup(int& mode, int& beds, int& inputMethod)
{
	cout << "-Select the hospital algorithm:" << endl
		<< "   1-First Come First Served" << endl
		<< "   2-Shortest Job First" << endl
		<< "   3-Priority Scheduling" << endl;
	cout << "Enter: ";
	cin >> mode;
	printDashLine();

	cout << "-Determine the number of beds:" << endl
		<< "Enter: ";
	cin >> beds;
	printDashLine();

	cout << "-How you enter patients data:" << endl
		<< "   1-From a text file" << endl
		<< "   2-Enter one by one from console" << endl;
	cout << "Enter: ";
	cin >> inputMethod;
	printDashLine();
}

void patienceReception(Hospital& myHospital, int inputMethod)
{
	if (inputMethod == 1)
	{
		string fileName;
		cout << "-Place text file in the current "
			"directory and enter its name(filename(.txt))." << endl;
		cout << "Enter:";
		cin >> fileName;

		if (fileName.rfind(".txt") == -1)
			fileName.append(".txt");


		ifstream clientFile(fileName, ios::in);

		if (!clientFile)
		{
			cerr << "File could not be opened!\n"
				"Make sure you are choosing a text file.";
			exit(1);
		}

		int id, entrTime, hospTime, deathTime;

		while (clientFile >> id >> entrTime >> hospTime >> deathTime)
		{
			Patient* person =
				new Patient(id, entrTime, hospTime, deathTime);
			myHospital.reception(person);
		}
		printDashLine();
	}
	else if (inputMethod == 2)
	{
		int id, entrTime, hospTime, deathTime;

		cout << "Enter patience ID or -1 to exit: ";
		cin >> id;

		while (id != -1)
		{
			cout << "Enter patience entrance time: ";
			cin >> entrTime;
			cout << "Enter hopitalization time: ";
			cin >> hospTime;
			cout << "Enter time left to death: ";
			cin >> deathTime;
			printDashLine();
			Patient* person =
				new Patient(id, entrTime, hospTime, deathTime);
			myHospital.reception(person);
			cout << "Enter patience ID or -1 to exit: ";
			cin >> id;
			printDashLine();
			cout << endl << endl;
			printDashLine();
		}
	}
	cout << endl;
}

void patienceDischarge(Hospital& myHospital)
{
	Patient* dischargedPatient = myHospital.discharge();
	Patient* deletePaPtr = dischargedPatient;
	printAsterikLine();
	cout << "Time: " << myHospital.getTime() << endl;
	printDashLine();
	cout << "Hospitalized Patients:" << endl;
	myHospital.printHospitalizedList();
	printDashLine();
	cout << "Waiting Patients: " << endl;
	myHospital.printWaitingList();

	while (dischargedPatient != 0)
	{
		printDashLine();
		printPatience(dischargedPatient);
		dischargedPatient = dischargedPatient->nextPtr;
	}

	while (deletePaPtr != 0)
	{
		Patient* del = deletePaPtr;
		if (deletePaPtr->nextPtr != 0)
			deletePaPtr = deletePaPtr->nextPtr;
		else
			deletePaPtr = 0;
		delete del;
	}

	printAsterikLine();
	cout << endl;
}

void printPatience(Patient* person)
{
	cout << "Discharged Patient:" << endl;
	cout << "ID: " << person->getPaID()
		<< "   Status: " << (person->isAlive() ? "Alive" : "Dead") << endl;
}

bool anotherRound()
{
	char yesNo;

	cout << "-Do you want to make another hospital? (y/n)" << endl;
	cout << "Enter: ";
	cin >> yesNo;
					
	if (yesNo == 'y' || yesNo == 'Y')
	{
		cout << endl << endl;
		return true;
	}
	else
	{
		cout << "Farewell!" << endl;
		return false;
	}
}

void printDashLine()
{
	cout << "-------------------------------"
		"----------------------------" << endl;
}
void printAsterikLine()
{
	cout << "********************************"
		"********************************"
		"********************************" << endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
