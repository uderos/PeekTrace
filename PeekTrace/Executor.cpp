#include "stdafx.h"
#include "Config.h"
#include "Executor.h"
#include "Filter.h"
#include "InputFile.h"
#include "OutputFile.h"


Executor::Executor(const int argc, const char *argv[], const bool * abort_flag_ptr) :
	m_abort_flag_ptr(abort_flag_ptr)
{
	g_CONFIG.ProcessCmdLine(argc, argv);
}

Executor::Executor(const int argc, const char *argv[]) :
	Executor(argc, argv, nullptr)
{
}

Executor::~Executor()
{
}

bool Executor::Run()
{
	bool rc = true;

	if (g_CONFIG.IsExecutionRequired())
	{
		rc = m_execute();
	}
		
	return rc;
}

bool Executor::m_execute()
{
	const fs::path infile_path = g_CONFIG.GetInpoutFilePath();
	
	InputFile infile(infile_path);
	infile.openFile();

	if (!infile.is_open())
	{
		std::cout << "Unable to open input file: " << infile_path.string() << std::endl;
		return false;
	}

	if (g_CONFIG.GetTailFlag())
		infile.MoveToEndOfFile();

	OutputFile outfile(std::cout);
	Filter filter;

	m_read_and_log(infile, outfile, filter);

	return true;
}

void Executor::m_read_and_log(
	InputFile & infile, 
	OutputFile & outfile,
	Filter & filter)
{
	constexpr auto SLEEP_TIME = std::chrono::milliseconds(1000);
	const bool single_shot = (!g_CONFIG.GetTailFlag());
	const bool close_file_on_idle = g_CONFIG.GetCloseFileOnIdleFlag();

	std::string line;
	do
	{
		if (infile.getline(line))
		{
			if (filter.test_string(line))
				outfile.write_string(line);
		}
		else if (single_shot)
		{
			return;
		}
		else
		{
			if (close_file_on_idle)
				infile.closeFile();

			std::this_thread::sleep_for(SLEEP_TIME);

			if (close_file_on_idle)
				infile.openFile();
		}

	} while (!(*m_abort_flag_ptr));
}
