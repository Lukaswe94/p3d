#pragma once
#include <string>
#include <filesystem>
#include <fstream>

class FileInfo
{

	public:

	FileInfo() = delete;
	explicit FileInfo(const std::string&);
	FileInfo( const FileInfo& );
	std::byte* getBytes();
	uintmax_t getSize() const;

	private:
	std::ifstream m_file;
	std::vector<std::byte> m_bytes;
	uintmax_t m_length;
};

