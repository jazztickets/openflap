#pragma once

// Libraries
#include <string>
#include <vector>

// Class for managing file systems
class _FileSystem {

	public:
	
		static void CreateDir(const std::string &Path);
		static std::string GetHomePath();
		static void GetFiles(const std::string &Path, std::vector<std::string> &Contents);
		
	private:
	
};
