package ru.devgame;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL10;

import android.view.SurfaceHolder;

public class RenderIniter
{
	public SurfaceHolder surfaceHolder;
	public int surfaceHolderRealWidth = 0;
	public int surfaceHolderRealHeight = 0;
	public EGL10 egl;
	public GL10 gl;
	public EGLDisplay display;
	public EGLConfig config;
	public EGLContext context;
	public EGLSurface surface;
		
	public static int EGL_OPENGL_ES2_BIT = 4;
	public static int EGL_OPENGL_ES_BIT = 1;
	public static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
    
	public RenderIniter()
	{
	}

	public Object init()
	{
		return null;
	}
	
	
	public Object initWithMainWindow(Object surHolder, int width, int height, boolean fullscreen, boolean stereo)
	{
		surfaceHolder = (SurfaceHolder)surHolder;
		if(surfaceHolder == null)
			return null;
		
		egl = (EGL10)EGLContext.getEGL();
		display = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);
		
		int[] major_minor = new int[2];
		egl.eglInitialize(display, major_minor);
		
		// Locate an 8/8/8 RGB configuration
	    int[] configAttrs = { EGL10.EGL_RED_SIZE, 8,
	                           EGL10.EGL_GREEN_SIZE, 8,
	                           EGL10.EGL_BLUE_SIZE, 8,
	                           //EGL10.EGL_ALPHA_SIZE, EGL10.EGL_DONT_CARE,
	                           EGL10.EGL_DEPTH_SIZE, 16,
	                           //EGL10.EGL_STENCIL_SIZE, EGL10.EGL_DONT_CARE,
	                           EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
	                           EGL10.EGL_NONE};
	    
	    int[] num_config = new int[1];
	    EGLConfig[] eglConfigs = new EGLConfig[1];
	    egl.eglChooseConfig(display, configAttrs, eglConfigs, 1, num_config);
	    config = eglConfigs[0];
	    
	    int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
	    context = egl.eglCreateContext(display, config, EGL10.EGL_NO_CONTEXT, attrib_list);

		String excname;
	    try {
	    	surface = egl.eglCreateWindowSurface(display, config, surfaceHolder, null);
	    	 
	    } catch (Exception t) {
	    	excname = t.toString();
	    }
	    
	    egl.eglMakeCurrent(display, surface, surface, context);
	    gl = (GL10)context.getGL();
	    surfaceHolderRealWidth = width;
		surfaceHolderRealHeight = height;
		return surfaceHolder;
	}
	
	public boolean deinit()
	{
		if(surface != null){
			egl.eglMakeCurrent(display, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
			egl.eglDestroySurface(display, surface);
			surface = null;
		}
		if (context != null) {
            egl.eglDestroyContext(display, context);
            context = null;
        }
        if (display != null) {
        	egl.eglTerminate(display);
        	display = null;
        }
        return true;
	}
	
	
	public boolean makeCurrentContext()
	{
		return makeCurrentContextCashed(surface, context);
	}
	
	EGLSurface makeCurrentContextCashed_surface;
	EGLContext makeCurrentContextCashed_context;
	
	public boolean makeCurrentContextCashed(EGLSurface surf, EGLContext con)
	{
		if(makeCurrentContextCashed_surface == surf && makeCurrentContextCashed_context == con)
			return true;
		makeCurrentContextCashed_surface = surf;
		makeCurrentContextCashed_context = con;
		return egl.eglMakeCurrent(display, surf, surf, con);
	}
	
	public boolean setSyncIntrval(int n)
	{
		//egl.eglSwapInterval();
		return false;
	}
	
	
	public Object newWindowIniter(boolean mainWindowSurface)
	{
		return new RenderWindowIniter(this, mainWindowSurface);
	}
	
	public boolean deleteWindowIniter(Object obj)
	{
		((RenderWindowIniter)obj).Destroy();
		return true;
	}
}