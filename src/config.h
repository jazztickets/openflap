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
#pragma once

// Libraries
#include <stdint.h>
#include <string>
#include <sstream>
#include <map>

// Load/save config file
class _Config {

	public:

		void Init(const std::string &ConfigFile);
		void Close();

		void Load();
		void Save();
		void SetDefaults();

		const std::string &GetConfigPath() { return ConfigPath; }

		// Graphics
		int ScreenWidth, ScreenHeight;
		int Vsync;
		bool Fullscreen;

		// Audio
		bool AudioEnabled;
		float SoundVolume;
		float MusicVolume;

	private:

		template <typename Type>
		void GetValue(const std::string &Field, Type &Value) {
			MapIteratorType MapIterator = Map.find(Field);
			if(MapIterator != Map.end()) {
				std::stringstream Stream(MapIterator->second);
				Stream >> Value;
			}
		}

		// State
		std::string ConfigFile;
		std::string ConfigPath;
		std::map<std::string, std::string> Map;
		typedef std::map<std::string, std::string>::iterator MapIteratorType;
};

extern _Config Config;
