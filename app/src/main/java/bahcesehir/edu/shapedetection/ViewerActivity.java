package bahcesehir.edu.shapedetection;

import android.content.Intent;
import android.graphics.Bitmap;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import org.opencv.android.Utils;
import org.opencv.core.Mat;

public class ViewerActivity extends AppCompatActivity {

    Mat inputMat;
    ImageView imageView;

    Mat mShape;
    Mat mThreshold;
    Mat mGaussian;
    Mat mCanny;

    Bitmap bOriginal;
    Bitmap bShape;
    Bitmap bThreshold;
    Bitmap bGaussian;
    Bitmap bCanny;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_viewer);


        Intent i = getIntent();
        long frameAddress = i.getLongExtra("CapturedFrame", 0);

        inputMat = new Mat(frameAddress);
        Mat m = inputMat.clone();
        mShape = inputMat.clone();
        mThreshold = inputMat.clone();
        mGaussian = inputMat.clone();
        mCanny = inputMat.clone();


        ShapeDetection.detectCircle(m.getNativeObjAddr());
        ShapeDetection.detectRectangle(m.getNativeObjAddr());


        Bitmap bm = Bitmap.createBitmap(m.cols(), m.rows(), Bitmap.Config.ARGB_8888);
        Utils.matToBitmap(m, bm);

        bOriginal = bm;

        imageView = (ImageView) findViewById(R.id.frameViewer);
        imageView.setImageBitmap(bm);


        Button btnOriginal = (Button) findViewById(R.id.btnOriginal);
        Button btnThreshold = (Button) findViewById(R.id.btnThreshold);
        Button btnGaussian = (Button) findViewById(R.id.btnGaussian);
        Button btnCanny = (Button) findViewById(R.id.btnCanny);
        Button btnShape = (Button) findViewById(R.id.btnShape);

        btnOriginal.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                imageView.setImageBitmap(bOriginal);

            }
        });

        btnShape.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                if (bShape == null) {
                    ShapeDetection.fillShape(mShape.getNativeObjAddr());
                    bShape = Bitmap.createBitmap(mShape.cols(), mShape.rows(), Bitmap.Config.ARGB_8888);
                    Utils.matToBitmap(mShape, bShape);
                }
                imageView.setImageBitmap(bShape);

            }
        });


        btnThreshold.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                if (bThreshold == null) {
                    ShapeDetection.getThreshold(mThreshold.getNativeObjAddr());
                    bThreshold = Bitmap.createBitmap(mThreshold.cols(), mThreshold.rows(), Bitmap.Config.ARGB_8888);
                    Utils.matToBitmap(mThreshold, bThreshold);
                }
                imageView.setImageBitmap(bThreshold);

            }
        });

        btnGaussian.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                if (bGaussian == null) {
                    ShapeDetection.getGaussian(mGaussian.getNativeObjAddr());
                    imageView.setImageBitmap(bGaussian);
                    bGaussian = Bitmap.createBitmap(mGaussian.cols(), mGaussian.rows(), Bitmap.Config.ARGB_8888);
                    Utils.matToBitmap(mGaussian, bGaussian);
                }
                imageView.setImageBitmap(bGaussian);
            }
        });

        btnCanny.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                if (bCanny == null) {
                    ShapeDetection.getCanny(mCanny.getNativeObjAddr());
                    bCanny = Bitmap.createBitmap(mCanny.cols(), mCanny.rows(), Bitmap.Config.ARGB_8888);
                    Utils.matToBitmap(mCanny, bCanny);
                }
                imageView.setImageBitmap(bCanny);

            }
        });

    }
}
