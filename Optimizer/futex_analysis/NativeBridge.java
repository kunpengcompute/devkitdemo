public class NativeBridge {

    static {
        System.loadLibrary("trace_demo");
    }

    public static native long process(byte[] data, int nativeComputeRounds, int writeCount);
}