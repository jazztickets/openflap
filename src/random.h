#pragma once

#include <types.h>

class _Random {

	public:
	
		_Random();
		_Random(u32 Seed);
		
		// Set initial seed
		void SetSeed(u32 Seed);
		
		// Random functions
		double Generate();
		u32 Generate(u32 Count);
		int GenerateRange(int Min, int Max);
		u32 GenerateRange(u32 Min, u32 Max);
		double GenerateRange(double Min, double Max);
		
	private:
	
		// Base function
		u32 GenerateRandomInteger();
		
		// Seeds
		u32 Q[1024];
};

// Constructor
inline _Random::_Random() {

	SetSeed(0);
}

// Constructor
inline _Random::_Random(u32 Seed) {

	SetSeed(Seed);
}

// Sets the seed for the generator
inline void _Random::SetSeed(u32 Seed) {

	for(u32 i = 0; i < 1024; i++) {
		Seed ^= Seed << 13;
		Seed ^= Seed >> 17;
		Seed ^= Seed << 5;
		Q[i] = Seed;
	}
}

// Generates a random integer
inline u32 _Random::GenerateRandomInteger() {
	static u32 c = 8471623, i = 1023;
	u64 t, a = 123471786LL;
	u32 x, r = 0xfffffffe;
	
	i = (i + 1) & 1023;
	t = a * Q[i] + c;
	c = t >> 32;
	x = (u32)(t + c);
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
inline u32 _Random::Generate(u32 Count) {

	return (u32)(Generate() * Count);
}

// Generates a random number [Min, Max]
inline int _Random::GenerateRange(int Min, int Max) {

	return (int)(Generate() * (Max - Min + 1)) + Min;
}

// Generates a random number [Min, Max]
inline u32 _Random::GenerateRange(u32 Min, u32 Max) {

	return (u32)(Generate() * (Max - Min + 1)) + Min;
}

// Generates a random number [Min, Max]
inline double _Random::GenerateRange(double Min, double Max) {

	return (GenerateRandomInteger() / 4294967295.0) * (Max - Min) + Min;
}

extern _Random Random;
