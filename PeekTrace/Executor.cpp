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
	const fs::path infile_path = m_GetInputFilePath();
	
	InputFile infile(infile_path);
	infile.openFile();

	if (!infile.is_open())
	{
		std::cout << "Unable to open input file: " << infile_path.string() << std::endl;
		return false;
	}

	if (g_CONFIG.GetSingleShotFlag())
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
	const bool single_shot = g_CONFIG.GetSingleShotFlag();

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

	} while (!(*m_abort_flag_ptr));
}

fs::path Executor::m_GetInputFilePath() const
{
	fs::path infile_path = g_CONFIG.GetInputFilePath();
	if (!fs::exists(infile_path))
	{
		fs::path alternate_infile_path = g_CONFIG.GetAlternateInputFilePath();
		if (fs::exists(alternate_infile_path))
			infile_path = alternate_infile_path;
	}

	return infile_path;
}

