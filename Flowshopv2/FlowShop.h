#pragma once
#include <vector>
#include <string>

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
	bool bInitialized;
	int iBestExecutionTime;
	int iMachineNumber;
	int iJobsNumber;
	double dTime;
	double PCFreq;
	__int64 CounterStart;
	bool* visited;
	int iCountVisited;
public:
	void StartCounter();
	double GetCounter();
	FlowShop();
	int GetExecutionTime();
	void CalculateMachines();
	void PromoteResult(int*);
	void SetBestToCurrent(int*);
	int SimulatedAnnealing();
	void InitBrutForce();
	void BrutForce(int);
	void Run(char**);
	void Write(std::string arg);
	bool Load(std::string);
	~FlowShop();
};

