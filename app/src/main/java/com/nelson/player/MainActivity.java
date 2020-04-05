package com.nelson.player;

import android.view.SurfaceView;
import android.view.View;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import com.nelson.player.player.PlayerHelper;

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
        mPlayerHelper.setDataSource("http://ivi.bupt.edu.cn/hls/cctv1hd.m3u8");
    }

}
