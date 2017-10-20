#pragma once

class InputFile;
class OutputFile;
class Filter;

class Executor
{
public:
	Executor(const int argc, const char *argv[]);
	virtual ~Executor();

	bool Run();

private:
	bool m_execute();
	void m_read_and_log(
		InputFile & infile, 
		OutputFile & outfile,
		Filter & filter);
};

