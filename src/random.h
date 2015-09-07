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

#include <stdint.h>

class _Random {

	public:

		_Random();
		_Random(uint32_t Seed);

		// Set initial seed
		void SetSeed(uint32_t Seed);

		// Random functions
		double Generate();
		uint32_t Generate(uint32_t Count);
		int GenerateRange(int Min, int Max);
		uint32_t GenerateRange(uint32_t Min, uint32_t Max);
		double GenerateRange(double Min, double Max);

	private:

		// Base function
		uint32_t GenerateRandomInteger();

		// Seeds
		uint32_t Q[1024];
};

// Constructor
inline _Random::_Random() {

	SetSeed(0);
}

// Constructor
inline _Random::_Random(uint32_t Seed) {

	SetSeed(Seed);
}

// Sets the seed for the generator
inline void _Random::SetSeed(uint32_t Seed) {

	for(uint32_t i = 0; i < 1024; i++) {
		Seed ^= Seed << 13;
		Seed ^= Seed >> 17;
		Seed ^= Seed << 5;
		Q[i] = Seed;
	}
}

// Generates a random integer
inline uint32_t _Random::GenerateRandomInteger() {
	static uint32_t c = 8471623, i = 1023;
	uint64_t t, a = 123471786LL;
	uint32_t x, r = 0xfffffffe;

	i = (i + 1) & 1023;
	t = a * Q[i] + c;
	c = t >> 32;
	x = (uint32_t)(t + c);
	if(x < c) {
		x++;
		c++;
	}

	return Q[i] = r - x;
}

// Generates a random number [0, 1)
inline double _Random::Generate() {

	return GenerateRandomInteger() / 4294967296.0;
}

// Generates a random number [0, Count-1]
inline uint32_t _Random::Generate(uint32_t Count) {

	return (uint32_t)(Generate() * Count);
}

// Generates a random number [Min, Max]
inline int _Random::GenerateRange(int Min, int Max) {

	return (int)(Generate() * (Max - Min + 1)) + Min;
}

// Generates a random number [Min, Max]
inline uint32_t _Random::GenerateRange(uint32_t Min, uint32_t Max) {

	return (uint32_t)(Generate() * (Max - Min + 1)) + Min;
}

// Generates a random number [Min, Max]
inline double _Random::GenerateRange(double Min, double Max) {

	return (GenerateRandomInteger() / 4294967295.0) * (Max - Min) + Min;
}

extern _Random Random;
