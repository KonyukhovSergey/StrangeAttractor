package ru.jauseg.strangeattractor;

import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import js.engine.BufferAllocator;
import js.engine.FrameRateCalculator;
import js.engine.FrameRateCalculator.FrameRateUpdateInterface;
import js.engine.QuadBuffer;
import js.gesture.FlingDetector.FlingDetectorListener;
import js.jni.code.NativeCalls;
import js.utils.TimeCounter;
import net.rbgrn.android.glwallpaperservice.GLWallpaperService;
import android.content.Context;
import android.opengl.GLU;
import android.os.SystemClock;
import android.util.Log;

public class StrangeAttractorRenderer implements GLWallpaperService.Renderer, FlingDetectorListener,
		FrameRateUpdateInterface
{
	private static final int VERTEX_NUMBER = 50000;

	private static final String TAG = "StrangeAttractorRenderer";

	private float width;
	private float height;
	private FrameRateCalculator fps;

	private TimeCounter timeCounter;

	private QuadBuffer quadBuffer;

	private FloatBuffer vertex;

	public StrangeAttractorRenderer(Context strangeAttractorActivity)
	{
		fps = new FrameRateCalculator(this);
		timeCounter = new TimeCounter();
		timeCounter.start();
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		gl.glDisable(GL10.GL_DEPTH_TEST);
		gl.glDisable(GL10.GL_LIGHTING);
		gl.glDisable(GL10.GL_CULL_FACE);
		gl.glDisable(GL10.GL_DITHER);

		gl.glEnable(GL10.GL_BLEND);
		gl.glBlendFunc(GL10.GL_SRC_ALPHA, GL10.GL_ONE);

		gl.glDisable(GL10.GL_TEXTURE_2D);

		gl.glShadeModel(GL10.GL_SMOOTH);
		gl.glHint(GL10.GL_PERSPECTIVE_CORRECTION_HINT, GL10.GL_NICEST);

		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);

		gl.glClearColor(0, 0, 0, 0);

		vertex = BufferAllocator.createFloatBuffer(VERTEX_NUMBER);

	}

	private float time()
	{
		return (float) ((float) (SystemClock.elapsedRealtime() & 0xfffffff) / 20000.0f);
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height)
	{
		this.width = width;
		this.height = height;

		gl.glMatrixMode(GL10.GL_PROJECTION);
		gl.glLoadIdentity();

		gl.glViewport(0, 0, width, height);
		// GLU.gluOrtho2D(gl, -1.75f, 1.75f, 2, -2);
		GLU.gluOrtho2D(gl, -1.5f, 1.5f, 1.5f, -1.5f);

		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		// gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY);

		quadBuffer = new QuadBuffer(-2.1f, -2.1f, 2.1f, 2.1f);
	}

	@Override
	public void onDrawFrame(GL10 gl)
	{
		fps.frameBegin();

		gl.glColor4f(0.0f, 0.0f, 0.0f, 0.1f);

		gl.glVertexPointer(2, GL10.GL_FLOAT, 0, quadBuffer.buffer);
		gl.glBlendFunc(GL10.GL_SRC_ALPHA, GL10.GL_ONE_MINUS_SRC_ALPHA);

		gl.glDrawArrays(GL10.GL_TRIANGLE_FAN, 0, 4);

		gl.glBlendFunc(GL10.GL_SRC_ALPHA, GL10.GL_ONE);
		gl.glColor4f(
				(float) (1.0f + 0.5f * Math.sin(time() * 5.0f)),
				(float) (1.0f + 0.5f * Math.sin(time() * 7.0f)),
				(float) (1.0f + 0.5f * Math.sin(time() * 11.0f)), 0.2f);

		gl.glPointSize(1.0f);

		gl.glVertexPointer(2, GL10.GL_FLOAT, 0, vertex);

		NativeCalls.calculate(vertex, VERTEX_NUMBER, time());
		gl.glDrawArrays(GL10.GL_POINTS, 0, VERTEX_NUMBER >> 1);

		fps.frameDone();
		// SystemClock.sleep(10);

	}

	@Override
	public void onFrameRateUpdate(FrameRateCalculator frameRateCalculator)
	{
		Log.v(TAG, frameRateCalculator.frameString());
	}

	@Override
	public void onFling(float fromX, float fromY, float toX, float toY, int deltaTime)
	{
		// TODO Auto-generated method stub

	}
}
