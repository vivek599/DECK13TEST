#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct Matrix
{
	float a[16];
};

struct Vector3
{
	float x, y, z;
};

struct Quaternion
{
	float x, y, z, w;
};

struct CVector3
{
	uint16_t x[3];
};

struct CQuaternion
{
	uint16_t x[3];
};
 
struct BoneName
{
	string	Name;
	int		Id;
	Matrix	Transform;
};

struct AnimationKey
{
	float			timePos;
	Vector3			pos;
	Quaternion		rot;
	Vector3			scale;
};

struct CompressedAnimationKey
{
	uint16_t 			timePos;//0-0.0f, 65535-1.0f
	CVector3			pos;
	CQuaternion			rot;
	CVector3			scale;
};

struct AnimationTrack
{
	vector<AnimationKey> animationKeys;
};

class Animation
{
public:

private:
	map<BoneName, AnimationTrack*> perBoneAnimationTracks;

};

