package bahcesehir.edu.shapedetection;

/**
 * Created by ye on 10/04/16.
 */
public class ShapeDetection {

    public static native String nativeHello();

    public static native boolean blackAndWhite(long matAddress);

    public static native boolean detectRectangle(long matAddress);

    public static native boolean detectCircle(long matAddress);

    public static native boolean fillShape(long matAddress);


    public static native boolean getThreshold(long matAddress);
    public static native boolean getGaussian(long matAddress);
    public static native boolean getCanny(long matAddress);



}
