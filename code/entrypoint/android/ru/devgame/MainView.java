package ru.devgame;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.PixelFormat;
import android.os.Handler;
import android.os.Message;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup.LayoutParams;

public class MainView extends SurfaceView implements SurfaceHolder.Callback
{
	Main main;
	
	public MainView(Context context, Main main)
	{
		super(context);
		this.main = main;
		getHolder().addCallback(this);
		getHolder().setFormat(PixelFormat.RGBA_8888);
		getHolder().setType(android.view.SurfaceHolder.SURFACE_TYPE_GPU);
	}
	
	public void surfaceCreated(SurfaceHolder holder)
	{
    }

	public void surfaceDestroyed(SurfaceHolder holder)
	{
    }
	
	private float density = 1.0f;
	private int renderWidth = -1;
	private int renderHeight = -1;
	
	public void setDensity(float d)
	{
		Message msg = new Message();
		msg.obj = d;
		DensityChanged.sendMessage(msg);
	}
	
	public float getDensity()
	{
		return density;
	}
	
	public Handler DensityChanged = new Handler()
    {
        public void handleMessage(Message msg)
        {
        	density = (Float)msg.obj;
        	if(getHolder().getSurface().isValid())
        	{
        		int w = getHolder().getSurfaceFrame().width();
            	int h = getHolder().getSurfaceFrame().height();
            	setSize((int)((float)w * density), (int)((float)h * density));
        	}
        }
    };
    
    private void setSize(int w, int h)
    {
    	int step = 4;
    	renderWidth = (w + step/2) / step * step;
    	renderHeight = (h + step/2) / step * step;
    	getHolder().setFixedSize(renderWidth, renderHeight);
    }
	    
	@Override
    protected void onSizeChanged(int xNew, int yNew, int xOld, int yOld)
    {
        super.onSizeChanged(xNew, yNew, xOld, yOld);
        setSize((int)((float)xNew * density), (int)((float)yNew * density));
    	getHolder().setFixedSize(renderWidth, renderHeight);
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h)
    {
		main.nativeLib.renderWindowChanged(getHolder().getSurface().isValid() ? getHolder() : null, renderWidth, renderHeight);
    }
	
	@Override public boolean onTouchEvent(MotionEvent ev)
	{		
        int histCount = ev.getHistorySize();
        final int pointerIndex = (ev.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) 
                >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
                
        final int action = ev.getAction();
        switch (action & MotionEvent.ACTION_MASK) 
        {
        
        case MotionEvent.ACTION_DOWN:
        case MotionEvent.ACTION_POINTER_DOWN:
        {
            main.nativeLib.onTouch(ev.getPointerId(pointerIndex), ev.getX(pointerIndex) * density, ev.getY(pointerIndex) * density, getWidth() * density, getHeight() * density, ev.getPressure(pointerIndex), ev.getSize(pointerIndex), true, false);
            break;
        }
           
        case MotionEvent.ACTION_MOVE:
        {
        	for (int i=0; i < ev.getPointerCount(); i++)
        		main.nativeLib.onTouch(ev.getPointerId(i), ev.getX(i) * density, ev.getY(i) * density, getWidth() * density, getHeight() * density, ev.getPressure(i), ev.getSize(i), false, false);        	
        	break;
        }
        
        case MotionEvent.ACTION_POINTER_UP:
        case MotionEvent.ACTION_UP:
        {
        	main.nativeLib.onTouch(ev.getPointerId(pointerIndex), ev.getX(pointerIndex) * density, ev.getY(pointerIndex) * density, getWidth() * density, getHeight() * density, ev.getPressure(pointerIndex), ev.getSize(pointerIndex), false, true);
            break;
        }
        
        case MotionEvent.ACTION_CANCEL:
            break;
        }
        
        //try{ Thread.currentThread().sleep(50); } catch(Exception e){}
        return true;
    }
}
