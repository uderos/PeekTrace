#include "stdafx.h"
#include "Config.h"
#include "Executor.h"
#include "Filter.h"
#include "InputFile.h"
#include "OutputFile.h"


Executor::Executor(const int argc, const char *argv[])
{
	g_CONFIG.ProcessCmdLine(argc, argv);
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
	constexpr auto SLEEP_TIME = std::chrono::milliseconds(500);
	const bool single_shot = (!g_CONFIG.GetTailFlag());

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
			std::this_thread::sleep_for(SLEEP_TIME);
		}

	} while (1);

}
