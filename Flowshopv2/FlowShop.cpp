#include "FlowShop.h"
#include <math.h>
#include <iostream>
#include <ctime>
#include <Windows.h>
#include <string>
#include <fstream>
#include <iostream>
#define E (exp(1))




void FlowShop::StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		std::cerr << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}
double FlowShop::GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}

FlowShop::FlowShop()
{
	PCFreq = 0.0;
	CounterStart = 0;
	bInitialized = false;
}


int FlowShop::GetExecutionTime()
{
	return viMachines[iCurrentResult[iJobsNumber - 1]][iMachineNumber - 1];
}

void FlowShop::CalculateMachines()
{
	for (int i = 0; i < iJobsNumber; i++) {

		for (int j = 0; j < iMachineNumber; j++) {
			if (i == 0)
				if (j == 0)
					viMachines[iCurrentResult[i]][j] = viJobs[iCurrentResult[i]][j];
				else
					viMachines[iCurrentResult[i]][j] = viMachines[iCurrentResult[i]][j - 1] + viJobs[iCurrentResult[i]][j];
			else
				if (j == 0)
					viMachines[iCurrentResult[i]][j] = viMachines[iCurrentResult[i - 1]][j] + viJobs[iCurrentResult[i]][j];
				else
					if (viMachines[iCurrentResult[i - 1]][j] > viMachines[iCurrentResult[i]][j - 1])
						viMachines[iCurrentResult[i]][j] = viMachines[iCurrentResult[i - 1]][j] + viJobs[iCurrentResult[i]][j];
					else
						viMachines[iCurrentResult[i]][j] = viMachines[iCurrentResult[i]][j - 1] + viJobs[iCurrentResult[i]][j];
		}
	}
}

void FlowShop::PromoteResult(int *iCurrent)
{
	for (int i = 0; i < iJobsNumber; i++)
		iBestResult[i] = iCurrent[i];
}

void FlowShop::SetBestToCurrent(int *iBest)
{
	for (int i = 0; i < iJobsNumber; i++)
		iCurrentResult[i] = iBest[i];
}

int FlowShop::SimulatedAnnealing()
{
	srand(time(NULL));

	bool *used = new bool[iJobsNumber];
	int iTmp;
	int iRand1, iRand2;

	for (int i = 0; i < iJobsNumber; i++) {
		used[i] = false;
	}

	//losowanie pierwszego rozwizania
	for (int i = 0; i < iJobsNumber; i++) {
		iTmp = rand() % iJobsNumber;

		if (used[iTmp] == false) {
			used[iTmp] = true;
			iCurrentResult[i] = iTmp;
		}
		else
			if (i > 0) i--;
	}
	CalculateMachines();
	iBestExecutionTime = GetExecutionTime();
	PromoteResult(iCurrentResult);

	while (dT > dMinT) {
		for (int i = 0; i < 100; i++) {
			iRand1 = rand() % iJobsNumber;
			iRand2 = rand() % iJobsNumber;
			if(iRand1 != iRand2)
				std::swap(iCurrentResult[iRand1], iCurrentResult[iRand2]);

			CalculateMachines();
			dP0 = rand() / RAND_MAX;
			if (iBestExecutionTime > GetExecutionTime()) {
				iBestExecutionTime = GetExecutionTime();
				PromoteResult(iCurrentResult);
			}else if(!(std::pow(E, (GetExecutionTime() - iBestExecutionTime) / (-dT)) > dP0))
				std::swap(iCurrentResult[iRand1], iCurrentResult[iRand2]);			
		}
		dT = dT * dAlpha;
	}
	SetBestToCurrent(iBestResult);
	CalculateMachines();
	delete used;
	return 0;
}

void FlowShop::InitBrutForce()
{
	iCountVisited = 0;
	delete iCurrentResult;
	delete iBestResult;
	iCurrentResult = new int[iJobsNumber];
	iBestResult = new int[iJobsNumber];
	iBestExecutionTime = INT_MAX;
	visited = new bool[iJobsNumber];
	for (int i = 0; i < iJobsNumber; i++)
	{
		visited[i] = false;
	}
	BrutForce(-1);
	SetBestToCurrent(iBestResult);
	CalculateMachines();
}

void FlowShop::BrutForce(int v)
{
	if (v == -1)
		for (int i = 0; i < iJobsNumber; i++)
			BrutForce(i);
	else {
		iCurrentResult[iCountVisited++] = v;
		if (iCountVisited < iJobsNumber) {
			visited[v] = true;
			for (int u = 0; u < iJobsNumber; u++)
				if (u != v && !visited[u])
					BrutForce(u);
			visited[v] = false;
		}
		else {
			CalculateMachines();
			if (GetExecutionTime() < iBestExecutionTime) {
				iBestExecutionTime = GetExecutionTime();
				PromoteResult(iCurrentResult);
			}
		}
		iCountVisited--;
	}
}

void FlowShop::Run(char** argv)
{
	dT = 1000;
	dMinT = 1;
	dAlpha = 0.999;
	Load(std::string(argv[1]));
	std::ofstream plik;
	plik.open("wyniki.txt");
	SimulatedAnnealing();
	for (int i = 0; i < 10; i++) {
		delete iCurrentResult;
		delete iBestResult;
		iCurrentResult = new int[iJobsNumber];
		iBestResult = new int[iJobsNumber];
		iBestExecutionTime = INT_MAX;
		StartCounter();
		SimulatedAnnealing();
		plik <<  GetCounter() << std::endl;
	}
	plik.close();
}

void FlowShop::Write(std::string arg)
{
	//plik << dTime;
}

bool FlowShop::Load(std::string arg)
{
	std::ifstream plik;
	plik.open(arg);
	if (!plik.good())
		return false; //Nie udało się otworzyć pliku
	plik >> iMachineNumber;
	plik >> iJobsNumber;

	viJobs = new int*[iJobsNumber];
	viMachines = new int*[iJobsNumber];
	for (int i = 0; i < iJobsNumber; i++) {
		viJobs[i] = new int[iMachineNumber];
		viMachines[i] = new int[iMachineNumber];
	}

	for (int i = 0; i < iJobsNumber; i++) 
		for (int j = 0; j < iMachineNumber; j++) 
			plik >> viJobs[i][j];
		
	
	plik.close();
	iCurrentResult = new int[iJobsNumber];
	iBestResult = new int[iJobsNumber];
	iBestExecutionTime = INT_MAX;
	bInitialized = true;
	return true;
}

FlowShop::~FlowShop()
{
	for (int i = 0; i < iJobsNumber; i++) {
		delete viJobs[i];
		delete viMachines[i];
	}
	delete viJobs;
	delete viMachines;
	delete iCurrentResult;
	delete iBestResult;
}
