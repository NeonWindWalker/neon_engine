package ru.devgame;

import java.util.Locale;

import org.onepf.oms.OpenIabHelper;

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.net.Uri;
import android.os.Message;
import android.util.DisplayMetrics;
import android.util.Log;
import android.widget.Toast;

public class NativeLib
{
	static
	{
		System.loadLibrary("mynativelib");
	}
	
	public native void setLogPath(String path);
	public native boolean init(Object renderIniter, Object assetReader, String dataBaseName, String homeDir, float pixelSize);
	public native boolean step();
	public native void stepFinish();
	public native void deinit();
	
	public native void renderWindowChanged(Object surfaceHolder, int width, int height);
	
	public native void onTouch(int id, float x, float y, float width, float height, float pressure, float size, boolean down, boolean up);
	public native boolean onKey(int key, boolean down, boolean up);
	public native void onAccelerometer(int id, float x, float y, float z);
	public native void onItemPurchasableInfo(String item, boolean purchased, String price);
	public native void onItemPurchased(String item);
	public native void onConnectedToBillingServer();
	
	public MainView mainView;
	public MyActivity activity;
	public void setDensity(float f)
	{
		mainView.setDensity(f);
	}
	
	public int getScreenSize()
	{
		DisplayMetrics metrics = new DisplayMetrics();
		activity.getWindowManager().getDefaultDisplay().getMetrics(metrics);
		return metrics.widthPixels | (metrics.heightPixels << 16);
	}
	
	public String getDeviceName()
	{
		return android.os.Build.MODEL;
	}
	
	public void connectToBillingServerRequest() 
	{
		if(activity.billingInConnecionProcess)
			return;
		
		Message msg = new Message();
		activity.ConnectToBillingMessage.sendMessage(msg);
	}
	
	public void restorePurchases() 
	{
		Message msg = new Message();
		activity.ConnectToBillingMessage.sendMessage(msg);
	}
	
	public void doPurchase(String item)
	{
		Message msg = new Message();
		msg.obj = item;
		activity.DoPurchaseMessage.sendMessage(msg);
	}
	
	public void openURL(String url)
	{
		Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
		activity.startActivity(browserIntent);
	}
	
	public void openRateUs()
	{
		activity.OpenRateUsMessage.sendMessage(new Message());
	}
	
	public String getLanguage()
	{
		String lang = Locale.getDefault().getLanguage();
		return lang;
	}
}
