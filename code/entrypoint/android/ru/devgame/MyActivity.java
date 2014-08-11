package ru.devgame;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.onepf.oms.OpenIabHelper;
import org.onepf.oms.appstore.googleUtils.IabHelper;
import org.onepf.oms.appstore.googleUtils.IabResult;
import org.onepf.oms.appstore.googleUtils.Inventory;
import org.onepf.oms.appstore.googleUtils.Purchase;
import org.onepf.oms.appstore.googleUtils.SkuDetails;

import ru.devgame.NativeLib;
import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Surface;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;
import android.widget.Toast;

public class MyActivity extends Activity implements SensorEventListener 
{
	protected String title = "Game";
	protected String packdir = "";
	protected String homedir = "";
	public HashMap<String, String> storeKeys = new HashMap<String, String>();
	protected List<String> additionalSkuList = new ArrayList<String>();
	protected boolean needAccelerometer = true;

	Main main;
	MainView view;
	NativeLib nativeLib;
	
	SensorManager sensorManager;
    
	OpenIabHelper mBillingHelper;
    
    public MyActivity()
    {
    	nativeLib = new NativeLib();
    	nativeLib.activity = this;
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
    	super.onCreate(savedInstanceState);
    	OpenIabHelper.enableDebugLogging(true);
    	
    	if(needAccelerometer)
    		sensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);
        
        DisplayMetrics metrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(metrics);
		float pixelSize = 0.025400f / metrics.xdpi;
    	
    	this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        
    	AssetsReader assetReader = new AssetsReader(getAssets(), "data/");
    	if(homedir.length() == 0)
    		homedir = getFilesDir().getPath();
    	
    	main = new Main(this, nativeLib, assetReader, homedir + "/log.txt", packdir, homedir, pixelSize);
    	//main.setPriority(Thread.MAX_PRIORITY);

    	view = new MainView(getApplication(), main);
    	nativeLib.mainView = view;
    	setContentView(view);
    	
    	main.start();
    }
    
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) 
    {
        if (mBillingHelper != null && !mBillingHelper.handleActivityResult(requestCode, resultCode, data)) {
            super.onActivityResult(requestCode, resultCode, data);
        }
        else {
            Log.i("ru.devgame", "onActivityResult handled by IABUtil.");
        }
    }
    
    @Override
    protected void onStart()
    {
    	super.onStart();
    }
    
    @Override
    protected void onResume()
    {
    	//setRequestedOrientation(getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT ? ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT : ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
    	super.onResume();
    	if(needAccelerometer)
    		sensorManager.registerListener(this, sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_FASTEST);
    	main.pause = false;
    	main.pauseAccepted = false;
    }
    
    @Override
    protected void onPause()
    {
    	main.pause = true;
    	if(needAccelerometer)
    		sensorManager.unregisterListener(this);
    	while(!main.pauseAccepted && !main.destroyed)
    	{
    		try{ Thread.currentThread().yield(); } catch(Exception e){}
    	}
    	super.onPause();
    	//setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
    }
    
    @Override
    public void onStop()
    {
    	super.onStop();
    }
    
    @Override
    public void onDestroy()
    {
    	main.exitFlag = true;
    	main.noCallFinishOnThreadEnd = true;
    	try { main.join(); } catch(Exception e){}
    	
    	super.onDestroy();
    	
    	if(mBillingHelper != null)
    		mBillingHelper.dispose();
    	mBillingHelper = null;
    	
    	Log.i("ru.devgame", "onDestroy() complite");
    	 
    	System.runFinalizersOnExit(true);
    	android.os.Process.killProcess(android.os.Process.myPid());
    }
    
    public Handler MyDestroyEvent = new Handler()
    {
        public void handleMessage(Message msg)
        {
        	finish();
        }
    };
    
    public void callMyDestroyEvent()
    {
    	MyDestroyEvent.sendMessage(new Message() );
    }
    
    @Override
    public void onConfigurationChanged(Configuration newConfig) {
		main.nativeLib.renderWindowChanged(null, 0, 0);
	    super.onConfigurationChanged(newConfig);
	    //view = new MainView(getApplication(), main);
	  	setContentView(view);
	  	view.refreshDrawableState();
    }
    
    public void onAccuracyChanged(Sensor sensor, int accuracy)
    {
    }

    public void onSensorChanged(SensorEvent event)
    {
    	if(event.sensor.getType() ==  Sensor.TYPE_ACCELEROMETER)
    		nativeLib.onAccelerometer(0, -event.values[1], event.values[0], event.values[2]);
    }
    
    @Override public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		return main.nativeLib.onKey(keyCode, true, false);
	}
	
	@Override public boolean onKeyUp(int keyCode, KeyEvent event)
	{
		return main.nativeLib.onKey(keyCode, false, true);
	}
	
	public volatile boolean billingInConnecionProcess = false;
	private boolean billingSetupComplited = false;
	
	public void connectToBilling() {
		if (billingInConnecionProcess)
			return;
		
		billingInConnecionProcess = true;

		try
    	{
			if (mBillingHelper == null) {
				OpenIabHelper.Options options = new OpenIabHelper.Options();
				options.storeKeys = storeKeys;
				options.verifyMode = OpenIabHelper.Options.VERIFY_EVERYTHING;
				mBillingHelper = new OpenIabHelper(this, options);
			}
			
			if(!billingSetupComplited) {
				mBillingHelper.startSetup(setupFinishedListener);
			} else  {
				mBillingHelper.queryInventoryAsync(true, additionalSkuList, mGotInventoryListener);
			}
    	}
    	catch(Exception ex)
    	{
    		 Log.d("ru.devgame", ex.toString());
    		 billingInConnecionProcess = false;
    		 billingSetupComplited = false;
    		 if (mBillingHelper != null) {
    			 try {
    				 mBillingHelper.dispose();
    			 } catch(Exception ex2) {}
    			 mBillingHelper = null;
    		 }
    	}
	}
	
	public Handler ConnectToBillingMessage = new Handler()
    {
        public void handleMessage(Message msg)
        {
        	connectToBilling();
        }
    };
	
	IabHelper.OnIabSetupFinishedListener setupFinishedListener = new IabHelper.OnIabSetupFinishedListener() {
		   public void onIabSetupFinished(IabResult result) {
		      if (!result.isSuccess()) {
		         Log.d("ru.devgame", "Problem setting up In-app Billing: " + result);
		         billingInConnecionProcess = false;
		         billingSetupComplited = false;
		         if (mBillingHelper != null) {
		        	 try {
	    				 mBillingHelper.dispose();
	    			 } catch(Exception ex2) {}
	    			 mBillingHelper = null;
	    		 }
		      }
		      else
		      {
		    	  billingSetupComplited = true;
		    	  mBillingHelper.queryInventoryAsync(true, additionalSkuList, mGotInventoryListener);
		      }
		   }
		};
		
	IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
	   public void onQueryInventoryFinished(IabResult result, Inventory inventory)
	   {
	      if (result.isFailure())
	      {
	    	  Log.d("ru.devgame", result.toString());
	      }
	      else
	      {
	    	for(String item : additionalSkuList)
	    	{
	    		try
	    		{
	    			SkuDetails details = inventory.getSkuDetails(item);
	    			nativeLib.onItemPurchasableInfo(item, inventory.getPurchase(item) != null, details != null ? details.getPrice() : "");
	    		}
	    		catch(Exception ex)
	    		{
	    			Log.d("ru.devgame", ex.toString());
	    		}
	    	}
	    	
	        // does the user have the premium upgrade?
	        List<String> items = inventory.getAllOwnedSkus();        
	        for(String item : items)
	        {
	        	nativeLib.onItemPurchased(item);
	        }
	        
	        nativeLib.onConnectedToBillingServer();
	      }
	      
	      billingInConnecionProcess = false;
	   }
	};
	
	
	IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener 
	   = new IabHelper.OnIabPurchaseFinishedListener() {
	   public void onIabPurchaseFinished(IabResult result, Purchase purchase) 
	   {
	      if (result.isFailure()) {
	         Log.d("ru.devgame", "Error purchasing: " + result);
	         return;
	      }      
	      else if (purchase.getSku() != null && purchase.getSku() != "") {
	    	  nativeLib.onItemPurchased(purchase.getSku());
	      }
	   }
	};
	
	public void doPurchase(String item)
	{
		try
		{
			if(mBillingHelper != null)
				mBillingHelper.launchPurchaseFlow(this, item, 10001, mPurchaseFinishedListener, "");
		}
		catch(Exception ex)
		{
			 Log.d("ru.devgame", ex.toString());
		}
	}
	
	public Handler DoPurchaseMessage = new Handler()
    {
        public void handleMessage(Message msg)
        {
        	doPurchase((String)msg.obj);
        }
    };
    
    public void openRateUs()
	{
		if (storeKeys.containsKey(OpenIabHelper.NAME_YANDEX)) {
			Uri uri = Uri.parse("market://details?id=" + getPackageName());
	        Intent myAppLinkToMarket = new Intent(Intent.ACTION_VIEW, uri);
	        myAppLinkToMarket.setPackage("com.yandex.store");
	        try {
	        	startActivity(myAppLinkToMarket);
	        } catch (ActivityNotFoundException e) {
	            Toast.makeText(this, " unable to find Yandex.Strore app", Toast.LENGTH_LONG).show();
	        }
		} else {
			Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("market://details?id=" + getPackageName()));
			startActivity(browserIntent);
		}
	}
    
    public Handler OpenRateUsMessage = new Handler()
    {
        public void handleMessage(Message msg)
        {
        	openRateUs();
        }
    };
}