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
#include <iostream>
#include <iomanip>
#include <cmath>

// Constants
const float DEGREES_IN_RADIAN = 180.0f / (float)M_PI;

// Classes
union Vector2 {

	public:

		Vector2();
		Vector2(float X, float Y);
		Vector2(const float *Data);
		Vector2(float Degrees);

		// Utility functions
		void Set(float X, float Y) { this->X = X; this->Y = Y; }
		void Print() const;
		void SetZero();
		void Normalize();
		float Magnitude() const;
		float MagnitudeSquared() const;
		Vector2 UnitVector() const;
		Vector2 RotateVector(float Degrees) const;
		Vector2 CrossProduct(const Vector2 &Vector) const;

		// Operators
		Vector2(const Vector2 &Vector);
		Vector2 &operator=(const Vector2 &Vector);
		bool operator==(const Vector2 &Vector) const;
		bool operator!=(const Vector2 &Vector) const;
		Vector2 operator+(const Vector2 &Vector) const;
		Vector2 operator-(const Vector2 &Vector) const;
		Vector2 operator+=(const Vector2 &Vector);
		Vector2 operator-=(const Vector2 &Vector);
		float operator*(const Vector2 &Vector) const;
		Vector2 operator*(const float &Value) const;
		Vector2 operator*=(const float &Value);
		Vector2 operator/(const float &Value) const;
		Vector2 operator/=(const float &Value);
		float &operator[](int Index) { return Data[Index]; }
		float operator[](int Index) const { return Data[Index]; }
		friend std::ostream &operator<<(std::ostream &Stream, const Vector2 &Vector);

		struct {
			float X;
			float Y;
		};

		float Data[2];
};

const Vector2 ZERO_VECTOR = Vector2(0.0f, 0.0f);

// Constructor
inline Vector2::Vector2() {
}

// Constructor
inline Vector2::Vector2(float X, float Y) {
	this->X = X;
	this->Y = Y;
}

// Constructor
inline Vector2::Vector2(const float *Data) {
	this->X = Data[0];
	this->Y = Data[1];
}

// Creates a vector from a given degree
inline Vector2::Vector2(float Degrees) {
	Degrees -= 90.0f;
	X = cosf(Degrees / DEGREES_IN_RADIAN);
	Y = sinf(Degrees / DEGREES_IN_RADIAN);
}

// Outputs the vector to stdout
inline void Vector2::Print() const {
	std::cout << "| ";
	std::cout << std::setw(13) << X;
	std::cout << std::setw(13) << Y;
	std::cout << "|" << std::endl;
}

// Sets the vector to zero
inline void Vector2::SetZero() {
	X = Y = 0;
}

// Normalizes the vector
inline void Vector2::Normalize() {
	*this /= Magnitude();
}

// Calculates the magnitude of the vector
inline float Vector2::Magnitude() const {
	return sqrtf(X * X + Y * Y);
}

// Calculates the squared magnitude of the vector
inline float Vector2::MagnitudeSquared() const {
	return X * X + Y * Y;
}

// Calculates the unit vector
inline Vector2 Vector2::UnitVector() const {
	return *this / Magnitude();
}

// Rotates a vector
inline Vector2 Vector2::RotateVector(float Degrees) const {

	float Cosine = cosf(Degrees / DEGREES_IN_RADIAN);
	float Sine = sinf(Degrees / DEGREES_IN_RADIAN);

	return Vector2(Cosine * X - Sine * Y, Sine * X + Cosine * Y);
}

// Copy constructor
inline Vector2::Vector2(const Vector2 &Vector) {
	X = Vector.X;
	Y = Vector.Y;
}

// Assignment
inline Vector2 &Vector2::operator=(const Vector2 &Vector) {
	X = Vector.X;
	Y = Vector.Y;

	return *this;
}

// Equality
inline bool Vector2::operator==(const Vector2 &Vector) const {

	return X == Vector.X && Y == Vector.Y;
}

// Inequality
inline bool Vector2::operator!=(const Vector2 &Vector) const {

	return !(X == Vector.X && Y == Vector.Y);
}

// Vector addition
inline Vector2 Vector2::operator+(const Vector2 &Vector) const {

	return Vector2(X + Vector.X, Y + Vector.Y);
}

// Vector addition
inline Vector2 Vector2::operator+=(const Vector2 &Vector) {
	X = X + Vector.X;
	Y = Y + Vector.Y;

	return *this;
}

// Vector subtraction
inline Vector2 Vector2::operator-(const Vector2 &Vector) const {

	return Vector2(X - Vector.X, Y - Vector.Y);
}

// Vector subtraction
inline Vector2 Vector2::operator-=(const Vector2 &Vector) {
	X = X - Vector.X;
	Y = Y - Vector.Y;

	return *this;
}

// Dot product
inline float Vector2::operator*(const Vector2 &Vector) const {
	return (X * Vector.X + Y * Vector.Y);
}

// Scalar multiplication
inline Vector2 Vector2::operator*(const float &Value) const {

	return Vector2(X * Value, Y * Value);
}

// Scalar multiplication
inline Vector2 Vector2::operator*=(const float &Value) {
	X = X * Value;
	Y = Y * Value;

	return *this;
}

// Scalar division
inline Vector2 Vector2::operator/(const float &Value) const {
	return Vector2(X / Value, Y / Value);
}

// Scalar division
inline Vector2 Vector2::operator/=(const float &Value) {
	X = X / Value;
	Y = Y / Value;

	return *this;
}

// Stream operator
inline std::ostream &operator<<(std::ostream &Stream, const Vector2 &Vector) {
	Stream << " X=" << std::setw(13) << Vector.X << " Y=" << std::setw(13) << Vector.Y;

	return Stream;
}
