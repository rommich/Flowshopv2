#include "FlowShop.h"
#include <math.h>
#include <iostream>
#include <ctime>
#include <Windows.h>
#define E (exp(1))
FlowShop::FlowShop()
{
}

void FlowShop::Initialize()
{
	int iTemp;

	std::cout << "Wprowadz liczbe maszyn:" << std::endl;
	std::cin >> iMachineNumber;
	std::cout  << "Wprowadz liczbe zadan:" << std::endl;
	std::cin >> iJobsNumber;

	viJobs = new int*[iJobsNumber];
	viMachines = new int*[iJobsNumber];
	for (int i = 0; i < iJobsNumber; i++) {
		viJobs[i] = new int[iMachineNumber];
		viMachines[i] = new int[iMachineNumber];
	}

	for (int i = 0; i < iJobsNumber; i++) {
		std::cout << "Wprowadz czasy dla zadania: " << i << " dla maszyny:"  << std::endl;
	
		for (int j = 0; j < iMachineNumber; j++) {
			std::cout << j << ": ";
			std::cin >> viJobs[i][j];
		}
	}
	std::cout << "Wprowadz temperature poczatkowa:" << std::endl;
	std::cin >> dT;
	std::cout << "Wprowadz temperature minimalna:" << std::endl;
	std::cin >> dMinT;
	std::cout << "Wprowadz wspolczynnik schladzania:" << std::endl;
	std::cin >> dAlpha;

	iCurrentResult = new int[iJobsNumber];
	iBestResult = new int[iJobsNumber];
	iBestExecutionTime = INT_MAX;	
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

int FlowShop::Execute()
{
	srand(time(NULL));
	bool *used = new bool[iJobsNumber];
	int iTmp;
	int iExecutionTime, iRand1, iRand2;

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
	return 0;
}

void FlowShop::Run()
{
	Execute();
	Write();
}

void FlowShop::Write()
{
	std::cout << std::endl << "Czas wykonania zadan: " << iBestExecutionTime << std::endl << "kolejnosc: " << std::endl;
	for (int i = 0; i < iJobsNumber; i++)
		std::cout << iBestResult[i] << " ";
	std::cout << std::endl << std::endl;

	for (int i = 0; i < iMachineNumber; i++) {
		for (int j = 0; j < iJobsNumber; j++) {

			if (i == 0)		//maszyna 0
				for (int k = 0; k < viJobs[iBestResult[j]][i]; k++)
					std::cout << iBestResult[j];
			else			//maszyna != 0
				if (j == 0) {		//zadanie 0
					for (int k = 0; k < viMachines[iBestResult[j]][i-1]; k++)
						std::cout << "-";
					for (int k = 0; k < viJobs[iBestResult[j]][i]; k++)
						std::cout << iBestResult[j];
				}
				else		//zadanie != 0
					if (viMachines[iBestResult[j - 1]][i] > viMachines[iBestResult[j]][i - 1])
						for (int k = 0; k < viJobs[iBestResult[j]][i]; k++)
							std::cout << iBestResult[j];
					else {
						for (int k = 0; k < (viMachines[iBestResult[j]][i - 1] - viMachines[iBestResult[j - 1]][i]); k++)
							std::cout << "-";
						for (int k = 0; k < viJobs[iBestResult[j]][i]; k++)
							std::cout << iBestResult[j];
					}

		}
		std::cout << std::endl;
	}

	system("pause");
}

FlowShop::~FlowShop()
{
}
