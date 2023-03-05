#pragma once
class Hospital;

class Patient
{
	//Hopital now can access private section of this class
	friend class Hospital;

public:
	//Constructor for initiation
	Patient(int = 0, int = 0, int = 0, int = 0);
	//Constructor for copy
	Patient(Patient*);

	//Necessary functions for getting ans setting parameters
	void setPaBed(int);
	int getPaBed() const;
	int getPaID() const;
	int getPaEntranceTime() const;
	int getPaHospitalizationTime() const;
	int getPaDeathTime() const;
	bool isAlive();

private:
	int paID;
	int paBed;
	int paEntranceTime;
	int paHospitalizationTime;
	int paDeathTime;
	const int entrance, hospitalization, death;
	bool alive;
	Patient* nextPtr;
};

