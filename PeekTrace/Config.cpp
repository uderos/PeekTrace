#include "stdafx.h"
#include "Config.h"
#include "exh.h"


namespace po = boost::program_options;

static const std::string f_PROGRAM_NAME("PeekTrace");
static const std::string f_PROGRAM_VERSION("1.01");

static constexpr char f_ALTERNATE_INPUT_FILE_PATH_ENV[] = "PEEKTRACE_FILEPATH";

static const std::string DEFAULT_INPUT_FILE{
	(R"(C:\Program Files (x86)\IL\ACL-TOP\backup\CurrentTraceFile.txt)")
};

Config * Config::m_instance_ptr = nullptr;

Config::Config() :
	m_has_configuration(false),
	m_verbose_flag(false),
	m_debug_flag(false),
	m_single_shot_flag(false),
	m_execution_required(true)
{
}


Config::~Config()
{
}

Config & Config::Instance()
{
	if (!m_instance_ptr)
		m_instance_ptr = new Config;
	return (*m_instance_ptr);
}

void Config::ProcessCmdLine(const int argc, const char *argv[])
{
	const std::string desc_str = m_get_prog_name_and_version() + std::string(" - Allowed options");
	po::options_description desc(desc_str);

	desc.add_options()
		("help,h", "Produce this help message")
		("verbose,v", "Verbose logging on stdout")
		("debug,d", "Enable debugging features")
		("file,f", po::value<std::string>()->default_value(DEFAULT_INPUT_FILE), "Input file")
		("single-shot,s", "Process the entire file once")
		("and,a", po::value<std::vector<std::string>>(&m_and_filters), "AND filter")
		("or,o", po::value<std::vector<std::string>>(&m_or_filters), "OR filter")
		("not,x", po::value<std::vector<std::string>>(&m_not_filters), "NOT filter")
		("am", "Select only AM messages")
		("cm", "Select only CM messages")
		("cl", "Select only CL messages")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help"))
	{
		std::cout << desc << std::endl;
		m_execution_required = false;
	}
	else
	{
		po::notify(vm);

		m_verbose_flag = (vm.count("verbose") > 0);
		m_debug_flag = (vm.count("debug") > 0);
		m_input_file_path = fs::path(vm["file"].as<std::string>());
		m_single_shot_flag = (vm.count("single-shot") > 0);
		m_read_alternate_input_file_path();

		if (vm.count("am") > 0) m_and_filters.push_back(R"(\|A\|)");
		if (vm.count("cm") > 0) m_and_filters.push_back(R"(\|C\|)");
		if (vm.count("cl") > 0) m_and_filters.push_back(R"(\|L\|)");

		if (m_verbose_flag)
			Dump();
	}

	m_has_configuration = true;
}

void Config::Dump() const
{
	std::cout << "Configuration:"
		<< "\n\t input-file=" << m_input_file_path
		<< "\n\t verbose=" << m_verbose_flag
		<< "\n\t debug=" << m_debug_flag
		<< "\n\t single-shot=" << m_single_shot_flag;

	for (const auto s : m_and_filters)
		std::cout << "\n\t AND " << s;

	for (const auto s : m_or_filters)
		std::cout << "\n\t OR " << s;

	for (const auto s : m_not_filters)
		std::cout << "\n\t NOT " << s;

	std::cout << std::endl;
}

const fs::path & Config::GetInputFilePath() const
{
	if (!m_has_configuration) THROW_RUNTIME_ERROR;
	return m_input_file_path;
}

const fs::path & Config::GetAlternateInputFilePath() const
{
	if (!m_has_configuration) THROW_RUNTIME_ERROR;
	return m_alternate_input_file_path;
}

bool Config::GetVerboseFlag() const
{
	if (!m_has_configuration) THROW_RUNTIME_ERROR;
	return m_verbose_flag;
}

bool Config::GetDebugFlag() const
{
	if (!m_has_configuration) THROW_RUNTIME_ERROR;
	return m_debug_flag;
}

bool Config::GetSingleShotFlag() const
{
	if (!m_has_configuration) THROW_RUNTIME_ERROR;
	return m_single_shot_flag;
}

bool Config::IsExecutionRequired() const
{
	if (!m_has_configuration) THROW_RUNTIME_ERROR;
	return m_execution_required;
}

const std::vector<std::string> & Config::GetANDFilters() const
{
	if (!m_has_configuration) THROW_RUNTIME_ERROR;
	return m_and_filters;
}

const std::vector<std::string> & Config::GetORFilters() const
{
	if (!m_has_configuration) THROW_RUNTIME_ERROR;
	return m_or_filters;
}

const std::vector<std::string> & Config::GetNOTFilters() const
{
	if (!m_has_configuration) THROW_RUNTIME_ERROR;
	return m_not_filters;
}

std::string Config::m_get_prog_name_and_version() const
{
	std::ostringstream oss;
	oss << f_PROGRAM_NAME << " v" << f_PROGRAM_VERSION;
	return oss.str();
}


void Config::m_read_alternate_input_file_path()
{
	m_alternate_input_file_path.clear();

	size_t requiredSize = 0;
	getenv_s(&requiredSize, NULL, 0, f_ALTERNATE_INPUT_FILE_PATH_ENV);

	if (requiredSize)
	{
		auto membuff_ptr = std::make_unique<char[]>(requiredSize);
		getenv_s(&requiredSize, membuff_ptr.get(), requiredSize, f_ALTERNATE_INPUT_FILE_PATH_ENV);
		m_alternate_input_file_path = fs::path(membuff_ptr.get());
	}
}



