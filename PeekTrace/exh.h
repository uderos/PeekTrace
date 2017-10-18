#pragma once

namespace exh
{
	inline void g_throw_runtime_error_str(
		const char * const file_name,
		const int file_line,
		const char * const function_name,
		const std::string & argument)
	{
		std::ostringstream oss;
		oss << "FUNCTION:" << function_name
			<< " FILE:" << file_name
			<< " LINE:" << file_line;

		if (!argument.empty())
			oss << " ARG=" << argument;

		throw std::runtime_error(oss.str());
	}

	template <typename ARG_T>
	void g_throw_runtime_error(
		const char * const file_name,
		const int file_line,
		const char * const function_name,
		const ARG_T & argument)
	{
		std::ostringstream oss;
		oss << " ARG=" << argument;
		g_throw_runtime_error_str(file_name, file_line, function_name, oss.str());
	}
}

#define THROW_RUNTIME_ERROR_ARG(ARG)	\
	exh::g_throw_runtime_error(__FILE__, __LINE__, __FUNCTION__, (ARG))

#define THROW_RUNTIME_ERROR	\
	exh::g_throw_runtime_error_str(__FILE__, __LINE__, __FUNCTION__, std::string())
