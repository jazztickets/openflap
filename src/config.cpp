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
#include <config.h>
#include <constants.h>
#include <sstream>
#include <fstream>
#include <SDL_filesystem.h>

// Globals
_Config Config;

// Initializes the config system
void _Config::Init(const std::string &ConfigFile) {

	// Create config path
	char *PrefPath = SDL_GetPrefPath("", "openflap");
	if(PrefPath) {
		ConfigPath = PrefPath;
		SDL_free(PrefPath);
	}
	else {
		throw std::runtime_error("Cannot create config path!");
	}

	this->ConfigFile = ConfigPath + ConfigFile;

	// Load defaults
	SetDefaults();

	// Load config
	Load();
}

// Closes the config system
void _Config::Close() {
}

// Set defaults
void _Config::SetDefaults() {

	ScreenWidth = DEFAULT_SCREEN_WIDTH;
	ScreenHeight = DEFAULT_SCREEN_HEIGHT;
	Fullscreen = DEFAULT_FULLSCREEN;
	Vsync = DEFAULT_VSYNC;
	AudioEnabled = DEFAULT_AUDIOENABLED;

	SoundVolume = DEFAULT_SOUNDVOLUME;
	MusicVolume = DEFAULT_MUSICVOLUME;
}

// Load the config file
void _Config::Load() {

	// Open file
	std::ifstream In(ConfigFile.c_str());
	if(!In.is_open()) {
		Save();
		return;
	}

	// Read data into map
	Map.clear();
	char Buffer[256];
	while(In) {

		In.getline(Buffer, 256);
		if(In.good()) {
			std::string Line(Buffer);
			std::size_t Pos = Line.find_first_of('=');
			if(Pos != std::string::npos) {
				std::string Field = Line.substr(0, Pos);
				std::string Value = Line.substr(Pos+1, Line.size());

				Map[Field] = Value;
			}
		}
	}
	In.close();

	// Read config
	GetValue("screen_width", ScreenWidth);
	GetValue("screen_height", ScreenHeight);
	GetValue("fullscreen", Fullscreen);
	GetValue("vsync", Vsync);
	GetValue("audio_enabled", AudioEnabled);
	GetValue("sound_volume", SoundVolume);
	GetValue("music_volume", MusicVolume);

}

// Save variables to the config file
void _Config::Save() {

	std::ofstream Out(ConfigFile.c_str());
	if(!Out.is_open()) {
		return;
	}

	// Write variables
	Out << "screen_width=" << ScreenWidth << std::endl;
	Out << "screen_height=" << ScreenHeight << std::endl;
	Out << "fullscreen=" << Fullscreen << std::endl;
	Out << "vsync=" << Vsync << std::endl;
	Out << "audio_enabled=" << AudioEnabled << std::endl;
	Out << "sound_volume=" << SoundVolume << std::endl;
	Out << "music_volume=" << MusicVolume << std::endl;

	Out.close();
}
