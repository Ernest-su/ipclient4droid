package com.molei.util;
/**
 * java Dword
 * 
 *
 */
public class Dword implements Cloneable{
	/**
	 * ���ڴ��Dwordֵ,��c++�ж����Dword��unsigned long����Ϊ0-4294967295;
	 */
	private long dword;
	/**
	 * ���ֽ�
	 * @param dword
	 */
	public Dword(byte[] dword){
		long d0 =  (long)dword[3] & 0xff;   
		long d1 = ((long)(dword[2] & 0xff))<< 8;   
		long d2 = ((long)(dword[1] & 0xff))<< 16;   
		long d3 = ((long)(dword[0] & 0xff))<< 24;   
		this.dword=d0|d1|d2|d3;
	}
	public Dword(){
		
	}
	public Dword(long dword){
		this.dword=dword;
	}
	public long getValue(){
		return dword;
	}
	
	public void setValue(long dword){
		this.dword=dword;
	}
	/**
	 * ת��Ϊ4�ֽ����
	 * @return
	 */
	public byte[] toByte(){
		byte[] d=new byte[4]; 
		d[0]=(byte) (dword>>24&0xFF);
		d[1]=(byte) ((dword>>16)&0xFF);
		d[2]=(byte) ((dword>>8)&0xFF);
		d[3]=(byte) (dword&0xFF);
		return d;
	}
	/**
	 * ����lenλ
	 * @param d
	 * @param len
	 */
	public static void shr(Dword d,long len){
		d.setValue(d.getValue()>>len);
	}
	/**
	 * ���,����ֵ����d1��
	 * @param d1
	 * @param d2
	 */
	public static void xor(Dword d1,Dword d2){
		d1.setValue(d1.getValue()^d2.getValue());
	}
	/**
	 * ������
	 * @param d
	 * @param y
	 */
	public static void and(Dword d,int y){
		d.setValue(d.getValue()&y);
	}
	/**
	 * ��
	 * @param d1
	 * @param d2
	 */
	public static void add(Dword d1,Dword d2){
		long r=d1.getValue()+d2.getValue();
		//����Խ��
		while(r>4294967295L){
			r-=4294967295L+1;
		}
		d1.setValue(r);
	}
	
	/**
	 * ��һ��byte�����л��4�ֽڷ�ת�󷵻�Dword
	 * @param data
	 * @param offset1
	 * @return
	 */
	public static Dword getDword(byte[] data,long offset1){
		int offset=(int)offset1;
		
		if(data.length<offset||data.length%4!=0||offset%4!=0){return null;}
		byte[] a=new byte[4];
		for(int i=0;i<4;i++){
			a[i]=data[offset+3-i];
		}
		return new Dword(a);
	}
	
	/**
	 * ��getDword�෴
	 * @param data
	 * @param offset
	 * @param d
	 */
	public static void setDword(byte[] data,int offset,Dword d){
		byte[] a=d.toByte();
		for(int i=0;i<4;i++){
			data[offset+3-i]=a[i];
		}
	}
	
	/**
	 * ����һ��int���飬����Ϊ�ģ��ֱ��ӦDword��ÿ���ֽ�
	 * @return
	 */
	public int[] toInts(){
		byte[] a=toByte();
		int[] b=new int[4];
		for(int i=0;i<4;i++){
			b[i]=a[i]&0xff;
		}
		return b;
	}
	@Override
	public String toString() {
		byte[] d=this.toByte();
		return String.format("%02x%02x%02x%02x", d[0],d[1],d[2],d[3]);
	}

	@Override
	public Dword clone(){
		try {
			return (Dword)super.clone();
		} catch (CloneNotSupportedException e) {
			e.printStackTrace();
			return null;
		}
	}

	
}
