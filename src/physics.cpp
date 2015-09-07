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
#include <physics.h>

// Constructor
_Physics::_Physics() {

}

// Constructor
_Physics::_Physics(const Vector2 &Position, const Vector2 &Velocity, const Vector2 &Acceleration) :
	LastPosition(Position),
	Position(Position),
	Velocity(Velocity),
	Acceleration(Acceleration) {
}

// Integrate
void _Physics::Update(float FrameTime) {

	// RK4 increments
	_Physics A, B, C, D;
	RungeKutta4Evaluate(_Physics(Vector2(0, 0), Vector2(0, 0), Vector2(0, 0)), 0.0f, A);
	RungeKutta4Evaluate(A, FrameTime * 0.5f, B);
	RungeKutta4Evaluate(B, FrameTime * 0.5f, C);
	RungeKutta4Evaluate(C, FrameTime, D);

	// Calculate weighted sum
	Vector2 PositionChange = (A.Position + (B.Position + C.Position) * 2.0f + D.Position) * (1.0f / 6.0f);
	Vector2 VelocityChange = (A.Velocity + (B.Velocity + C.Velocity) * 2.0f + D.Velocity) * (1.0f / 6.0f);

	// Update physics state
	LastPosition = Position;
	Position = Position + PositionChange * FrameTime;
	Velocity = Velocity + VelocityChange * FrameTime;
}

// Evaluate increments
void _Physics::RungeKutta4Evaluate(const _Physics &Derivative, float FrameTime, _Physics &Output) {

	_Physics NewState;
	NewState.Position = Position + Derivative.Position * FrameTime;
	NewState.Velocity = Velocity + Derivative.Velocity * FrameTime;

	// Set derivative
	Output.Position = NewState.Velocity;
	Output.Velocity = Acceleration;
}
