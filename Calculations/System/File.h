#pragma once
#include <filesystem>
#include <istream>
#include <fstream>

namespace File
{
	typedef std::filesystem::path Filepath;

	inline static bool GetAllFilesInDirectoryAndSubdirectories(const Filepath& directory, std::vector<Filepath>& filepaths, const bool includeSubdirectories = false)
	{
		filepaths.clear();

		if (std::filesystem::is_directory(directory) == false)
			return false;

		std::filesystem::recursive_directory_iterator itr = std::filesystem::recursive_directory_iterator(directory);

		for (const auto& file : itr)
		{
			filepaths.push_back(file.path());
		}

		return true;
	}

	inline static bool GetAllFilesInDirectory(const Filepath& directory, std::vector<Filepath>& filepaths)
	{
		filepaths.clear();

		if (std::filesystem::is_directory(directory) == false)
			return false;

		std::filesystem::directory_iterator itr = std::filesystem::directory_iterator(directory);

		for (const auto& file : itr)
		{
			filepaths.push_back(file.path());
		}

		return true;
	};

	inline static bool LoadFileIntoStringStream(const Filepath& filepath, std::stringstream& ss)
	{
		std::ifstream fileStream(filepath);

		if (fileStream.good() == false || fileStream.is_open() == false)
		{
			SDL_Log("Failed to open file for string stream loading.");
			throw std::runtime_error("Failed to open file for string stream loading.");
			return false;
		}

		ss.clear();
		ss << fileStream.rdbuf();
		return true;
	}
}

