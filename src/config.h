#pragma once

// Libraries
#include <types.h>
#include <string>
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
