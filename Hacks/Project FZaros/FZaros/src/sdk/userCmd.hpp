#pragma once
#include "vector.hpp"

class CViewSetup
{
public:
	__int32   x;                  //0x0000 
	__int32   x_old;              //0x0004 
	__int32   y;                  //0x0008 
	__int32   y_old;              //0x000C 
	__int32   width;              //0x0010 
	__int32   width_old;          //0x0014 
	__int32   height;             //0x0018 
	__int32   height_old;         //0x001C 
	char      pad_0x0020[0x90];   //0x0020
	float     fov;                //0x00B0 
	float     viewmodel_fov;      //0x00B4 
	Vector3    origin;             //0x00B8 
	Vector3    angles;             //0x00C4 
	char      pad_0x00D0[0x7C];   //0x00D0

};//Size=0x014C

class CUserCmd
{
public:
	char __PAD000[4];

	int commandNumber;
	int tickCount;

	Vector3 viewAngles;
	Vector3 aimDirection;
	float forwardmove; //x
	float sidemove; // y
	float upmove; //z


	int buttons;
	char    impulse;       
	int     weaponselect;  
	int     weaponsubtype; 
	int     randomSeed;   

	short mouseX;
	short mouseY;

	bool hasbeenPredicted;

	char __PAD002[25];

};