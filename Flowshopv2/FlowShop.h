#pragma once
#include <vector>

class FlowShop
{
private:
	int** viJobs;
	int** viMachines;
	int* iCurrentResult;
	int* iBestResult;
	double dT;
	double dMinT;
	double dAlpha;
	double dP0;

	int iBestExecutionTime;
	int iMachineNumber;
	int iJobsNumber;

public:
	FlowShop();
	void Initialize();
	int GetExecutionTime();
	void CalculateMachines();
	void PromoteResult(int*);
	int Execute();
	void Run();
	void Write();
	~FlowShop();
};

