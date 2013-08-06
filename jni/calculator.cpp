#include <jni.h>
#include <math.h>
#include <stdlib.h>
#include <android/log.h>


extern "C"
{
JNIEXPORT void JNICALL Java_js_jni_code_NativeCalls_calculate(JNIEnv *env,
		jclass, jobject vertexCoords, jint length, jfloat time);
}

float rndf()
{
	return (float) (rand() & 0xffffff) / (float) 0xffffff;
}

struct Vector4D
{
	float a;
	float b;
	float c;
	float d;

	void init()
	{
		a = 0;
		b = 0;
		c = 0;
		d = 0;
	}

	void init(float a, float b, float c, float d)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
	}

	void init(Vector4D &v)
	{
		this->a = v.a;
		this->b = v.b;
		this->c = v.c;
		this->d = v.d;
	}

	void plus(float a, float b, float c, float d)
	{
		this->a += a;
		this->b += b;
		this->c += c;
		this->d += d;
	}

	void plus(Vector4D &v)
	{
		this->a += v.a;
		this->b += v.b;
		this->c += v.c;
		this->d += v.d;
	}

	void plus(Vector4D &v, float s)
	{
		this->a += v.a * s;
		this->b += v.b * s;
		this->c += v.c * s;
		this->d += v.d * s;
	}

	void scale(float s)
	{
		a *= s;
		b *= s;
		c *= s;
		d *= s;
	}

	void minus(Vector4D &a, Vector4D &b)
	{
		this->a = a.a - b.a;
		this->b = a.b - b.b;
		this->c = a.c - b.c;
		this->d = a.d - b.d;
	}

	float quadLen()
	{
		return a * a + b * b + c * c + d * d;
	}

};

struct StrangeState
{
	Vector4D p;
	Vector4D e;
	Vector4D v;
	Vector4D d;

	void init()
	{
		p.init(-0.966918f, 2.879879f, 0.765145f, 0.744728f);
		e.init(p);
		v.init();
	}

	void tick()
	{
		d.minus(e, p);

		if (d.quadLen() < 0.0001)
		{
			e.init(-0.966918f - 0.75f + 0.9f * rndf(),
					2.879879f - 0.0f + 1.0f * rndf(),
					0.765145f - 1.7f + 1.6f * rndf(),
					0.744728f - 0.5f + 0.6f * rndf());

			__android_log_print(ANDROID_LOG_VERBOSE, "StrangeAttractor", "new position");
		}

		v.plus(d, 0.01);
		p.plus(v, 0.01);
		v.scale(0.99);
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

//	float r = sin(time*0.1);
//
//	float a = -0.966918f;// - 0.3f + 0.45f * sinf(time*10); // -0.75 0.9
//	float b = 2.879879f;// +0.5f + 0.5f * sinf(time*10); // 0 1
//	float c = 0.765145f;// - 0.9f + 0.8f * sinf(time * 20); -1.7 1.6
//	float d = 0.744728f;// - 0.2f + 0.3f * sinf(time*20); -0.5 0.6


	float x = 10.0f;
	float y = 10.0f;

	float *pixels = pVertex;

	for (int i = 0; i < length; i += 2)
	{
		float xnew = sinf(y * s.p.b) + s.p.c * sinf(x * s.p.b);
		float ynew = sinf(x * s.p.a) + s.p.d * sinf(y * s.p.a);
//		float xnew = sinf(y * b) + c * sinf(x * b);
//		float ynew = sinf(x * a) + d * sinf(y * a);

		x = xnew;
		y = ynew;

		*pixels = x;
		pixels++;
		*pixels = y;
		pixels++;
	}

}
