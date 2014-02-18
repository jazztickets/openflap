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
