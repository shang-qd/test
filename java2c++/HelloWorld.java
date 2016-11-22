class HelloWorld   
{
    	static
    	{
        	System.loadLibrary("HelloWorld");
    	}  
      
    	public native void myprint(int c);  
  
	public native int add(int a,int b);

    	public static void main(String[] args)   
    	{
		HelloWorld obj = new HelloWorld();
        	//obj.myprint(obj.add(1,2));
		int res = obj.add(1,2);
		obj.myprint(res);
		System.out.println(res);
    	}  
}
