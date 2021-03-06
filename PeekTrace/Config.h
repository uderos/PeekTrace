#pragma once

#define g_CONFIG (Config::Instance())

class Config
{
	public:

		static Config & Instance();

		void ProcessCmdLine(const int argc, const char *argv[]);
		void Dump() const;

		const fs::path & GetInputFilePath() const;
		const fs::path & GetAlternateInputFilePath() const;
		bool GetVerboseFlag() const;
		bool GetDebugFlag() const;
		bool GetContinuousReadingFlag() const;
		bool IsExecutionRequired() const;
		std::chrono::milliseconds GetContinuousReadingIntervalMs() const;
		const std::vector<std::string> & GetANDFilters() const;
		const std::vector<std::string> & GetORFilters() const;
		const std::vector<std::string> & GetNOTFilters() const;

	private:

		static Config * m_instance_ptr;

		bool m_has_configuration;
		bool m_verbose_flag;
		bool m_debug_flag;
		bool m_keep_reading_flag;
		bool m_execution_required;
		std::uint64_t m_read_interval_ms;
		std::vector<std::string> m_and_filters;
		std::vector<std::string> m_or_filters;
		std::vector<std::string> m_not_filters;

		fs::path m_input_file_path;
		fs::path m_alternate_input_file_path;

		Config();
		virtual ~Config();

		std::string m_get_prog_name_and_version() const;
		void m_read_alternate_input_file_path();
};
