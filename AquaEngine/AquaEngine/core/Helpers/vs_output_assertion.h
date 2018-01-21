#pragma once

#include <sstream>

enum log_type
{
	TODO_msg,
	INFO_msg,
	ERROR_msg,
};

class Logger
{
public:
	Logger() = default;
	~Logger() = default;

	template<typename T>
	void log(T _first)
	{
		container << _first << L"\n\n";
		OutputDebugStringW(container.str().c_str());
	}

	template<typename T, typename... Args>
	void log(T _first, Args... _args)
	{
		container << _first;
		log(_args...);
	}

	template<typename T, typename... Args>
	void log(log_type _type, T _first, Args... _args)
	{
		switch (_type)
		{
		case TODO_msg:
			container << L"\n[TODO] :\n";
			break;
		case INFO_msg:
			container << L"\n{INFO} :\n";
			break;
		case ERROR_msg:
			container << L"\n!!!ERROR!!! :\n";
			break;
		default:
			break;
		}

		log(_first, _args...);
	}

private:
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	std::wstringstream container;

};

template<typename... Args>
void vs_log(bool _condition, log_type _type, Args... _args)
{
#ifdef _DEBUG
	if (_condition)
	{
		Logger logger;
		logger.log(_type, _args...);
	}
#endif // _DEBUG

}

/*#define log_todo(str, args) \
Logger logger; [logger.log(TODO_msg, str, args);
#define log_todo(str) \
Logger logger; logger.log(TODO_msg, str, L"");

#define log_info(str, args) \
Logger logger; logger.log(INFO_msg, str, args);
#define log_info(str) \
Logger logger; logger.log(INFO_msg, str, L"");


#define GET_MACRO(_1, _2, NAME, ...) NAME
#define LOG_ERROR(...) GET_MACRO(__VA_ARGS__, log_error_args, log_error)(__VA_ARGS__)

#define log_error_args(str, args) \
Logger logger; logger.log(ERROR_msg, str, args);
#define log_error(str) \
Logger logger; logger.log(ERROR_msg, str, L"");

#define GET_MACRO(_1, _2, _3, NAME, ...) NAME
#define LOG_ERROR_IF(...) GET_MACRO(__VA_ARGS__, log_error_if_args, log_error_if)(__VA_ARGS__)

#define log_error_if_args(condition, str, args) \
if(!condition) { Logger logger; logger.log(ERROR_msg, str, args); }
#define log_error_if(condition, str) \
if(!condition) { Logger logger; logger.log(ERROR_msg, str, L""); }
*/