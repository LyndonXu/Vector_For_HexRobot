#include "vector.h"


int32_t Vector2DAdd(St2DVector *pA, St2DVector *pB, St2DVector *pOut)
{
	if (pA == NULL || pB == NULL || pOut == NULL)
	{
		return -1;
	}
	pOut->x = pA->x + pB->x;
	pOut->y = pA->y + pB->y;

	return 0;
}

int32_t Vector2DSub(St2DVector *pA, St2DVector *pB, St2DVector *pOut)
{
	if (pA == NULL || pB == NULL || pOut == NULL)
	{
		return -1;
	}

	pOut->x = pA->x - pB->x;
	pOut->y = pA->y - pB->y;

	return 0;
}

int32_t Vector2DRotate(St2DVector *pA, float f32Radian, St2DVector *pOut)
{
	if (pA == NULL || pOut == NULL)
	{
		return -1;
	}
	else
	{
		float x = pA->x * cosf(f32Radian) - pA->y * sinf(f32Radian);
		float y = pA->x * sinf(f32Radian) + pA->y * cosf(f32Radian);
		pOut->x = x;
		pOut->y = y;
	}

	return 0;
}

int32_t Vector2DGetModule(St2DVector *pA, float *pModule)
{
	if (pA == NULL || pModule == NULL)
	{
		return -1;
	}

	pModule[0] = sqrtf(pA->x * pA->x + pA->y * pA->y);

	return 0;
}
