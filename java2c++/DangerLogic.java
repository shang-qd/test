class DangerLogic
{
    	static
    	{
        	System.loadLibrary("DangerLogic");
    	}

	public native float anger_rate( long id, 		// 一次导航ID
					float engine_speed,	// 发动机转速
					float throttle_opening,	// 节气阀开度
					int gps_angle, 		// GPS方向角
					boolean speed_warning,	// 超速报警
					boolean fatigue,	// 疲劳驾驶
					boolean swerve,		// 急转弯
					boolean add_speed,	// 急加速
					boolean sub_speed,	// 急减速
					boolean impact);	// 碰撞

	// 关闭评估 id：一次导航ID
	public native void close_evaluate(long id);

    	public static void main(String[] args)   
    	{
		DangerLogic obj = new DangerLogic();
		// 导航ID
		int id = 100;
		// 调用危险率函数
		float f = obj.anger_rate(id,100.0f,1.0f,12,false,false,true,false,false,false);
		// 关闭评估		
		obj.close_evaluate(id);
		System.out.println(f);
		System.out.println("java call c++ ok.");
    	}
}
