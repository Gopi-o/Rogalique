#include "pch.h"
#include "FileMaster.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <Systems/Logger.h>

namespace Engine
{

	FileMaster* FileMaster::Instance()
	{
		static FileMaster instance;
		return &instance;
	}

	bool FileMaster::SaveText(const std::string& filename, const std::string& text)
	{
		try
		{
			std::filesystem::path filePath(filename);
			if (filePath.has_parent_path())
			{
				std::filesystem::create_directories(filePath.parent_path());
			}

			std::ofstream file(filename);
			if (!file.is_open())
			{
				LOG_ERROR("Failed to open file for writing: " + filename);
				return false;
			}
			file << text;
			file.close();
			return true;
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("Exception while saving text to file: ");
			return false;
		}
	}

	bool FileMaster::LoadText(const std::string& filename, std::string& outText)
	{
		try
		{
			if (!std::filesystem::exists(filename))
			{
				LOG_ERROR("File does not exist: " + filename);
				return false;
			}

			std::ifstream file(filename);
			if (!file.is_open())
			{
				LOG_ERROR("Failed to open file for reading: " + filename);
				std::cerr << "Failed to open file for reading: " << filename << std::endl;
				return false;
			}
			std::stringstream buffer;
			buffer << file.rdbuf();
			outText = buffer.str();
			file.close();
			return true;
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("Exception while loading text from file: ");
			return false;
		}
	}

	bool FileMaster::SaveBin(const std::string& filename, const std::vector<char>& data)
	{
		try
		{
			std::filesystem::path filePath(filename);
			if (filePath.has_parent_path())
			{
				std::filesystem::create_directories(filePath.parent_path());
			}

			std::ofstream file(filename, std::ios::binary);
			if (!file.is_open())
			{
				LOG_ERROR("Failed to open file for binary writing: " + filename);
				return false;
			}
			file.write(data.data(), data.size());
			file.close();
			return true;
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("Exception while loading text from file: ");
			return false;
		}
	}

	bool FileMaster::LoadBin(const std::string& filename, std::vector<char>& outData)
	{
		try
		{
			if (!std::filesystem::exists(filename))
			{
				LOG_ERROR("Binary file does not exist: " + filename);
				return false;
			}

			std::ifstream file(filename, std::ios::binary | std::ios::ate);
			if (!file.is_open())
			{
				LOG_ERROR("Failed to open file for binary reading: " + filename);
				return false;
			}

			std::streamsize size = file.tellg();
			file.seekg(0, std::ios::beg);

			outData.resize(size);
			if (file.read(outData.data(), size))
			{
				file.close();
				return true;
			}
			file.close();
			return false;
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("Exception while loading text from file: ");
			return false;
		}
	}
} // namespace Engine