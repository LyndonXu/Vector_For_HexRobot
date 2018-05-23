#include <stdio.h>
#include "vector.h"

enum
{
	_Base_Point_LeftUp = 0,
	_Base_Point_Left,
	_Base_Point_LeftDown,
	_Base_Point_RightUp,
	_Base_Point_Right,
	_Base_Point_RightDown,

	_Base_Point_Reserved,
};

#define Base_Point_LeftUp_Flag			0x01
#define Base_Point_Left_Flag			0x02
#define Base_Point_LeftDown_Flag		0x04
#define Base_Point_RightUp_Flag			0x08
#define Base_Point_Right_Flag			0x10
#define Base_Point_RightDown_Flag		0x20


const St2DVector c_stHexBase[_Base_Point_Reserved] =
{
	{ -40.0f, 75.0f, },
	{ -65.0f, 0.0f, },
	{ -40.0f, -75.0f, },
	{ 40.0f, 75.0f, },
	{ 65.0f, 0.0f, },
	{ 40.0f, -75.0f, },
};

const float c_f32HexBaseRadian[_Base_Point_Reserved] =
{
	(180.0f - 61.927513f) * M_PI / 180.0f,
	M_PI,
	-(180.0f - 61.927513f) * M_PI / 180.0f,
	(61.927513f) * M_PI / 180.0f,
	0.0f,
	-(61.927513f) * M_PI / 180.0f,
};


int32_t GetStepPointBase(int32_t s32PointFlag, 
	int32_t s32BasePoint, float f32Radius, 
	float f32Distance, 	
	float f32Leg, 
	St2DVector stFootOut[_Base_Point_Reserved])
{
	St2DVector stRO = { 0.0f - f32Radius, 0.0f };
	St2DVector stFoot = { 0.0f, 0.0f };
	bool boIsStraight = false;

	float f32BaseMoudle = 0.0f;

	int32_t i;
	if (s32PointFlag == 0 || s32BasePoint >= _Base_Point_Reserved || s32BasePoint < 0)
	{
		return -1;
	}

	if (f32Radius > 100.0f * 1000.0f ||
		f32Radius < -100.0f * 1000.0f)	/* 100m */
	{
		boIsStraight = true;
	}
	if (boIsStraight)
	{
		stRO.x = 0.0f;
	}
	else
	{
		St2DVector stRLB = { 0.0f, 0.0f };	/* origin to foot */

		stFoot.x = f32Leg * cosf(c_f32HexBaseRadian[s32BasePoint]);
		stFoot.y = f32Leg * sinf(c_f32HexBaseRadian[s32BasePoint]);

		/* base vector */
		Vector2DAdd(&stRO, c_stHexBase + s32BasePoint, &stRLB);		/* add the base */
		Vector2DAdd(&stRLB, &stFoot, &stRLB);						/* add the foot */

		Vector2DGetModule(&stRLB, &f32BaseMoudle);
	}

	
	for (i = 0; i < _Base_Point_Reserved; i++)
	{
		St2DVector stRL = { 0.0f, 0.0f };
		float f32Moudle = 0.0f;
		if ((s32PointFlag & (1 << i)) == 0)
		{
			continue;
		}

		stFoot.x = f32Leg * cosf(c_f32HexBaseRadian[i]);
		stFoot.y = f32Leg * sinf(c_f32HexBaseRadian[i]);



		Vector2DAdd(&stRO, c_stHexBase + i, &stRL);		/* add the base */
		Vector2DAdd(&stRL, &stFoot, &stRL);				/* add the foot */

		if (boIsStraight)
		{ 
			stFootOut[i].y = stRL.y + f32Distance;
			stFootOut[i].x = stRL.x;
		}
		else
		{
			float f32Radian = 0.0f;
			Vector2DGetModule(&stRL, &f32Moudle);
			f32Radian = (f32Distance / f32BaseMoudle) * f32Moudle / f32BaseMoudle;
			Vector2DRotate(&stRL, f32Radian, stFootOut + i);
		}


		printf("dis: % 8.2f, [%d] union foot (x: % 8.2f, y: % 8.2f), ", f32Distance, i,
			stFootOut[i].x, stFootOut[i].y);

		Vector2DSub(stFootOut + i, c_stHexBase + i, stFootOut + i);
		Vector2DSub(stFootOut + i, &stRO, stFootOut + i);
		Vector2DRotate(stFootOut + i, -c_f32HexBaseRadian[i], stFootOut + i);
		if (i <= _Base_Point_LeftDown)
		{
			//Vector2DRotate(stFootOut + i, M_PI, stFootOut + i);
		}

		printf("real foot (x: % 8.2f, y: % 8.2f)\n",stFootOut[i].x, stFootOut[i].y);

	}

	return 0;
}


int main()
{
	St2DVector stFoot[_Base_Point_Reserved];
	int32_t j;
	for (j = 0; j < _Base_Point_Reserved; j++)
	{
		float f32Dis = 60.0f;
		while (f32Dis > -60.001f)
		{
			GetStepPointBase(1 << j, _Base_Point_Right,
				-150000.0f, f32Dis, 87.75f, stFoot);
			f32Dis = f32Dis - 5.0f;
		}
		printf("\n\n\n");
	}

	getchar();
	return 0;
}
