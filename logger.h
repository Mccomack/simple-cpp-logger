#pragma once

#include <iostream>
#include <vector>
#include <format>
//#include <time.h>
#include <ctime>
#include <string>

using std::string;
using std::vector;

struct _Log {
	time_t timestamp;
	string type;
	string log;

	bool operator==(const _Log& other) const;

	_Log(time_t timestamp, string type, string log);
};

class Logger {
private:
	string name;

	vector<_Log*>* Logs;
public:
	Logger(string name);

	void Log(string logType, string log);
	vector<_Log*>* GetLogs();

	~Logger();
};

string tmToStr(std::tm* TM) {
	string hour = std::to_string(TM->tm_hour);
	string min = std::to_string(TM->tm_min);
	string sec = std::to_string(TM->tm_sec);

	if (hour.size() == 1) {
		hour.insert(0, "0");
	}

	if (min.size() == 1) {
		min.insert(0, "0");
	}

	if (sec.size() == 1) {
		sec.insert(0, "0");
	}

	return std::format("{}:{}:{}", TM->tm_hour, TM->tm_min, TM->tm_sec);
}

bool _Log::operator==(const _Log& other) const {
	return (this->timestamp == other.timestamp) && (this->type == other.type) && (this->log == other.log);
}

_Log::_Log(time_t timestamp, string type, string log) {
	this->timestamp = timestamp;
	this->type = type;
	this->log = log;
}

Logger::Logger(string name) {
	vector<_Log*>* Logs = new vector<_Log*>;

	this->name = name;
	this->Logs = Logs;
}

void Logger::Log(string logType, string log) {
	time_t curTime;
	time(&curTime);

	_Log* newLog = new _Log(curTime, logType, log);

	this->Logs->push_back(newLog);

	std::tm* localTM = new std::tm;
	localtime_s(localTM, &curTime);

	string nowTime = std::format("[{}]", tmToStr(localTM));
	string logEnvs = std::format("[{}/{}]", this->name, logType);

	string formatedLog = std::format("{} {} {}", nowTime, logEnvs, log);
	
	std::cout << formatedLog << std::endl;

	delete localTM;
}

vector<_Log*>* Logger::GetLogs() {
	vector<_Log*>* cloned = new vector<_Log*>;
	vector<_Log*>* _logs  = this->Logs;

	for (int i = 0; i < _logs->size(); i++) {
		_Log* _log = _logs->at(i);

		_Log* _log_cloned = new _Log(_log->timestamp, _log->type, _log->log);

		cloned->push_back(_log_cloned);
	}

	return cloned;
}

Logger::~Logger() {
	vector<_Log*>* _logs = this->Logs;

	for (int i = 0; i < _logs->size(); i++) {
		_Log* _log = _logs->at(i);

		delete _log;
	}

	delete this->Logs;
}