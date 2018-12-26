#pragma once
#pragma comment(lib, "user32.lib")
#include <iostream>
#include <Windows.h>
#include <string>
#include "Lmcons.h"


using namespace std;


string getSystemInfo();
string getComputerName();
string getUserName();
string getPCAndUserNames();
string getOsVersion();
