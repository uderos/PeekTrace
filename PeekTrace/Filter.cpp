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

	// If any of the AND rules fail, ignore the string
	for (const auto & re_ptr : m_and_filters)
		if (!std::regex_match(str, *re_ptr))
			return false;

	// If any of the NOT rules pass, ignore the string
	for (const auto & re_ptr : m_not_filters)
		if (std::regex_match(str, *re_ptr))
			return false;

	// If there are no OR rules, this string has to be displayed
	if (m_or_filters.empty())
		return true;

	// If at least one of the OR rules pass, this string has to be displayed
	for (const auto & re_ptr : m_or_filters)
		if (std::regex_match(str, *re_ptr))
			return true;

	// In any other case, the string is ignored
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
	const std::vector<std::string> & and_filter_str_list = g_CONFIG.GetANDFilters();
	const std::vector<std::string> & or_filter_str_list = g_CONFIG.GetORFilters();
	const std::vector<std::string> & not_filter_str_list = g_CONFIG.GetNOTFilters();

	for (const std::string & str_filter : and_filter_str_list)
		m_and_filters.emplace_back(m_create_regex(str_filter));

	for (const std::string & str_filter : or_filter_str_list)
		m_or_filters.emplace_back(m_create_regex(str_filter));

	for (const std::string & str_filter : not_filter_str_list)
		m_not_filters.emplace_back(m_create_regex(str_filter));
}

