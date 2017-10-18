#include "stdafx.h"
#include "FnTimeLogger.h"


FnTimeLogger::FnTimeLogger(const std::string & fn_name) :
	m_fn_name(fn_name),
	m_start_time(clock_t::now())
{
}

FnTimeLogger::~FnTimeLogger()
{
	const auto exec_time = clock_t::now() - m_start_time;
	const auto exec_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(exec_time);
	std::cout << "FUNCTION " << m_fn_name
		<< " ExecutionTimeMs=" << exec_time_ms.count()
		<< std::endl;
}
