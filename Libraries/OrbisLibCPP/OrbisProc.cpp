#include "stdafx.hpp"
#include "OrbisLib.hpp"
#include "OrbisProc.hpp"

OrbisProc::OrbisProc(OrbisLib* orbisLib)
{
	this->orbisLib = orbisLib;
}

OrbisProc::~OrbisProc()
{

}

int OrbisProc::GetList(char* IPAddr, int32_t* ProcCount, char* List)
{
	int Status = API_OK;

	API_Packet_s Packet;
	memset(&Packet, 0, sizeof(API_Packet_s));
	Packet.cmd = API_PROC_GET_LIST;

	Sockets* Sock;
	Status = orbisLib->API->CallLong(&Sock, IPAddr, &Packet);

	if (Status)
		return Status;

	if (!Sock->Receive((char*)ProcCount, sizeof(int)))
	{
		orbisLib->API->FinishCall(&Sock);

		return API_ERROR_FAIL;
	}

	if (!Sock->Receive(List, *ProcCount * sizeof(RESP_Proc)))
	{
		orbisLib->API->FinishCall(&Sock);

		return API_ERROR_FAIL;
	}

	orbisLib->API->FinishCall(&Sock);

	return API_OK;
}

int OrbisProc::Attach(char* IPAddr, char* ProcName)
{
	API_Packet_s Packet;
	memset(&Packet, 0, sizeof(API_Packet_s));
	Packet.cmd = API_PROC_ATTACH;
	strcpy_s(Packet.ProcName, ProcName);

	return orbisLib->API->Call(IPAddr, &Packet);
}

int OrbisProc::Detach(char* IPAddr, char* ProcName)
{
	API_Packet_s Packet;
	memset(&Packet, 0, sizeof(API_Packet_s));
	Packet.cmd = API_PROC_DETACH;
	strcpy_s(Packet.ProcName, ProcName);

	return orbisLib->API->Call(IPAddr, &Packet);
}

int OrbisProc::GetCurrent(char* IPAddr, RESP_Proc* Out)
{
	int Status = API_OK;

	API_Packet_s Packet;
	memset(&Packet, 0, sizeof(API_Packet_s));
	Packet.cmd = API_PROC_GET_CURRENT;

	Sockets* Sock;
	Status = orbisLib->API->CallLong(&Sock, IPAddr, &Packet);

	if (Status)
		return Status;

	if (!Sock->Receive((char*)Out, sizeof(RESP_Proc)))
	{
		orbisLib->API->FinishCall(&Sock);

		return API_ERROR_FAIL;
	}

	orbisLib->API->FinishCall(&Sock);

	return API_OK;
}

int OrbisProc::Read(char* IPAddr, uint64_t Address, size_t Len, char* Data)
{
	int Status = API_OK;

	API_Packet_s Packet;
	memset(&Packet, 0, sizeof(API_Packet_s));
	Packet.cmd = API_PROC_READ;
	Packet.Proc_RW.Address = Address;
	Packet.Proc_RW.len = Len;

	Sockets* Sock;
	Status = orbisLib->API->CallLong(&Sock, IPAddr, &Packet);

	if (Status)
		return Status;

	if (!Sock->Receive((char*)Data, (int)Len))
	{
		orbisLib->API->FinishCall(&Sock);

		return API_ERROR_FAIL;
	}

	orbisLib->API->FinishCall(&Sock);

	return API_OK;
}

int OrbisProc::Write(char* IPAddr, uint64_t Address, size_t Len, char* Data)
{
	int Status = API_OK;

	API_Packet_s Packet;
	memset(&Packet, 0, sizeof(API_Packet_s));
	Packet.cmd = API_PROC_WRITE;
	Packet.Proc_RW.Address = Address;
	Packet.Proc_RW.len = Len;

	Sockets* Sock;
	Status = orbisLib->API->CallLong(&Sock, IPAddr, &Packet);

	if (Status)
		return Status;

	if (!Sock->Send((char*)Data, (int)Len))
	{
		orbisLib->API->FinishCall(&Sock);

		return API_ERROR_FAIL;
	}

	if (!Sock->Send((char*)&Status, sizeof(int)))
	{
		orbisLib->API->FinishCall(&Sock);

		return API_ERROR_FAIL;
	}

	orbisLib->API->FinishCall(&Sock);

	return Status;
}

int OrbisProc::Kill(char* IPAddr, char* ProcName)
{
	API_Packet_s Packet;
	memset(&Packet, 0, sizeof(API_Packet_s));
	Packet.cmd = API_PROC_KILL;
	strcpy_s(Packet.ProcName, ProcName);

	return orbisLib->API->Call(IPAddr, &Packet);
}

int OrbisProc::LoadELF(char* IPAddr, char* ProcName, char* Data, size_t Len)
{
	int Status = API_OK;

	API_Packet_s Packet;
	memset(&Packet, 0, sizeof(API_Packet_s));
	Packet.cmd = API_PROC_LOAD_ELF;
	Packet.Proc_ELF.Len = Len;

	Sockets* Sock;
	Status = orbisLib->API->CallLong(&Sock, IPAddr, &Packet);

	if (Status)
		return Status;

	if (!Sock->Send((char*)Data, (int)Len))
	{
		orbisLib->API->FinishCall(&Sock);

		return API_ERROR_FAIL;
	}

	if (!Sock->Send((char*)&Status, sizeof(int)))
	{
		orbisLib->API->FinishCall(&Sock);

		return API_ERROR_FAIL;
	}

	orbisLib->API->FinishCall(&Sock);

	return Status;
}

//TODO:Implement RPC


int OrbisProc::LoadSPRX(char* IPAddr, char* Path, uint32_t Flags)
{
	API_Packet_s Packet;
	memset(&Packet, 0, sizeof(API_Packet_s));
	Packet.cmd = API_PROC_LOAD_SPRX;
	strcpy_s(Packet.Proc_SPRX.ModuleDir, Path);
	Packet.Proc_SPRX.Flags = Flags;

	return orbisLib->API->Call(IPAddr, &Packet);
}

int OrbisProc::UnloadSPRX(char* IPAddr, int32_t Handle, uint32_t Flags)
{
	API_Packet_s Packet;
	memset(&Packet, 0, sizeof(API_Packet_s));
	Packet.cmd = API_PROC_UNLOAD_SPRX;
	Packet.Proc_SPRX.hModule = Handle;
	Packet.Proc_SPRX.Flags = Flags;

	return orbisLib->API->Call(IPAddr, &Packet);
}

int OrbisProc::UnloadSPRX(char* IPAddr, char* Name, uint32_t Flags)
{
	API_Packet_s Packet;
	memset(&Packet, 0, sizeof(API_Packet_s));
	Packet.cmd = API_PROC_UNLOAD_SPRX_NAME;
	strcpy_s(Packet.Proc_SPRX.ModuleName, Name);
	Packet.Proc_SPRX.Flags = Flags;

	return orbisLib->API->Call(IPAddr, &Packet);
}

int OrbisProc::ReloadSPRX(char* IPAddr, char* Name, uint32_t Flags)
{
	API_Packet_s Packet;
	memset(&Packet, 0, sizeof(API_Packet_s));
	Packet.cmd = API_PROC_RELOAD_SPRX_NAME;
	strcpy_s(Packet.Proc_SPRX.ModuleName, Name);
	Packet.Proc_SPRX.Flags = Flags;

	return orbisLib->API->Call(IPAddr, &Packet);
}

int OrbisProc::ReloadSPRX(char* IPAddr, int32_t Handle, uint32_t Flags)
{
	API_Packet_s Packet;
	memset(&Packet, 0, sizeof(API_Packet_s));
	Packet.cmd = API_PROC_RELOAD_SPRX_HANDLE;
	Packet.Proc_SPRX.hModule = Handle;
	Packet.Proc_SPRX.Flags = Flags;

	return orbisLib->API->Call(IPAddr, &Packet);
}

int OrbisProc::GetLibraryList(char* IPAddr, int32_t* LibraryCount, char* Out)
{
	int Status = API_OK;

	API_Packet_s Packet;
	memset(&Packet, 0, sizeof(API_Packet_s));
	Packet.cmd = API_PROC_GET_LIST;

	Sockets* Sock;
	Status = orbisLib->API->CallLong(&Sock, IPAddr, &Packet);

	if (Status)
		return Status;

	int ModuleCount = 0;
	if (!Sock->Receive((char*)&ModuleCount, sizeof(int)))
	{
		orbisLib->API->FinishCall(&Sock);

		return API_ERROR_FAIL;
	}

	if (!Sock->Receive(Out, ModuleCount * sizeof(RESP_ModuleList)))
	{
		orbisLib->API->FinishCall(&Sock);

		return API_ERROR_FAIL;
	}

	orbisLib->API->FinishCall(&Sock);

	return API_OK;
}