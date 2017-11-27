#pragma once

class InputFile;
class OutputFile;
class Filter;

class Executor
{
public:
	Executor(const int argc, const char *argv[]);
	Executor(const int argc,
			 const char *argv[],
			 const bool * abort_flag_ptr);
	virtual ~Executor();

	bool Run();

private:
	const bool * const m_abort_flag_ptr;

	bool m_execute();
	void m_read_and_log(
		InputFile & infile, 
		OutputFile & outfile,
		Filter & filter);
	fs::path m_GetInputFilePath() const;
};

