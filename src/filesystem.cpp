/******************************************************************************
* openflap
* Copyright (C) 2014  Alan Witkowski
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
#include <filesystem.h>
#include <cstdlib>

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#else
	#include <sys/stat.h>
	#include <dirent.h> 
#endif

// Create a directory
void _FileSystem::CreateDir(const std::string &Path) {
	#ifdef _WIN32
		CreateDirectory(Path.c_str(), NULL);
	#else
		mkdir(Path.c_str(), S_IRWXU | S_IXGRP | S_IRGRP | S_IXOTH | S_IROTH);
	#endif
}

// Get home path
std::string _FileSystem::GetHomePath() {
	#ifdef _WIN32
		std::string Path;

		char *Buffer = 0;
		size_t Count = 0;
		if (_dupenv_s(&Buffer, &Count, "AppData") == 0) {
			Path = Buffer;
			free(Buffer);
		}
	
		return Path;
	#else
		return getenv("HOME");
	#endif
}

#include <cstdio>

// Get a list of files in a directory
void _FileSystem::GetFiles(const std::string &Path, std::vector<std::string> &Contents) {
	
	#ifdef _WIN32

		// Get file handle
		WIN32_FIND_DATA FindFileData;
		HANDLE FindHandle = FindFirstFile((Path + "*").c_str(), &FindFileData);
		if(FindHandle == INVALID_HANDLE_VALUE) {
			return;
		}

		// Add first value
		if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			Contents.push_back(FindFileData.cFileName);

		// Get the other files
		while(FindNextFile(FindHandle, &FindFileData)) {
			if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				Contents.push_back(FindFileData.cFileName);
		}

		// Close
		FindClose(FindHandle);
	#else
	
		DIR *Directory;
		struct dirent *Entry;
		Directory = opendir(Path.c_str());
		if(Directory) {
			while((Entry = readdir(Directory)) != NULL) {
				if(Entry->d_type == DT_REG) {
					Contents.push_back(Entry->d_name);
				}
			}

			closedir(Directory);
		}

	#endif
}
