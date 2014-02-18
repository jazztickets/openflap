#pragma once

// Libraries
#include "vector2.h"

// Physics data
class _Physics {

	public:
	
		_Physics();
		_Physics(const Vector2 &Position, const Vector2 &Velocity, const Vector2 &Acceleration);
		
		// Update
		void Update(float FrameTime);
		
		void SetAcceleration(const Vector2 &Acceleration) { this->Acceleration = Acceleration; }
		void SetLastPosition(const Vector2 &LastPosition) { this->LastPosition = LastPosition; }
		void SetPosition(const Vector2 &Position) { this->Position = Position; }
		void SetVelocity(const Vector2 &Velocity) { this->Velocity = Velocity; }
		
		const Vector2 &GetAcceleration() const { return Acceleration; }
		const Vector2 &GetLastPosition() const { return LastPosition; }
		const Vector2 &GetPosition() const { return Position; }
		const Vector2 &GetVelocity() const { return Velocity; }
		
	private:
	
		void RungeKutta4Evaluate(const _Physics &Derivative, float FrameTime, _Physics &Output);
		
		// State
		Vector2 LastPosition, Position;
		Vector2 Velocity;
		Vector2 Acceleration;
};
