#include <jni.h>
#include <math.h>
#include <stdlib.h>

extern "C"
{
JNIEXPORT void JNICALL Java_js_jni_code_NativeCalls_calculate(JNIEnv *env,
		jclass, jobject vertexCoords, jint length, jfloat time);
}

float rndf()
{
	return (float) (rand() & 0xffffff) / (float) 0xffffff;
}

struct StrangeState
{
	float a;
	float b;
	float c;
	float d;

	int count;
	StrangeState* direction;

	void calculateDirections(StrangeState* p, StrangeState* q)
	{
		a = q->a - p->a;
		b = q->b - p->b;
		c = q->c - p->c;
		d = q->d - p->d;

		float l = sqrtf(a * a + b * b + c * c + d * d);

		if (l > 0.001f)
		{
			count = l / 0.005f;
			l = (1.0f / l) * 0.005f;
			a *= l;
			b *= l;
			c *= l;
			d *= l;
		}
	}

	void init()
	{
		a = -0.966918f;
		b = 2.879879f;
		c = 0.765145f;
		d = 0.744728f;
		count = 0;
		direction = new StrangeState();
	}

	void tick()
	{
		if (count > 0)
		{
			count--;
			a += direction->a;
			b += direction->b;
			c += direction->c;
			d += direction->d;
		}
		else
		{
			StrangeState ss;
			ss.a = -0.966918f +0.3f - 0.6f * rndf();
			ss.b = 2.879879f - 0.3f * rndf();
			ss.c = 0.765145f - 0.2f * rndf();
			ss.d = 0.744728f - 0.2f * rndf();

			direction->calculateDirections(this, &ss);
			count = direction->count;
		}
	}
};

StrangeState s;
bool firstCall = true;

JNIEXPORT void JNICALL Java_js_jni_code_NativeCalls_calculate(JNIEnv *env,
		jclass, jobject vertexCoords, jint length, jfloat time)
{
	if (firstCall)
	{
		s.init();
		firstCall = false;
	}
	else
	{
		s.tick();
	}
	float* pVertex = (float*) env->GetDirectBufferAddress(vertexCoords);

	float a = -0.966918f - 0.3f + 0.5f * sinf(time + 1.0f);
	float b = 2.879879f + 1.1f + 0.8f * sinf(time * 2.0f + 3.0f);
	float c = 0.765145f - 0.3f + 0.2f * sinf(time * 3.0f + 5.0f);
	float d = 0.744728f - 0.3f + 0.4f * sinf(time * 1.4f + 7.0f);

	float x = 10.0f;
	float y = 10.0f;

	float *pixels = pVertex;

	for (int i = 0; i < length; i += 2)
	{
//		float xnew = sinf(y * s.b) + s.c * sinf(x * s.b);
//		float ynew = sinf(x * s.a) + s.d * sinf(y * s.a);
		float xnew = sinf(y * b) + c * sinf(x * b);
		float ynew = sinf(x * a) + d * sinf(y * a);

		x = xnew;
		y = ynew;

		*pixels = x;
		pixels++;
		*pixels = y;
		pixels++;
	}

}
