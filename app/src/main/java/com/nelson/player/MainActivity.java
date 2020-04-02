package com.nelson.player;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import com.nelson.player.player.PlayerHelper;

public class MainActivity extends AppCompatActivity {

    private PlayerHelper mPlayerHelper;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mPlayerHelper = new PlayerHelper();
        mPlayerHelper.init();
        mPlayerHelper.setDataSource("http://ivi.bupt.edu.cn/hls/cctv1hd.m3u8");

    }

}
