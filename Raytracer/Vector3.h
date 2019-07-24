#pragma once

#include <iostream>
#include <cmath>

template<typename T>
class Vector3
{
private:

public:
	T X, Y, Z;
	Vector3() : X(T(0)), Y(T(0)), Z(T(0)) {}
	Vector3(T x) : X(x), Y(x), Z(x) {}
	Vector3(T x, T y, T z) : X(x), Y(y), Z(z) {}
	Vector3<T> operator * (const T &f) const { return Vector3<T>(X * f, Y * f, Z * f); }
	Vector3<T> operator * (const Vector3<T> &v) const { return Vector3<T>(X * v.X, Y * v.Y, Z * v.Z); }
	Vector3<T> operator + (const Vector3<T> &v) const { return Vector3<T>(X + v.X, Y + v.Y, Z + v.Z); }
	Vector3<T> operator - (const Vector3<T> &v) const { return Vector3<T>(X - v.X, Y - v.Y, Z - v.Z); }
	Vector3<T>& operator *= (const Vector3<T> &v) const { X *= v.X; Y *= v.Y; Z *= v.Z; return *this; }
	Vector3<T>& operator += (const Vector3<T> &v) const { X += v.X; Y += v.Y; Z += v.Z; return *this; }
	Vector3<T>& operator -= (const Vector3<T> &v) const { X -= v.X; Y -= v.Y; Z -= v.Z; return *this; }
	Vector3<T> operator - () const { return Vector3<T>(-X, -Y, -Z); }
	T Dot(const Vector3<T> &v) const { return Vector3<T>(X * v.X + Y * v.Y + Z * v.Z); }
	T LengthSqrt() const { return X * X + Y * Y + Z * Z; }
	T Length() const { return sqrt(LengthSqr()); }
	Vector3& Normalize()
	{
		T norSqrt = LengthSqrt();
		if (norSqrt > 0)
		{
			T invNor = 1 / sqrt(norSqrt);
			X *= invNor, Y *= invNor, Z *= invNor;
		}

		return *this;
	}

	friend std::ostream& operator << (std::ostream &os, const Vector3<T> &v)
	{
		os << "[" << v.X << " " << v.Y << " " << v.Z << "]";
		return os;
	}
};

