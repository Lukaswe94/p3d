#pragma once
#include <string>
#include <filesystem>
#include <fstream>

class FileInfo
{

	public:

	FileInfo() = delete;
	FileInfo( std::string path );
	const std::byte* GetBytes();
	const uintmax_t GetSize();

	private:
	std::ifstream file;
	std::vector<std::byte> bytes;
	uintmax_t length;
};

