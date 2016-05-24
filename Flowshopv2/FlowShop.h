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

	bool* visited;
	int iCountVisited;

public:
	FlowShop();
	void Menu();
	void Initialize();
	void getRandom();
	int GetExecutionTime();
	void CalculateMachines();
	void PromoteResult(int*);
	void SetBestToCurrent(int*);
	int Execute();
	void InitBrutForce();
	void BrutForce(int);
	void Run();
	void Write();
	~FlowShop();
};

