#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define _USE_MATH_DEFINES
#include <math.h>


typedef struct _tagSt2DVector
{
	float x;
	float y;
}St2DVector;
int32_t Vector2DAdd(St2DVector *pA, St2DVector *pB, St2DVector *pOut);
int32_t Vector2DSub(St2DVector *pA, St2DVector *pB, St2DVector *pOut);


int32_t Vector2DRotate(St2DVector *pA, float f32Radian, St2DVector *pOut);
int32_t Vector2DGetModule(St2DVector *pA, float *pModule);

#endif // _VECTOR_H_
