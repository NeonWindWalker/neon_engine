package ru.devgame;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Process;

public class AudioOutput extends Thread
{
	public static final int PACKET_FRAMES = 800;
	public static final int FREQUENCY = 48000;
	AudioTrack audioTrack;
	short[] buffer;
	int cppCalssPtr;
	volatile boolean playFlag;
	
	
	public AudioOutput(int cppCalssPtr)
	{
		this.cppCalssPtr = cppCalssPtr;
		playFlag = false;
		//setPriority(Process.THREAD_PRIORITY_AUDIO);
		setPriority(Thread.MAX_PRIORITY);
	}
	
	public volatile boolean stopFlag;
	
	@Override public void run()
	{
		int bufferSize = AudioTrack.getMinBufferSize(FREQUENCY, AudioFormat.CHANNEL_OUT_STEREO, AudioFormat.ENCODING_PCM_16BIT);
		this.setPriority(MAX_PRIORITY);
		
		audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, FREQUENCY, AudioFormat.CHANNEL_OUT_STEREO, AudioFormat.ENCODING_PCM_16BIT, bufferSize, AudioTrack.MODE_STREAM);
		buffer = new short[PACKET_FRAMES * 2];
		
		stopFlag = false;
		while(!stopFlag)
		{
			int ps = audioTrack.getPlayState();
			
			if(playFlag && ps != AudioTrack.PLAYSTATE_PLAYING)
			{
				audioTrack.play();
			}
			else if(!playFlag && ps == AudioTrack.PLAYSTATE_PLAYING)
			{
				audioTrack.pause();
			}
			
			if(ps == AudioTrack.PLAYSTATE_PLAYING)
			{
				callback(cppCalssPtr, buffer);
				audioTrack.write(buffer, 0, PACKET_FRAMES * 2);
			}
			else
			{
				try{ Thread.currentThread().sleep(100); } catch(Exception e){}
			}
		}
       
		audioTrack.stop();
		audioTrack.release();
		audioTrack = null;
	}
	
	public void play(boolean b)
	{
		playFlag = b;
	}
	
	public native void callback(int cppCalssPtr, short[] data);
	
	public static AudioOutput create(int cppCalssPtr)
	{
		AudioOutput p = new AudioOutput(cppCalssPtr);
		p.start();
		return p;
	}
	
	public void decreate()
	{
		stopFlag = true;
		try { join(); } catch(Exception e){}
	}
}
