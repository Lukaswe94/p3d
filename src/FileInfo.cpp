#include "FileInfo.h"

FileInfo::FileInfo(const std::string& path)	:
	m_file( path, std::ios::binary ),
	m_length( std::filesystem::file_size( path ) )
{
	m_bytes.resize( getSize() );
	m_file.read( reinterpret_cast< char* >( m_bytes.data() ), static_cast< long >( m_length ) );
}

FileInfo::FileInfo(const FileInfo& other):  m_bytes(other.m_bytes), m_length( other.getSize() )
{
}


std::byte* FileInfo::getBytes()
{
	return m_bytes.data();
}

uintmax_t FileInfo::getSize() const
{
	return m_length;
}
