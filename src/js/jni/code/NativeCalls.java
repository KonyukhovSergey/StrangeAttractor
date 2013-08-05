package js.jni.code;

import java.nio.Buffer;

public class NativeCalls
{
	public static native int calculate(Buffer vertexCoords, int length,float time);

	static
	{
		System.loadLibrary("calculator");
	}
}
