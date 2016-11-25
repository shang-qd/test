/**
 * 本类说明了java调用C++的接口和方法
 * @version 1.0
 * @author 尚庆东
 */
class DangerLogic
{
	static {
		// unix下 动态库的名称为libDangerLogic.so
		// win 下动态库的名称为DangerLogic.dll
		System.loadLibrary("DangerLogic");
    }

    /**
     * 获得危险率函数
     * @param id: 一次导航的ID
     * @param engine_speed: 发动机转速
     * @param throttle_opening:节气阀开度
     * @param gps_angle:GPS方向角
     * @param speed_warning:超速报警
     * @param fatigue:疲劳驾驶
     * @param swerve:急转弯
     * @param add_speed:急加速
     * @param sub_speed:急减速
     * @param impact:碰撞
     */
	public native float anger_rate( long id,
					float engine_speed,
					float throttle_opening,
					int gps_angle,
					boolean speed_warning,
					boolean fatigue,
					boolean swerve,	
					boolean add_speed,
					boolean sub_speed,
					boolean impact);

	/**
	 * 关闭危险评估，释放资源
	 * @param id : 一次导航的ID
	 */
	public native void close_evaluate(long id);

    public static void main(String[] args) {
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