#pragma once
class Filter
{
public:
	Filter();
	virtual ~Filter();
	bool test_string(const std::string & str) const;

private:

	using regex_ptr = std::unique_ptr<std::regex>;
	using regex_list = std::vector<regex_ptr>;

	regex_list m_and_filters;
	regex_list m_or_filters;
	regex_list m_not_filters;


	void m_setup_filters();
	regex_ptr m_create_regex(const std::string & user_filter_str) const;
	
};

