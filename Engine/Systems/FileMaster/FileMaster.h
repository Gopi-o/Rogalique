#pragma once
#include <string>
#include <vector>

namespace Engine
{
	class FileMaster
	{
	public:
		static FileMaster* Instance();

		FileMaster(const FileMaster&) = delete;
		FileMaster& operator=(const FileMaster&) = delete;

		bool SaveText(const std::string& filename, const std::string& text);
		bool LoadText(const std::string& filename, std::string& outText);

		bool SaveBin(const std::string& filename, const std::vector<char>& data);
		bool LoadBin(const std::string& filename, std::vector<char>& outData);

	private:
		FileMaster() = default;
		~FileMaster() = default;
	};
} // namespace Engine
