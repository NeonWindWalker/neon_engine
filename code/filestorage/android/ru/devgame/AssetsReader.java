package ru.devgame;


import java.io.IOException;
import java.io.InputStream;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;

class AssetsReader
{
	AssetManager manager;
	String rootFolder;
	
	public AssetsReader(AssetManager manager, String rootFolder)
	{
		this.manager = manager;
		this.rootFolder = rootFolder;
	}
	
	public int size(String name)
	{
		InputStream stream = null;
		try
		{
			stream = manager.open(rootFolder + name, android.content.res.AssetManager.ACCESS_STREAMING);
			int s = stream.available();
			stream.close();
			return s;
		}
		catch(Exception ex)
		{
			try
			{
				if(stream != null)
					stream.close();
			}
			catch(Exception ex2)
			{}
			
			return -1;
		}
	}
	
	public boolean exists(String name)
	{
		InputStream stream = null;
		try
		{
			stream = manager.open(rootFolder + name, android.content.res.AssetManager.ACCESS_STREAMING);
			stream.close();
			return true;
		}
		catch(Exception ex)
		{
			try
			{
				if(stream != null)
					stream.close();
			}
			catch(Exception ex2)
			{}
			
			return false;
		}
	}
	
	public byte[] read(String name)
	{
		InputStream stream = null;
		try
		{
			stream = manager.open(rootFolder + name, android.content.res.AssetManager.ACCESS_STREAMING);
			byte[] data = new byte[stream.available()];
			stream.read(data);
			stream.close();
			return data;
		}
		catch(Exception ex)
		{
			try
			{
				if(stream != null)
					stream.close();
			}
			catch(Exception ex2)
			{}
			
			return null;
		}
	}
	
	
	public byte[] readChunk(String name, int offset, int size)
	{
		InputStream stream = null;
		try
		{
			stream = manager.open(rootFolder + name, android.content.res.AssetManager.ACCESS_STREAMING);
			if(offset + size > stream.available()){
				stream.close();
				return null;
			}
			byte[] data = new byte[size];
			stream.read(data, offset, size);
			stream.close();
			return data;
		}
		catch(Exception ex)
		{
			try
			{
				if(stream != null)
					stream.close();
			}
			catch(Exception ex2)
			{}
			
			return null;
		}
	}
	
	public boolean isFile(String name)
	{
		return name.contains(".");
	}
	
	public void listTraverse(java.util.ArrayList<String> arr, String path, String basepath)
	{
		try
		{
			String[] assets = manager.list(path);
			if(path.length() != 0)
				path += "/";
			if(basepath.length() != 0)
				basepath += "/";
			for(int i=0; i < assets.length; ++i)
			{
				if(isFile(path + assets[i]))
					arr.add(basepath + assets[i]);
				else
					listTraverse(arr, path + assets[i], basepath + assets[i]);
			}
		}
		catch(Exception ex)
		{
			return;
		}
	}
	
	public String[] list(String path, String basepath)
	{
		try
		{
			java.util.ArrayList<String> arr = new java.util.ArrayList<String>();
			listTraverse(arr, rootFolder + path, basepath);
			String[] ret = new String[arr.size()];
			for(int i=0; i < arr.size(); ++i)
				ret[i] = arr.get(i);
			return ret;
		}
		catch(Exception ex)
		{
			return null;
		}
	}
}