#include "FileInfo.h"
using namespace std;

FileInfo::FileInfo( std::string path)
	: file( path, std::ios::binary ),
	length( std::filesystem::file_size( path ) ),
	bytes()
{
	bytes.resize( GetSize() );
	file.read( reinterpret_cast< char* >( bytes.data() ), static_cast< long >( length ) );
}

const std::byte* FileInfo::GetBytes()
{
	return bytes.data();
}

const uintmax_t FileInfo::GetSize()
{
	return length;
}
