// PeekTrace.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Config.h"
#include "Executor.h"


enum eReturnCode
{
	RC_OKAY = 0,
	RC_ERROR = 1
};

static std::weak_ptr<Executor> f_executor_remote_ptr;

static BOOL WINAPI f_ConsoleHandler(DWORD signal) {

	if (signal == CTRL_C_EVENT)
	{
		if (auto executor_ptr = f_executor_remote_ptr.lock())
		{
			std::cout << "Ctrl-C handled" << std::endl;
			executor_ptr->Abort();
		}
	}

	return TRUE;
}


int main(const int argc, const char *argv[])
{
	int rc = RC_OKAY;

	if (!SetConsoleCtrlHandler(f_ConsoleHandler, TRUE)) 
	{
		std::cout << "ERROR: Could not set control handler" << std::endl;
		return RC_ERROR;
	}


	try
	{
		auto executor_ptr = std::make_shared<Executor>(argc, argv);
		f_executor_remote_ptr = executor_ptr;
		executor_ptr->Run();
	}
	catch (std::exception & exh)
	{
		std::cout << "\nC++EXCEPTION: " << exh.what() << std::endl;
		rc = RC_ERROR;
	}
	catch (...)
	{
		std::cout << "\nEXCEPTION: " << std::endl;
		rc = RC_ERROR;
	}

	return 0;
}


