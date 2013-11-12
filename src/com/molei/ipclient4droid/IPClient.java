package com.molei.ipclient4droid;

import java.io.UnsupportedEncodingException;

public class IPClient {
	static {
		try {
			System.loadLibrary("ipclient");
		} catch (UnsatisfiedLinkError ule) {
			System.err.println("WARNING: Could not load library!");
		}
	}

	/******************** 连接服务器的函数 **********************************/
	public native byte[] Send05();
	public String[] Recv06(byte[] data){
		 String[] result = new String[3];
		if(data[2]!=0x06){
			result[0]="连接服务器失败!";
		}else{
			result[0]="连接服务器成功!";
			}
		try {
			result[1] = new String(data, 0x29, 69,"GBK");
			result[2] = new String(data,0x72,14,"UTF-8");
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return result;
	}
	/********************开放IP的函数们**********************************/
	public native byte[] Send1F(String userName);
	
	
	public byte[] Recv20(byte[] data){
		byte[] key = new byte[2];
		key[0] = data[0x35];
		key[1] = data[0x36];
		return key;
	}
	public native byte[] Send21(String userName,String password,byte[] key);
	
	public String[] Recv22(byte[] data){
		String[] tips = new String[2];
		if(data[0x02]!=0x22){
			tips[0] = "-1";
			tips[1] = "netword error";
			return tips;
		}
		if(data[0x03]!=0x00){
			tips[0] = "0"; 
		}else tips[0] ="-2";
		byte[] len = new byte[4];
		for(int i = 0; i < 4;i++){
			len[i] = data[0x64+i];
		}
		int lens = toInt(len);
		byte[] tip = new byte[lens];
		for(int i = 0 ; i < lens;i++){
			tip[i] = data[0x68+i];
		}
		
		try {
		 tips[1] =  new String(tip,0,tip.length,"GBK");
	
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return tips;
	};
	
	
	/********************心跳包函数 ***********************************/
	
	public native byte[] Send_1e(String userName,byte[] key);
	
	public String[] Recv_0x1f(byte[] data){
		if(data[0x02]!=0x1f){
			return new String[]{""};
		}
		byte[] flow = new byte[8];
		for(int i = 0; i < 8;i++){
			flow[i] = data[0x0b+i];
		}
		
		byte[] balance = new byte[8];
		for(int i = 0; i < 8;i++){
			balance[i] = data[0x13+i];
		}
		
		return new String[]{byteToDouble(flow) + "kb",byteToDouble(balance) + "元"};
	};
	
	/**
	 * byte[] to int
	 * @param bRefArr
	 * @return
	 */
	 public static int toInt(byte[] bRefArr) { 
	        int iOutcome = 0; 
	        byte bLoop; 
	         
	        for ( int i =0; i<4 ; i++) { 
	            bLoop = bRefArr[i]; 
	            iOutcome+= (bLoop & 0xFF) << (8 * i); 
	           
	        }   
	         
	        return iOutcome; 
	    }    
	 //  字节到浮点转换
	 public static double byteToDouble(byte[] b){
	  long l;
	  l=b[0];
	  l&=0xff;
	  l|=((long)b[1]<<8);
	  l&=0xffff;
	  l|=((long)b[2]<<16);
	  l&=0xffffff;
	  l|=((long)b[3]<<24);
	  l&=0xffffffffl;
	  l|=((long)b[4]<<32);
	  l&=0xffffffffffl;
	  l|=((long)b[5]<<40);
	  l&=0xffffffffffffl;
	  l|=((long)b[6]<<48);
	  l|=((long)b[7]<<56);
	  return Double.longBitsToDouble(l);
	 }
}