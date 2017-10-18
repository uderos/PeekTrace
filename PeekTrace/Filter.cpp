#include "stdafx.h"
#include "Config.h"
#include "Filter.h"
#include "FnTimeLogger.h"

Filter::Filter()
{
	m_setup_filters();
}


Filter::~Filter()
{
}

bool Filter::test_string(const std::string & str) const
{
	// FnTimeLogger fnl("Filter::test_string");
	
	for (const auto & re_ptr : m_and_filters)
		if (!std::regex_match(str, *re_ptr))
			return false;

	if (m_or_filters.empty())
		return true;

	for (const auto & re_ptr : m_or_filters)
		if (std::regex_match(str, *re_ptr))
			return true;

	return false;
}

Filter::regex_ptr Filter::m_create_regex(const std::string & user_filter_str) const
{
	std::ostringstream oss;
	oss << "^.*" << user_filter_str << ".*"  << "[\r\n]*$";
	return std::make_unique<std::regex>(oss.str(), std::regex_constants::optimize);
}


void Filter::m_setup_filters()
{
	std::vector<std::string> and_filter_str_list, or_filter_str_list;
	g_CONFIG.GetFilters(and_filter_str_list, or_filter_str_list);

	for (const std::string str_filter : and_filter_str_list)
		m_and_filters.emplace_back(m_create_regex(str_filter));

	for (const std::string str_filter : or_filter_str_list)
		m_or_filters.emplace_back(m_create_regex(str_filter));
}

