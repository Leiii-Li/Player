package com.nelson.player;

import android.os.Environment;
import android.view.SurfaceView;
import android.view.View;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import com.nelson.player.player.PlayerHelper;
import java.io.File;

public class MainActivity extends AppCompatActivity {

    private PlayerHelper mPlayerHelper;
    private SurfaceView mSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mSurfaceView = findViewById(R.id.surface_view);
        mPlayerHelper = new PlayerHelper(mSurfaceView);
        mPlayerHelper.init();
        File root = Environment.getExternalStorageDirectory();
        File videoFile  = new File(root,"cutout1.mp4");
        mPlayerHelper.setDataSource(videoFile.getAbsolutePath());

    }

}
