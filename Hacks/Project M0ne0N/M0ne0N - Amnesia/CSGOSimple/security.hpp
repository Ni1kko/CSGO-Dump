#pragma once
#include <iostream>
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "Advapi32.lib")
#include <urlmon.h>
#include <WinInet.h>
#include <iomanip>

using namespace std;

static int Negr(string host, string request, string ver)
{
	HINTERNET hIntSession = InternetOpenA("", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!hIntSession) return 0;

	HINTERNET hHttpSession = InternetConnectA(hIntSession, host.c_str(), 80, 0, 0, INTERNET_SERVICE_HTTP, 0, NULL);
	if (!hHttpSession) return 0;

	HINTERNET hHttpRequest = HttpOpenRequestA(hHttpSession, "GET", request.c_str(), 0, 0, 0, INTERNET_FLAG_RELOAD, 0);
	if (!hHttpSession) return 0;

	char* szHeaders = ("Content-Type: text/html\r\nUser-Agent: License");
	char szRequest[1024] = { 0 };

	if (!HttpSendRequestA(hHttpRequest, szHeaders, strlen(szHeaders), szRequest, strlen(szRequest)))
		return 0;

	CHAR szBuffer[1024] = { 0 };
	DWORD dwRead = 0;

	string request_data = "";

	while (InternetReadFile(hHttpRequest, szBuffer, sizeof(szBuffer) - 1, &dwRead) && dwRead)
		request_data.append(szBuffer, dwRead);

	InternetCloseHandle(hHttpRequest);
	InternetCloseHandle(hHttpSession);
	InternetCloseHandle(hIntSession);

	if (request_data == ver.c_str())
		return 22971;
	else
		return 24696;
}