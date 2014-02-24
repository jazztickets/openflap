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
#include <vector2.h>

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
