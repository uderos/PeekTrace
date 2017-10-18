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

static void f_run(const int argc, const char *argv[])
{
	Executor executor(argc, argv);
	executor.Run();
}



int main(const int argc, const char *argv[])
{
	int rc = RC_OKAY;

	try
	{
		f_run(argc, argv);
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

//#include <stdio.h>
//#ifdef WIN32
//#include <windows.h>
//#else
//#include <signal.h>
//#endif
//
//#ifdef WIN32
//BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
//{
//	if (fdwCtrlType == CTRL_C_EVENT)
//	{
//		printf("Got ^C ...\n");
//		return TRUE;
//	}
//	return FALSE;
//}
//#else 
//void CtlCHandler(int nSignal)
//{
//	printf("Got ^C(%d) ...\n", nSignal);
//	exit(0);
//}
//#endif
//
//int main(int argc, char *argv[], char *envp[])
//{
//#ifdef WIN32
//	SetConsoleCtrlHandler(CtrlHandler, TRUE);
//#else
//	struct sigaction  sigact;
//	memset(&sigact, 0, sizeof(struct sigaction));
//	sigact.sa_handler = CtlCHandler;
//	sigaction(SIGINT, &sigact, NULL);
//	sigaction(SIGTERM, &sigact, NULL);
//#endif
//	/* .... */
//	return 0;
//}



