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

	void Abort();

private:
	volatile std::atomic_bool m_abort_flag;

	bool m_execute();
	void m_read_and_log(
		InputFile & infile, 
		OutputFile & outfile,
		Filter & filter);
	fs::path m_GetInputFilePath() const;
	void m_sleep(const std::chrono::milliseconds & sleep_time_ms) const;
};

