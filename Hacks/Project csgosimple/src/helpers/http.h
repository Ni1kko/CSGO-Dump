#pragma once
#include <map>
#include <vector>
#include <string>
#include <functional>

namespace http
{
	void get(const char* url, std::function<void(std::string)> callback);
	void get(const char* url, std::map<std::string, std::string> headers, std::function<void(std::string)> callback);
	void post(const char* url, std::map<std::string, std::string> params, std::function<void(std::string)> callback);
	void post(const char* url, std::map<std::string, std::string> params, std::map<std::string, std::string> headers, std::function<void(std::string)> callback);
}
