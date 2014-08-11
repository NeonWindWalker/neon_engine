package ru.devgame;


import java.lang.Thread;
import java.lang.Runnable;
import java.util.ArrayList;

import android.app.Activity;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.util.DisplayMetrics;
import android.view.Surface;
import android.view.SurfaceHolder;

import ru.devgame.NativeLib;
import ru.devgame.MainView;

public class Main extends Thread 
{
	public volatile boolean exitFlag = false;
	public volatile boolean pause = false;
	public volatile boolean pauseAccepted = false;
	public volatile boolean destroyed = false;
	public volatile boolean noCallFinishOnThreadEnd = false;
	
	public NativeLib nativeLib;
	RenderIniter renderIniter;
	AssetsReader assetReader;
	String packageResourcePath;
	String homeDirPath;
	float pixelSize;
	
	MyActivity activity;
	
	public Main(MyActivity activity, NativeLib inNativeLib, AssetsReader assetReader, String logPath, String inPackageResourcePath, String inHomeDirPath, float pixelSize)
	{
		this.activity = activity;
		nativeLib = inNativeLib;
		renderIniter = new RenderIniter();
		this.assetReader = assetReader;
		nativeLib.setLogPath(logPath);
		packageResourcePath = inPackageResourcePath;
		homeDirPath = inHomeDirPath;
		this.pixelSize = pixelSize;
		setName("my main thread");
	}

	
	@Override public void run()
	{	
		if(!nativeLib.init(renderIniter, assetReader, packageResourcePath, homeDirPath, pixelSize))
			exitFlag = true;
		
		boolean stepFinishCalled = false;
		while(!exitFlag)
		{
			if(!pause)
			{			
				exitFlag |= !nativeLib.step();
				stepFinishCalled = false;
			}
			else
			{
				if(!stepFinishCalled)
				{
					nativeLib.stepFinish();
					stepFinishCalled = true;
				}
				
				pauseAccepted = true;
				
				try{ Thread.currentThread().sleep(200); } catch(Exception e){}
			}
		}
		if(!stepFinishCalled)
			nativeLib.stepFinish();
		nativeLib.deinit();
		destroyed = true;
		if(!noCallFinishOnThreadEnd)
			activity.callMyDestroyEvent();
	}	
}
