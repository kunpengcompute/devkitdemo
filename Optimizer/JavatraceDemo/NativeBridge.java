public class NativeBridge {
    static {
        try {
            System.loadLibrary("order_pipeline");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Failed to load order_pipeline: " + e.getMessage());
            throw e;
        }
    }

    /**
     * 计算订单价格（JNI）——calcPrice。
     * @param data 订单输入数据（1KB）
     * @param size 数据长度
     * @param orderId 订单号
     * @return 计算出的价格
     */
    public static native long calcPrice(byte[] data, int size, long orderId);

    /**
     * 扣减共享库存（JNI）——checkStock。
     * @param data 订单输入数据（1KB）
     * @param size 数据长度
     * @param orderId 订单号
     * @return 扣减后的剩余库存（负数表示不足）
     */
    public static native long checkStock(byte[] data, int size, long orderId);

    /**
     * 异步持久化订单（JNI）——asyncPersist。
     * @param data 订单输入数据（1KB）
     * @param size 数据长度
     * @param orderId 订单号
     * @param price 已计算的价格
     * @return 持久化写入的字节数
     */
    public static native long asyncPersist(byte[] data, int size, long orderId, long price);
}
