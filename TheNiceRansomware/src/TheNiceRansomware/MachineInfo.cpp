#include "pch.h"
#include "MachineInfo.h"


using namespace std;


string getComputerName() {
	WCHAR name_buf[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
	string pc_name = "";
	if (!GetComputerName(name_buf, &size))
		pc_name = "null";
	else {
		char buf[MAX_COMPUTERNAME_LENGTH + 1];
		char defualt_char = ' ';
		WideCharToMultiByte(CP_ACP, 0, name_buf, -1, buf, MAX_COMPUTERNAME_LENGTH + 1, &defualt_char, NULL);
		pc_name = string(buf);
	}
	return pc_name;
}


string getUserName() {
	WCHAR name_buf[UNLEN + 1];
	DWORD size = UNLEN + 1;
	string user_name = "";
	if (!GetUserName(name_buf, &size))
		user_name = "null";
	else {
		char buf[UNLEN + 1];
		char defualt_char = ' ';
		WideCharToMultiByte(CP_ACP, 0, name_buf, -1, buf, UNLEN + 1, &defualt_char, NULL);
		user_name = string(buf);
	}
	return user_name;
}


string getPCAndUserNames() {
	string user_name = getUserName();
	string pc_name = getComputerName();
	return "COMPUTER: " + pc_name + "; USER: " + user_name;
}


string getOsVersion()
{

	DWORD dwType;
	char  prodcutName[255];
	char  releaseId[255];
	DWORD dwDataSize = 255;
	string product_name;
	string release_id;
	HKEY keyHandle;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &keyHandle) == ERROR_SUCCESS){
		if (!(RegQueryValueExA(keyHandle, "ProductName", NULL, &dwType, (LPBYTE)prodcutName, &dwDataSize) == ERROR_SUCCESS))
			product_name = "";
		else 
			product_name = string(prodcutName);

		if (!(RegQueryValueExA(keyHandle, "ReleaseId", NULL, &dwType, (LPBYTE)releaseId, &dwDataSize) == ERROR_SUCCESS))
			release_id = "";
		else
			release_id = string(releaseId);

	}
	RegCloseKey(keyHandle);
	return product_name + " " + release_id;
}


string getSystemInfo() {
	string processor_arch;
	string processor_type;
	string number_of_processors;
	string system_info;
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);

	switch (siSysInfo.wProcessorArchitecture) {
	case 9: processor_arch = "x64";
		break;
	case 5: processor_arch = "ARM";
		break;
	case 12: processor_arch = "ARM64";
		break;
	case 6: processor_arch = "Intel Itanium-based ";
		break;
	case 0: processor_arch = "x86";
		break;
	default: processor_arch = "unkown";
		break;
	}

	switch (siSysInfo.dwProcessorType) {
	case 386: processor_type = "INTEL_386";
		break;
	case 486: processor_type = "INTEL_486";
		break;
	case 586: processor_type = "INTEL_PENTIUM";
		break;
	case 2200: processor_type = "INTEL_IA64 ";
		break;
	case 8664: processor_type = "AMD_X8664";
		break;
	default: processor_type = "unkown";
		break;
	}

	number_of_processors = to_string(siSysInfo.dwNumberOfProcessors);
	system_info = processor_type + ";" + processor_arch + ";" + number_of_processors + " cores";
	return system_info;
}
