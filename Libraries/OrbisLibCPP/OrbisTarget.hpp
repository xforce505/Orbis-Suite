#pragma once
class OrbisLib;

#include "Export.hpp"
#include "sqlite3.h"

#define MAX_TARGETS 20

class EXPORT OrbisTarget
{
private:
	OrbisLib* orbisLib;

	static unsigned char DefaultDB[12288];
	const char* DBname = "Orbis-User-Data.db";
	char DBPath[0x200];
	bool Running;

	bool OpenDatabase(sqlite3** db);
	bool DoesTargetExist(const char* TargetName);
	bool GetDefaultTarget(DB_TargetInfo* Out);

	//Depreciated
	static DWORD WINAPI FileWatcherThread(LPVOID Params);

public:
	bool AutoLoadPayload;
	bool StartOnBoot;
	DB_TargetInfo DefaultTarget;

	DB_TargetInfo Targets[MAX_TARGETS];
	int TargetCount;

	OrbisTarget(OrbisLib* OrbisLib);
	~OrbisTarget();

	//Target Management
	bool GetTarget(const char* TargetName, DB_TargetInfo* Out);
	bool SetTarget(const char* TargetName, DB_TargetInfo In);
	bool UpdateTargetExtInfo(int Target);
	bool DeleteTarget(const char* TargetName);
	bool NewTarget(DB_TargetInfo In);

	//Target List
	int GetTargetCount();
	bool GetTargetList(DB_TargetInfo Out[]);

	//Settings
	void UpdateSettings();
	bool SetAutoLoadPayload(bool Value);
	bool SetStartOnBoot(bool Value);
	bool SetDefaultTarget(const char* Name);

	//API Calls
	int GetInfo(char* IPAddr, RESP_TargetInfo* TargetInfo);
};
