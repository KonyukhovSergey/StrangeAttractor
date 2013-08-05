package ru.jauseg.strangeattractor;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.ViewGroup;

public class StrangeAttractorActivity extends Activity
{
	private GLSurfaceView engine;
	private StrangeAttractorRenderer renderer;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		engine = new GLSurfaceView(this);
		renderer = new StrangeAttractorRenderer(this);
		engine.setRenderer(renderer);
		engine.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
		setContentView(engine);
	}
}
