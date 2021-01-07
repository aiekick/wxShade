/*
   Copyright 2016 Stephane Cuillerdier aka Aiekick

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef _TOOLS_H_
#define _TOOLS_H_

#ifdef COCOS2D
#include "cocos2d.h"
using namespace cocos2d;
#endif

#ifdef BOX2D
#include "Box2D\Box2D.h"
#endif

#include <glad/glad.h>

#include "float.h"

#include <sstream>
#include <ostream>
#include <cmath>
#include <list>
#include <string>
#include <map>
#include <chrono>
#include <vector>
using namespace std;

typedef signed char	int8;
typedef signed short int16;
typedef signed int int32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef float float32;
typedef double float64;

#ifdef WIN32
typedef __int64   int64;
typedef unsigned __int64   uint64;
#else // !WIN32
typedef long long int64;
typedef unsigned long long uint64;
#endif

#define	_maxFloat		FLT_MAX
#define	_epsilon		FLT_EPSILON
#define _pi			3.14159265359f

#define DEGtoRAD(n) _pi * n / 180.0f

inline std::list<std::string> splitString(const std::string& text, char delimiter)
{
	std::list<std::string> result;
	std::string::size_type start = 0;
	std::string::size_type end = text.find(delimiter, start);
	while (end != std::string::npos)
	{
		std::string token = text.substr(start, end - start);
		result.push_front(token);
		start = end + 1;
		end = text.find(delimiter, start);
	}
	result.push_front(text.substr(start));

	return result;
}

inline bool StringToFloat(string vWord, float *vFloat)
{
	float v = 0.0;
	if (vWord.find_first_not_of("-+0123456789.") == string::npos)
	{
		std::stringstream IsItaFloat = std::stringstream(vWord);
		IsItaFloat >> v;
		if (IsItaFloat)
		{
			*vFloat = v;
			return true;
		}
	}
	return false;
}

class cActionTime
{
private:
	static __int64 GetTicks() {
		return std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();
	}
	__int64 lastTick;
public:
	cActionTime()
	{
		lastTick = cActionTime::GetTicks();
	}
	void Fix() // fixe le marqueur de temps
	{
		lastTick = cActionTime::GetTicks();
	}
	long get() { return (long)(cActionTime::GetTicks() - lastTick); }
	// verifie si vMs millisecnd depuis le dernier fix et donc si on peut agir
	// vFix est pour fixer en meme temps ou pas // on pourrait vouloir interroger sans vouloir fixer
	bool IsTimeToAct(long vMs, bool vFix)
	{
		long diff = (long)(cActionTime::GetTicks() - lastTick);
		if (diff > vMs)
		{
			if (vFix == true)
				Fix();
			return true;
		}
		return false;
	}
};

#ifndef COCOS2D
#define cAssert(a,b) if (a==true) { LogStr(b); assert(a); }

template <typename T> string toStr(const T& t)
{
	ostringstream os;
	os << t;
	return os.str();
}
#endif
/*
static std::vector<std::wstring> splitWString(const std::wstring& text, wchar_t delimiter)
{
std::vector<std::wstring> result;

std::wstring::size_type start = 0;
std::wstring::size_type end = text.find(delimiter, start);

while (end != wstring::npos)
{
std::wstring token = text.substr(start, end - start);

result.push_back(token);

start = end + 1;
end = text.find(delimiter, start);
}

result.push_back(text.substr(start));

return result;
}
*/

class cUniformVariant
{
public:
	int uSampler2D;
	int uSamplerCube;
	int id;
	float x;
	float y;
	float z;
	float w;

	int count; // indique le nombre de var utilisée donc valeur de 1 à 4

public:
	cUniformVariant()
	{
		uSampler2D = -1;
		uSamplerCube = -1;
		id = 0;
		x = 0;
		y = 0;
		z = 0;
		w = 0;
		count = 0;
	}
};

class cTexture
{
public:
	cTexture()
	{
		format = "";
		relativPath = "";
		zOrder = 0.0f;
		w = h = x = y = 0.0f;
		//surface = NULL;
		step = 0.0;
	}
	string format;
	string relativPath;
	float zOrder;
	float w;
	float h;
	float x;
	float y;
	float step;
	//SDL_Surface *surface;
	GLuint glTex;
};

class cColor
{
public: // static
	static cColor RandomColor()
	{
		int r = (rand() % (256)); // 0 à 3
		int g = (rand() % (256)); // 0 à 3
		int b = (rand() % (256)); // 0 à 3
		int a = 255;
		return cColor(r, g, b, a);
	}
	static string RandomColorNameExcept(string cat)
	{
		string colorName = cat;
		while (colorName == cat)
		{
			int choice = (rand() % (4)); // 0 à 3
			if (choice == 0) colorName = "blue";
			else if (choice == 1) colorName = "red";
			else if (choice == 2) colorName = "purple";
			else if (choice == 3) colorName = "green";
		}

		return colorName;
	}
public:
	cColor()
	{
		r=g=b=a=1.0f;
	}
	cColor(string colorName)
	{
		if (colorName == "red") setColor(1.0f, 0.0f, 0.0f, 1.0f);
		else if (colorName == "green") setColor(0.0f, 1.0f, 0.0f, 1.0f);
		else if (colorName == "blue") setColor(0.0f, 0.0f, 1.0f, 1.0f);
		else if (colorName == "purple") setColor(1.0f, 0.0f, 1.0f, 1.0f);
		else if (colorName == "neutral") setColor(0.0f, 1.0f, 1.0f, 1.0f);
		/*else //may be in format "0.2f,0.3f,0.4f"
		{
			std::vector<float> result;
			std::string::size_type start = 0;
			std::string::size_type end = colorName.find(",", start);
			while (end != string::npos)
			{
				std::string token = colorName.substr(start, end - start);
				result.push_back((float)atof(token.c_str()));
				start = end + 1;
				end = colorName.find(",", start);
			}
			result.push_back((float)atof(colorName.substr(start).c_str()));

			if (result.size() == 3) setColor(result[0], result[1], result[2], 1.0f);
			else if (result.size() == 4) setColor(result[0], result[1], result[2], result[3]);
		}*/
	}
	cColor(int _r, int _g, int _b, int _a = 255)
	{
		setColor(_r, _g, _b, _a);
	}
	cColor(float _r, float _g, float _b, float _a = 1.0f)
	{
		setColor(_r, _g, _b, _a);
	}
	void setColor(int _r, int _g, int _b, int _a)
	{
		r = _r / 255.0f;
		g = _g / 255.0f;
		b = _b / 255.0f;
		a = _a / 255.0f;
	}
	void setColor(float _r, float _g, float _b, float _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	void setColor(const cColor col)
	{
		r = col.r;
		g = col.g;
		b = col.b;
		a = col.a;
	}
#ifdef COCOS2D
	cocos2d::Color3B getCCColor3B()
	{
		cocos2d::Color3B col;
		col.r = r * 255;
		col.g = g * 255;
		col.b = b * 255;
		return col;
	}
	cocos2d::Color4F getCCColor4F()
	{
		cocos2d::Color4F col;
		col.r = r;
		col.g = g;
		col.b = b;
		col.a = a;
		return col;
	}
#endif
	void setColorCanal(char canal, float value)
	{
		if (canal == 'r') r = value;
		if (canal == 'g') g = value;
		if (canal == 'b') b = value;
		if (canal == 'a') a = value;
	}
	void setColorCanal(char canal, int value)
	{
		if (canal == 'r') r = value / 255.0f;
		if (canal == 'g') g = value / 255.0f;
		if (canal == 'b') b = value / 255.0f;
		if (canal == 'a') a = value / 255.0f;
	}
public: // var
    float r;
	float g;
	float b;
	float a;
};

class cVector
{
private:
	
public:
	
};

/// This function is used to ensure that a floating point number is not a NaN or infinity.
inline bool floatIsValid(float32 x)
{
	union {
		float32 f;
		int32 i;
	} v = { x };
	return (v.i & 0x7f800000) != 0x7f800000;
}

template <typename T>
inline T cMin(T a, T b)
{
	return a < b ? a : b;
}

template <typename T>
inline T cMax(T a, T b)
{
	return a > b ? a : b;
}

//// ABS ////////////////////////////////////////////////////
inline float cAbs(float a)
{
	return a < 0 ? a * -1.0f : a;
}

inline int cAbs(int a)
{
	return a < 0 ? a * -1 : a;
}

inline double cAbs(double a)
{
	return a < 0 ? a * -1.0 : a;
}

/////////////////////////////////////////////////////////////
///////// cVec2 /////////////////////////////////////////////
/////////////////////////////////////////////////////////////

struct cVec2
{
	float32 x, y;

	/// Default constructor does nothing (for performance).
	cVec2() : x(0.0f), y(0.0f) {}

	/// Construct using coordinates.
	cVec2(float32 xy) : x(xy), y(xy) {}
	cVec2(float32 x, float32 y) : x(x), y(y) {}

#ifdef BOX2D
	/// cVec2 from b2Vec2
	cVec2(const b2Vec2 vec) : x(vec.x), y(vec.y) {}
#endif

#ifdef COCOS2D
	/// cVec2 from CCSize
	cVec2(const cocos2d::Size pt) : x(pt.width), y(pt.height) { }

	/// cVec2 from CCPoint
	cVec2(const cocos2d::Vec2 pt) : x(pt.x), y(pt.y) { }
#endif

	/// get offset of point
	cVec2 Offset(float32 vX, float32 vY) { return cVec2(x + vX, y + vY); }
	
	/// Set this vector to all zeros.
	void SetZero() { x = 0.0f; y = 0.0f; }

	bool IsEmpty() { if (x == 0.0f && y == 0.0f) return true; else return false; }

	/// Set this vector to some specified coordinates.
	void Set(float32 x_, float32 y_) { x = x_; y = y_; }

	/// Negate this vector.
	cVec2 operator -() const { cVec2 v; v.Set(-x, -y); return v; }

	/// Read from and indexed element.
	float32 operator () (int32 i) const
	{
		return (&x)[i];
	}

	float32 operator [] (int32 i) const
	{
		float32 val = (&x)[i];
		return val;
	}

	/// Write to an indexed element.
	float32& operator () (int32 i)
	{
		return (&x)[i];
	}

	/// Add a vector to this vector.
	void operator += (const cVec2& v)
	{
		x += v.x; y += v.y;
	}

	/// compare a vector to this vector.
	bool operator == (const cVec2& v)
	{
		if (x == v.x && y == v.y)
			return true;
		return false;
	}

	/// compare a vector to this vector.
	bool operator != (const cVec2& v)
	{
		if (x != v.x || y != v.y)
			return true;
		return false;
	}

	/// Subtract a vector from this vector.
	void operator -= (const cVec2& v)
	{
		x -= v.x; y -= v.y;
	}

	/// Multiply this vector by a scalar.
	void operator *= (float32 a)
	{
		x *= a; y *= a;
	}

	/// Divide this vector by a scalar.
	void operator /= (float32 a)
	{
		x /= a; y /= a;
	}

	/// Get the length of this vector (the norm).
	float32 Length() const
	{
		return sqrtf(x * x + y * y);
	}

	/// Get the Signed length of this vector (the norm).
	float32 SignedLength() const
	{
		int sx = 1; int sy = 1;
		if (x < 0) sx = -1;
		if (y < 0) sy = -1;
		return sqrtf(x * x * sx + y * y * sy);
	}
	/// Get the length squared. For performance, use this instead of
	/// cVec2::Length (if possible).
	float32 LengthSquared() const
	{
		return x * x + y * y;
	}

	/// Get the Signed length squared. For performance, use this instead of
	/// cVec2::SignedLength (if possible).
	float32 SignedLengthSquared() const
	{
		int sx = 1; int sy = 1;
		if (x < 0) sx = -1;
		if (y < 0) sy = -1;
		return x * x * sx + y * y * sy;
	}

	/// Convert this vector into a unit vector. Returns the length.
	float32 Normalize()
	{
		float32 length = Length();
		if (length < _epsilon)
		{
			return 0.0f;
		}
		float32 invLength = 1.0f / length;
		x *= invLength;
		y *= invLength;

		return length;
	}

	/// Set this vector to some specified coordinates.
	void Set(float32 xy_) { x = xy_; y = xy_; }

	void doAbs(){ x = abs(x); y = abs(y); }

	/// renvoie le cVec2 de direction
	cVec2 Sign()
	{
		cVec2 sgn;
		if (x > 0) sgn.x = 1; else if (x < 0) sgn.x = -1; else sgn.x = 0;
		if (y > 0) sgn.y = 1; else if (y < 0) sgn.y = -1; else sgn.y = 0;
		return sgn;
	}

	/// Does this vector contain finite coordinates?
	bool IsValid() const
	{
		return floatIsValid(x) && floatIsValid(y);
	}

	/// Get the skew vector such that dot(skew_vec, other) == cross(vec, other)
	const cVec2 Skew()
	{
		return cVec2(-y, x);
	}

	cVec2 Abs()
	{
		cVec2(cAbs(x), cAbs(y));
	}
#ifdef BOX2D
	const b2Vec2 Tob2Vec2()
	{
		b2Vec2 v = b2Vec2(x, y);
		return v;
	}
#endif

#ifdef COCOS2D
	cocos2d::Vec2 ToCCVec2()
	{
		return cocos2d::Vec2(x, y);
	}
	cocos2d::Size ToCCSize()
	{
		return cocos2d::Size(x, y);
	}
#endif
};

/// Add a float to a vector.
inline cVec2 operator + (const cVec2& v, float f)
{
	return cVec2(v.x + f, v.y + f);
}

/// Add a vector to a vector.
inline cVec2 operator + (const cVec2& v, cVec2 f)
{
	return cVec2(v.x + f.x, v.y + f.y);
}

/// Substract a float from a vector.
inline cVec2 operator - (const cVec2& v, float f)
{
	return cVec2(v.x - f, v.y - f);
}

/// Substract a vector to a vector.
inline cVec2 operator - (const cVec2& v, cVec2 f)
{
	return cVec2(v.x - f.x, v.y - f.y);
}

/// Multiply a float with a vector.
inline cVec2 operator * (const cVec2& v, float f)
{
	return cVec2(v.x * f, v.y * f);
}

/// Multiply a vector with a vector.
inline cVec2 operator * (const cVec2& v, cVec2 f)
{
	return cVec2(v.x * f.x, v.y * f.y);
}

/// Divide a vector by a float.
inline cVec2 operator / (const cVec2& v, float f)
{
	return cVec2(v.x / f, v.y / f);
}

/// Divide a vector by a float.
inline cVec2 operator / (cVec2& v, float f)
{
	return cVec2(v.x / f, v.y / f);
}

/// Divide a vector by a vector.
inline cVec2 operator / (const cVec2& v, cVec2 f)
{
	return cVec2(v.x / f.x, v.y / f.y);
}


/// Divide a vector by a vector.
inline bool operator == (const cVec2& v, cVec2 f)
{
	if (f.x == v.x && f.y == v.y)
		return true;
	return false;
}

/*template <typename T>
inline T cMin(T a, T b)
{
	return a < b ? a : b;
}*/

inline cVec2 cMin(const cVec2& a, const cVec2& b)
{
	return cVec2(cMin(a.x, b.x), cMin(a.y, b.y));
}

/*template <typename T>
inline T cMax(T a, T b)
{
	return a > b ? a : b;
}*/

inline cVec2 cMax(const cVec2& a, const cVec2& b)
{
	return cVec2(cMax(a.x, b.x), cMax(a.y, b.y));
}

// produit scalaire
inline float dotS(cVec2 a, cVec2 b)
{
	return a.x * b.x + a.y * b.y;
}

// produit Vectoriel
inline float dotV(cVec2 a, cVec2 b)
{
	return a.x * b.y - a.y * b.x;
}

inline float radAngle2Vec(cVec2 y, cVec2 z)
{
	/*	Si vec1 et vec2 sont des vecteurs unitaires (Normaux), la longueur de chaque vecteur est égale à 1.
	Ainsi, lorsque vec1 et vec2 sont bien des vecteurs unitaires:
	- le produit scalaire est simplement égal au cosinus de l'angle entre les deux vecteurs
	- le produit vectoriel est simplement égal au sinus de l'angle entre les deux vecteurs
	*/
	y.Normalize();
	z.Normalize();
	float dot = dotV(y, z);
	float asinVal = asinf(dot);
	return asinVal;
}

inline float radAngle2Point1Center(cVec2 a, cVec2 b, cVec2 center)
{
	/*	Si vec1 et vec2 sont des vecteurs unitaires (Normaux), la longueur de chaque vecteur est égale à 1.
	Ainsi, lorsque vec1 et vec2 sont bien des vecteurs unitaires:
	- le produit scalaire est simplement égal au cosinus de l'angle entre les deux vecteurs
	- le produit vectoriel est simplement égal au sinus de l'angle entre les deux vecteurs
	*/
	cVec2 vec1 = a - center;
	cVec2 vec2 = b - center;
	return radAngle2Vec(vec1, vec2);
}

inline float degAngle2Vec(cVec2 y, cVec2 z)
{
	return radAngle2Vec(y,z) * 180.0f / _pi;
}

inline float angle2Point1Center(cVec2 a, cVec2 b, cVec2 center)
{
	return radAngle2Point1Center(a, b, center) * 180.0f / _pi;
}

// vAngle en radians
// renvoi une eq de droite du type y = a * x + b => b2Vec2(a,b)
inline cVec2 getLineEQFromAngleAndPos(cVec2 vPos, float vAngle)
{
	float c = cosf(vAngle);
	float s = sinf(vAngle);
	float a = s / c; // pente

	// vPos.y = a * vPos.x + b
	float b = vPos.y - a * vPos.x;

	return cVec2(a, b);
}

inline cVec2 getIntersectionBetweenTwoLines(cVec2 vLine1, cVec2 vLine2)
{
	cVec2 pt;
	// y = a1 * x + b
	// y = a2 * x + b
	// intersection dit a1 * x + b1 = a2 * x + b2
	// a1 * x - a2 * x = b2 - b1
	// x * (a1 - a2) = b2 - b1
	// x = (b2 - b1) / (a1 - a2)
	pt.x = (vLine2.y - vLine1.y) / (vLine1.x - vLine2.x);

	// eq1 => y = a1 * x + b c'est pareil pour eq2 mais bon faut en prendre un des deux alors eq1
	pt.y = vLine1.x * pt.x + vLine1.y;

	return pt;
}

/////////////////////////////////////////////////////////////
///////// cVec4 /////////////////////////////////////////////
/////////////////////////////////////////////////////////////
struct cVec4
{
	float32 x, y, z, w;

	/// Default constructor does nothing (for performance).
	cVec4() : x(0.0f), y(0.0f), z(0.0), w(0.0) {}

	/// Construct using coordinates.
	cVec4(float32 xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}
	cVec4(float32 x, float32 y, float32 z, float32 w) : x(x), y(y), z(z), w(w) {}

	/*/// cVec4 from CCSize
	cVec4(const cocos2d::Size pt) : x(pt.width), y(pt.height) { }

	/// cVec4 from CCPoint
	cVec4(const cocos2d::Vec2 pt) : x(pt.x), y(pt.y) { }*/

	/// get offset of point
	cVec4 Offset(float32 vX, float32 vY, float32 vZ, float32 vW) { return cVec4(x + vX, y + vY, z + vZ, w + vW); }

	/// Set this vector to all zeros.
	void SetZero() { x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f; }

	bool IsEmpty() { if (x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f) return true; else return false; }

	/// Set this vector to some specified coordinates.
	void Set(float32 vX, float32 vY, float32 vZ, float32 vW) { x = vX; y = vY; z = vZ; w = vW; }

	/// Negate this vector.
	cVec4 operator -() const { cVec4 v; v.Set(-x, -y, -z, -w); return v; }

	/*/// Read from and indexed element.
	float32 operator () (int32 i) const
	{
	return (&x)[i];
	}

	/// Write to an indexed element.
	float32& operator () (int32 i)
	{
	return (&x)[i];
	}*/

	/// Add a vector to this vector.
	void operator += (const cVec4& v)
	{
		x += v.x; y += v.y; z += v.z; w += v.w;
	}

	/// compare a vector to this vector.
	bool operator == (const cVec4& v)
	{
		if (x == v.x && y == v.y && z == v.z && w == v.w)
			return true;
		return false;
	}

	/// compare a vector to this vector.
	bool operator != (const cVec4& v)
	{
		if (x != v.x || y != v.y || z != v.z || w != v.w)
			return true;
		return false;
	}

	/// Subtract a vector from this vector.
	void operator -= (const cVec4& v)
	{
		x -= v.x; y -= v.y; z -= v.z; w -= v.w;
	}

	/// Multiply this vector by a scalar.
	void operator *= (float32 a)
	{
		x *= a; y *= a; z *= a; w *= a;
	}

	/// Divide this vector by a scalar.
	void operator /= (float32 a)
	{
		x /= a; y /= a; z /= a; w /= a;
	}

	/// Get the length of this vector (the norm).
	float32 Length() const
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	/// Get the Signed length of this vector (the norm).
	float32 SignedLength() const
	{
		int sx = 1, sy = 1, sz = 1, sw = 1;
		if (x < 0) sx = -1;
		if (y < 0) sy = -1;
		if (z < 0) sz = -1;
		if (w < 0) sw = -1;
		return sqrtf(x * x * sx + y * y * sy + z * z * sz + w * w * sw);
	}
	/// Get the length squared. For performance, use this instead of
	/// cVec4::Length (if possible).
	float32 LengthSquared() const
	{
		return x * x + y * y + z * z + w * w;
	}

	/// Get the Signed length squared. For performance, use this instead of
	/// cVec4::SignedLength (if possible).
	float32 SignedLengthSquared() const
	{
		int sx = 1, sy = 1, sz = 1, sw = 1;
		if (x < 0) sx = -1;
		if (y < 0) sy = -1;
		if (z < 0) sz = -1;
		if (w < 0) sw = -1;
		return x * x * sx + y * y * sy + z * z * sz + w * w * sw;
	}

	cVec4 Abs()
	{
		return cVec4(cAbs(x), cAbs(y), cAbs(z), cAbs(w));
	}

	/// Convert this vector into a unit vector. Returns the length.
	float32 Normalize()
	{
		float32 length = Length();
		if (length < _epsilon)
		{
			return 0.0f;
		}
		float32 invLength = 1.0f / length;
		x *= invLength;
		y *= invLength;
		z *= invLength;
		w *= invLength;
		return length;
	}

	/// Does this vector contain finite coordinates?
	bool IsValid() const
	{
		return floatIsValid(x) && floatIsValid(y);
	}

	/*/// Get the skew vector such that dot(skew_vec, other) == cross(vec, other)
	const cVec4 Skew()
	{
	return cVec4(-y, x);
	}*/

	/*const cVec4 Tob2Vec2()
	{
	cVec4 v = cVec4(x, y);
	return v;
	}*/

	/*cocos2d::Vec2 ToCCVec2()
	{
	return cocos2d::Vec2(x, y);
	}
	cocos2d::Size ToCCSize()
	{
	return cocos2d::Size(x, y);
	}*/
};

/// Add a float to a vector.
inline cVec4 operator + (const cVec4& v, float f)
{
	return cVec4(v.x + f, v.y + f, v.z + f, v.w + f);
}

/// Add a vector to a vector.
inline cVec4 operator + (const cVec4& v, cVec4 f)
{
	return cVec4(v.x + f.x, v.y + f.y, v.z + f.z, v.w + f.w);
}

/// Substract a float from a vector.
inline cVec4 operator - (const cVec4& v, float f)
{
	return cVec4(v.x - f, v.y - f, v.z - f, v.w - f);
}

/// Substract a vector to a vector.
inline cVec4 operator - (const cVec4& v, cVec4 f)
{
	return cVec4(v.x - f.x, v.y - f.y, v.z - f.z, v.w - f.w);
}

/// Multiply a float with a vector.
inline cVec4 operator * (const cVec4& v, float f)
{
	return cVec4(v.x * f, v.y * f, v.z * f, v.w * f);
}

/// Multiply a vector with a vector.
inline cVec4 operator * (const cVec4& v, cVec4 f)
{
	return cVec4(v.x * f.x, v.y * f.y, v.z * f.z, v.w * f.w);
}

/// Divide a vector by a float.
inline cVec4 operator / (const cVec4& v, float f)
{
	return cVec4(v.x / f, v.y / f, v.z / f, v.w / f);
}

/// Divide a vector by a float.
inline cVec4 operator / (cVec4& v, float f)
{
	return cVec4(v.x / f, v.y / f, v.z / f, v.w / f);
}

/// Divide a vector by a vector.
inline cVec4 operator / (const cVec4& v, cVec4 f)
{
	return cVec4(v.x / f.x, v.y / f.y, v.z / f.z, v.w / f.w);
}


inline cVec4 cMin(const cVec4& a, const cVec4& b)
{
	return cVec4(cMin(a.x, b.x), cMin(a.y, b.y), cMin(a.z, b.z), cMin(a.w, b.w));
}


inline cVec4 cMax(const cVec4& a, const cVec4& b)
{
	return cVec4(cMax(a.x, b.x), cMax(a.y, b.y), cMax(a.z, b.z), cMax(a.w, b.w));
}

inline cVec4 cAbs(const cVec4& a)
{
	return cVec4(cAbs(a.x), cAbs(a.y), cAbs(a.z), cAbs(a.w));
}


class cRect // de bas en haut
{
private:
	float _x;
	float _y;
	float _width;
	float _height;
	float _left;
	float _right;
	float _top;
	float _bottom;
	cVec2 tmpPt;
public:
	cRect()
	{
		_x = 0;
		_y = 0;
		_width = 0;
		_height = 0;
		_left = 0;
		_right = 0;
		_top = 0;
		_bottom = 0;
	}
	cRect(float w, float h)
	{
		_x = 0;
		_y = 0;
		_width = 0;
		_height = 0;
		_left = 0;
		_right = 0;
		_top = 0;
		_bottom = 0;
		setWidth(w);
		setHeight(h);
	}

	void setRect(float x, float y, float w, float h)
	{
		_x = x;
		_y = y;
		_width = w;
		_height = h;
	}
	float x()
	{
		return _x;
	}
	float y()
	{
		return _y;
	}
	float width()
	{
		return _width;
	}
	float height()
	{
		return _height;
	}
	cVec2 rightBottom()
	{
		tmpPt.Set(_width + _x, _y);
		return tmpPt;
	}
	cVec2 rightTop()
	{
		tmpPt.Set(_width + _x, _height + _y);
		return tmpPt;
	}
	cVec2 leftBottom()
	{
		tmpPt.Set(_x, _y);
		return tmpPt;
	}
	cVec2 leftTop()
	{
		tmpPt.Set(_x, _height + _y);
		return tmpPt;
	}
	float bottom()
	{
		return _y;
	}
	float top()
	{
		return  _height+_y;
	}
	float left()
	{
		return _x;
	}
	float right()
	{
		return _width+_x;
	}
	cVec2 center()
	{
		tmpPt.Set(_width / 2 + _x, _height / 2 + _y);
		return tmpPt;
	}
	// get buffer size 4
	cVec2 *vertices()
	{
		cVec2 *buf = new cVec2[4];
		buf[0] = this->leftBottom();
		buf[1] = this->leftTop();
		buf[2] = this->rightTop();
		buf[3] = this->rightBottom();
		return buf;
	}
	// get buffer size 4
	cVec2 *texCoords(float scaleX, float scaleY)
	{
		cVec2 *buf = new cVec2[4];
		buf[0] = cVec2(0.0f*scaleX, 0.0f*scaleY);
		buf[1] = cVec2(1.0f*scaleX, 0.0f*scaleY);
		buf[2] = cVec2(1.0f*scaleX, 1.0f*scaleY);
		buf[3] = cVec2(0.0f*scaleX, 1.0f*scaleY);
		return buf;
	}	void setWidth(float w)
	{
		_width = w;
	}
		
	void setHeight(float h)
	{
		_height = h;
	}

	void SnackLeft(float left)
	{
		_x  = left;
		_width -= left;
		_left = _x;
	}
	void SnackRight(float right)
	{
		_width -= right;
		_right = _width + _x;
	}
	void SnackTop(float top)
	{
		_height -= top;
		_top = _height + _y;
	}
	void SnackBottom(float bottom)
	{
		_y = bottom;
		_height -= bottom;
		_bottom = _y;
	}
	bool IsContainPoint(cVec2 pt)
	{
		if ( pt.x > _left ) 
			if ( pt.x < _right )
				if ( pt.y > _bottom)
					if (pt.y < _top)
						return true;
		return false;
	}
};


class cColorBuffer
{
public:
	int _size;
	int _pitch;
	int _width;
	int _height;
	int _texId;
	uint8 *_buf;
	int _x;
	int _y;

public:
	cColorBuffer() : _size(0), _pitch(0), _width(0), _height(0), _buf(0) {}
	void Clear() { if (_buf != 0) free(_buf); }
	cColor getColorFromPoint(cVec2 pt) // http://cpansearch.perl.org/src/ZAPAD/Image-WebP-0.2/webp-src/src/dsp/upsampling.c
	{
		if (_buf != 0)
		{
			int stride = _width * _pitch;
			int pos = stride * (int)pt.y + (int)pt.x;
			// unpack the source data as 8 bit values
			uint8 a = _buf[2 * pos + 1] & 0x0f;
			uint8 r = _buf[2 * pos + 0] >> 4;
			uint8 g = _buf[2 * pos + 0] << 4;
			uint8 b = _buf[2 * pos + 1] >> 4;
			
			return cColor(r, g, b, a);
		}
	}
};

class cVariant
{
private:
	string type;

	string string_value;
	bool bool_value;
	int int_value;
	float float_value;
	double double_value;
	cColor color_value;
	cTexture texture_value;
	cVec2 point_value;

public:
	cVariant(){}
	cVariant(int v){ int_value = v; type = "int"; }
	cVariant(float v){ float_value = v; type = "float"; }
	cVariant(double v){ double_value = v; type = "double"; }
	cVariant(string v){ string_value = v; type = "string"; }
	cVariant(bool v){ bool_value = v; type = "bool"; }
	cVariant(cColor c){ color_value = c; type = "cColor"; }
	cVariant(cTexture c){ texture_value = c; type = "cTexture"; }
	cVariant(cVec2 c){ point_value = c; type = "cVec2"; }

	string getType()
	{
		return type;
	}

	float getF()
	{
		if (type == "string") return (float)atof(string_value.c_str());
		return float_value;
	}
	double getD()
	{
		if (type == "string") return (double)atof(string_value.c_str());
		return double_value;
	}
	int getI()
	{
		if (type == "string") return atoi(string_value.c_str());
		return int_value;
	}
	bool getB()
	{
		if (type == "bool") return bool_value;
		else if (type == "string")
		{
			if (string_value == "true" || string_value == "1") return true;
			else return false;
		}
	}
	string getS()
	{
		return string_value;
	}
	cColor getColor()
	{
		return color_value;
	}
	cTexture getTexture()
	{
		return texture_value;
	}
	cVec2 getPoint()
	{
		return point_value;
	}
};

struct cAABB // copy of b2AABB struct
{
	cAABB() : lowerBound(0.0f), upperBound(0.0f) {}
	cAABB(cVec2 vlowerBound, cVec2 vUpperBound)
		//: lowerBound(vlowerBound), upperBound(vUpperBound) 
	{
		lowerBound.x = cMin(vlowerBound.x, vUpperBound.x);
		lowerBound.y = cMin(vlowerBound.y, vUpperBound.y);
		upperBound.x = cMax(vlowerBound.x, vUpperBound.x);
		upperBound.y = cMax(vlowerBound.y, vUpperBound.y);
	}
#ifdef BOX2D
	cAABB(const b2AABB& aabb) : lowerBound(aabb.lowerBound), upperBound(aabb.upperBound) {}
#endif
	/// Verify that the bounds are sorted.
	bool IsValid() const;

	/// Add a vector to this vector.
	void operator += (const cVec2& v)
	{
		lowerBound += v; upperBound += v;
	}

	/// Subtract a vector from this vector.
	void operator -= (const cVec2& v)
	{
		lowerBound -= v; upperBound -= v;
	}

	/// Multiply this vector by a scalar.
	void operator *= (float32 a)
	{
		lowerBound *= a; upperBound *= a;
	}

	/// Divide this vector by a scalar.
	void operator /= (float32 a)
	{
		lowerBound /= a; upperBound /= a;
	}

	/// Get the center of the AABB.
	cVec2 GetCenter() const
	{
		return (lowerBound + upperBound) * 0.5f;
	}

	/// Get the extents of the AABB (half-widths).
	cVec2 GetExtents() const
	{
		return (upperBound - lowerBound) * 0.5f;
	}

	/// Get the perimeter length
	float32 GetPerimeter() const
	{
		float wx = upperBound.x - lowerBound.x;
		float wy = upperBound.y - lowerBound.y;
		return 2.0f * (wx + wy);
	}

	/// Combine an AABB into this one.
	void Combine(const cAABB& aabb)
	{
		lowerBound = cMin(lowerBound, aabb.lowerBound);
		upperBound = cMax(upperBound, aabb.upperBound);
	}

	/// Combine two AABBs into this one.
	void Combine(const cAABB& aabb1, const cAABB& aabb2)
	{
		lowerBound = cMin(aabb1.lowerBound, aabb2.lowerBound);
		upperBound = cMax(aabb1.upperBound, aabb2.upperBound);
	}

	/// Combine a point into this one.
	/*void Combine(const cVec2& pt)
	{
	lowerBound.x = cMin(lowerBound.x, pt.x);
	lowerBound.y = cMin(lowerBound.y, pt.y);
	upperBound.x = cMax(upperBound.x, pt.x);
	upperBound.y = cMax(upperBound.y, pt.y);
	}*/

	/// Does this aabb contain the provided AABB.
	bool Contains(const cAABB& aabb) const
	{
		bool result = true;
		result = result && lowerBound.x <= aabb.lowerBound.x;
		result = result && lowerBound.y <= aabb.lowerBound.y;
		result = result && aabb.upperBound.x <= upperBound.x;
		result = result && aabb.upperBound.y <= upperBound.y;
		return result;
	}

	/// Does this aabb contain the provided cVec2.
	bool ContainsPoint(const cVec2& pt) const
	{
		bool result = true;
		result = result && lowerBound.x <= pt.x;
		result = result && lowerBound.y <= pt.y;
		result = result && pt.x <= upperBound.x;
		result = result && pt.y <= upperBound.y;
		return result;
	}

	bool Intersects(const cAABB& other)
	{
		bool result = true;
		result = result || lowerBound.x <= other.lowerBound.x;
		result = result || lowerBound.y <= other.lowerBound.y;
		result = result || other.upperBound.x <= upperBound.x;
		result = result || other.upperBound.y <= upperBound.y;
		return result;
	}
#ifdef BOX2D
	const b2AABB Tob2AABB()
	{
		b2AABB v;
		v.lowerBound = lowerBound.Tob2Vec2();
		v.upperBound = upperBound.Tob2Vec2();
		return v;
	}
#endif
	const cVec2 Size()
	{
		return cVec2(upperBound - lowerBound);
	}

	cVec2 lowerBound;	///< the lower left vertex
	cVec2 upperBound;	///< the upper right vertex
};


/// Add a float to a cAABB.
inline cAABB operator + (const cAABB& v, float f)
{
	return cAABB(v.lowerBound + f, v.upperBound + f);
}

/// Add a cAABB to a cAABB.
inline cAABB operator + (const cAABB& v, cAABB f)
{
	return cAABB(v.lowerBound + f.lowerBound, v.upperBound + f.upperBound);
}

/// Substract a float from a cAABB.
inline cAABB operator - (const cAABB& v, float f)
{
	return cAABB(v.lowerBound - f, v.upperBound - f);
}

/// Substract a cAABB to a cAABB.
inline cAABB operator - (const cAABB& v, cAABB f)
{
	return cAABB(v.lowerBound - f.lowerBound, v.upperBound - f.upperBound);
}

/// Multiply a float with a cAABB.
inline cAABB operator * (const cAABB& v, float f)
{
	return cAABB(v.lowerBound * f, v.upperBound * f);
}

/// Multiply a cAABB with a cAABB.
inline cAABB operator * (const cAABB& v, cAABB f)
{
	return cAABB(v.lowerBound * f.lowerBound, v.upperBound * f.upperBound);
}

/// Divide a cAABB by a float.
inline cAABB operator / (const cAABB& v, float f)
{
	return cAABB(v.lowerBound / f, v.upperBound / f);
}

/// Divide a cAABB by a float.
inline cAABB operator / (cAABB& v, float f)
{
	return cAABB(v.lowerBound / f, v.upperBound / f);
}

/// Divide a cAABB by a cAABB.
inline cAABB operator / (const cAABB& v, cAABB f)
{
	return cAABB(v.lowerBound / f.lowerBound, v.upperBound / f.upperBound);
}

// Buffer a destroy apres utilisationBufferSize
// in : lst, offset
// out : return, BufferSize
template<typename T>
inline T* getNewBufferFromList(list<T> &lst, int offsetBefore, int offsetAfter, int *BufferSize)
{
	int count = (int)lst.size();
	if (count > 0)
	{
		*BufferSize = count + offsetBefore + offsetAfter;
		T *Buffer = new T[(*BufferSize)]; int idx = offsetBefore;
		// before init
		for (int i = 0; i < offsetBefore; i++)
		{
			Buffer[i] = T();
		}
		// content
		for (typename std::list<T>::iterator it = lst.begin(); it != lst.end(); ++it)
		{
			T obj = *it;
			Buffer[idx++] = obj;
		}
		// after init
		for (int i = 0; i < offsetAfter; i++)
		{
			Buffer[idx+i] = T();
		}
		return Buffer;
	}
	return 0;
}

// Buffer a destroy apres utilisationBufferSize
// in : lst, offset
// out : return, BufferSize
template<typename T, typename P>
inline P* getNewBufferFromMap(map<T, P> &mp, int offsetBefore, int offsetAfter, int *BufferSize)
{
	int count = (int)mp.size();
	if (count > 0)
	{
		*BufferSize = count + offsetBefore + offsetAfter;
		P *Buffer = new P[(*BufferSize)]; int idx = offsetBefore;
		// before init
		for (int i = 0; i < offsetBefore; i++)
		{
			Buffer[i] = P();
		}
		// content
		for (typename std::map<T, P>::iterator it = mp.begin(); it != mp.end(); ++it)
		{
//			T key = it->first;
			P obj = it->second;

			Buffer[idx++] = obj;
		}
		// after init
		for (int i = 0; i < offsetAfter; i++)
		{
			Buffer[idx + i] = P();
		}
		return Buffer;
	}
	return 0;
}

template<typename T>
inline void DeleteObjectsAndClearPointerList(list<T*> &lst)
{
	if (lst.size() > 0)
	{
		for (typename std::list<T*>::iterator it = lst.begin(); it != lst.end(); ++it)
		{
			T* type = 0;
			type = *it;
			if (type != 0)
			{
				delete type;
			}
		}
		lst.clear();
	}
}


#endif /* _TOOLS_H_ */