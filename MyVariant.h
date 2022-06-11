#pragma once

struct Vector4 
{
	float x, y, z, w;
};

enum VARIANT_TYPE
{
	VT_NONE,
	VT_BOOL,
	VT_BYTE,
	VT_SHORT,
	VT_INT,
	VT_FLOAT,
	VT_DOUBLE,
	VT_STRING,
	VT_VECTOR4,
};

class MyVariant
{
public:
	VARIANT_TYPE _VTType = VARIANT_TYPE::VT_NONE;
	union DATATYPES
	{
		bool bVal;
		char byteVal;
		short shortVal;
		int intVal;
		float floatVal;
		double doubleVal;
		char* stringVal;
		float* vector4Val;
	};
};

