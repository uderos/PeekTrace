#pragma once
class FnTimeLogger
{
public:
	FnTimeLogger(const std::string & fn_name);
	~FnTimeLogger();

private:

	using clock_t = std::chrono::high_resolution_clock;

	std::string m_fn_name;

	std::chrono::time_point<clock_t> m_start_time;
};

