package ru.devgame;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL10;

import android.view.SurfaceHolder;

public class RenderWindowIniter
{
	RenderIniter renderIniter;
	boolean mainWindow;
	
	SurfaceHolder surfaceHolder;
	EGLContext context;
	EGLSurface surface;
	int surfaceHolderRealWidth = 0;
	int surfaceHolderRealHeight = 0;

	public RenderWindowIniter(RenderIniter renderIniter, boolean mainWindow)
	{
		this.renderIniter = renderIniter;
		this.mainWindow = mainWindow;
		if(mainWindow)
		{
			surfaceHolder = renderIniter.surfaceHolder;
			context = renderIniter.context;
			surface = renderIniter.surface;
			surfaceHolderRealWidth = renderIniter.surfaceHolderRealWidth;
			surfaceHolderRealHeight = renderIniter.surfaceHolderRealHeight;
		}
	}
	
	public void Destroy()
	{
		deinit();
	}
	
	
	public int init(Object surHolder, int width, int height, boolean fullscreen, boolean stereo)
	{
		deinit();
		
		if(mainWindow)
		{
			if(surface != null){
				renderIniter.egl.eglMakeCurrent(renderIniter.display, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
				renderIniter.egl.eglDestroySurface(renderIniter.display, surface);
				surface = null;
			}
			renderIniter.surface = null;
		}
		else
			deinit();
		
		String excname;
	    try {
	    	surface = renderIniter.egl.eglCreateWindowSurface(renderIniter.display, renderIniter.config, surfaceHolder, null);
	    	 
	    } catch (Exception t) {
	    	excname = t.toString();
	    }
	    
	    
	    if(mainWindow)
		{
			renderIniter.surface = surface;
			renderIniter.egl.eglMakeCurrent(renderIniter.display, surface, surface, context);
		}
	    surfaceHolderRealWidth = width;
		surfaceHolderRealHeight = height;
	    return surfaceHolderRealWidth + (surfaceHolderRealHeight << 16);
	}
	
	
	public int present()
	{
		if(surface == null)
			return 0;
		
		renderIniter.egl.eglSwapBuffers(renderIniter.display, surface);
		return surfaceHolderRealWidth + (surfaceHolderRealHeight << 16);
	}
	
	public boolean makeCurrentContext()
	{
		return renderIniter.makeCurrentContextCashed(surface, context);
	}
	
	public boolean deinit()
	{
		if(mainWindow)
			return true;
		
		if(surface != null){
			renderIniter.egl.eglMakeCurrent(renderIniter.display, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
			renderIniter.egl.eglDestroySurface(renderIniter.display, surface);
			surface = null;
		}
		if (context != null) {
			renderIniter.egl.eglDestroyContext(renderIniter.display, context);
            context = null;
        }
        return true;
	}
}