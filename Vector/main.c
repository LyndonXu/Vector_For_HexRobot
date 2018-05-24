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


// 		printf("dis: % 8.2f, [%d] union foot (x: % 8.2f, y: % 8.2f), ", f32Distance, i,
// 			stFootOut[i].x, stFootOut[i].y);

		//Vector2DSub(stFootOut + i, c_stHexBase + i, stFootOut + i);
		//Vector2DSub(stFootOut + i, &stRO, stFootOut + i);
		//Vector2DRotate(stFootOut + i, -c_f32HexBaseRadian[i], stFootOut + i);
		//if (i <= _Base_Point_LeftDown)
		//{
		//	Vector2DRotate(stFootOut + i, M_PI, stFootOut + i);
		//}

// 		printf("real foot (x: % 8.2f, y: % 8.2f)\n",stFootOut[i].x, stFootOut[i].y);

	}

	return 0;
}


int main()
{
	St2DVector stFoot[_Base_Point_Reserved];
	{
		float f32MaxDis = 60.0f;
		float f32StepDis = 5.0f;
		float f32ProcessInit[4] =
		{
			-1.0f * f32StepDis,
			-(f32MaxDis / f32StepDis - 1.0f) * f32StepDis,
			1.0f * f32StepDis,
			(f32MaxDis / f32StepDis - 1.0f) * f32StepDis,
		};
		bool boStepDirection[4] =
		{
			false, true, true, false,
		};
		bool boIsAdvace = false;
		int32_t s32StepCounts = (int32_t)(f32MaxDis / f32StepDis);
		int32_t s32Process = -1;
		int32_t s32CurStep = -1;

		float f32Dis;
		bool boDir;
		while (true)
		{

			if (((s32Process == 0 || s32Process == 2) && (s32CurStep == s32StepCounts))
				|| (s32Process < 0 || s32Process > 4))
			{
				/* update the commond(direction and radius) */
				printf("input the commond:\n");
				int32_t c = getchar();
				getchar();
				if (c == 'f')
				{
					boIsAdvace = true;
				}
				else if (c == 'b')
				{
					boIsAdvace = false;
				}
				else if (c != 'c')
				{
					s32Process = -1;
					continue;
				}
				if (c != 'c')
				{
					s32Process = 0;
					s32CurStep = s32StepCounts;
					f32Dis = f32ProcessInit[s32Process];
					boDir = boStepDirection[s32Process];

					if (!boIsAdvace)
					{
						f32Dis = -f32Dis;
						boDir = !boDir;
					}
				}
			}

			if (s32CurStep <= 0)
			{
				s32Process++;
				if (s32Process >= 4)
				{
					s32Process = 0;
				}

				s32CurStep = s32StepCounts;
				f32Dis = f32ProcessInit[s32Process];
				boDir = boStepDirection[s32Process];

				if (!boIsAdvace)
				{
					f32Dis = -f32Dis;
					boDir = !boDir;
				}

				printf("\n\n\n");
				continue;/* check the command again */
			}

			s32CurStep--;
			GetStepPointBase(Base_Point_LeftUp_Flag |
				Base_Point_Right_Flag |
				Base_Point_LeftDown_Flag,
				_Base_Point_Right,
				-150000.0f, f32Dis, 87.75f, stFoot);

			GetStepPointBase(Base_Point_RightUp_Flag |
				Base_Point_Left_Flag |
				Base_Point_RightDown_Flag,
				_Base_Point_Right,
				-150000.0f, -f32Dis, 87.75f, stFoot);
			if (boDir)
			{
				f32Dis = f32Dis + f32StepDis;
			}
			else
			{
				f32Dis = f32Dis - f32StepDis;
			}
			{
				int32_t m;
				for (m = 0; m < _Base_Point_Reserved; m++)
				{
					printf("(% 8.2f, % 8.2f), ",
						stFoot[m].x, stFoot[m].y);
				}
				printf("\n");
			}
		}
	}
	getchar();
	return 0;
}
