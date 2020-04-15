package com.nelson.player;

import android.annotation.SuppressLint;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import androidx.databinding.DataBindingUtil;
import com.nelson.player.databinding.ActivityMainBinding;
import com.nelson.player.player.PlayerCallBackAdapter;
import com.nelson.player.player.PlayerHelper;
import com.nelson.player.utils.CommonUtil;
import io.reactivex.Observable;
import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.functions.Consumer;
import java.io.File;
import java.util.concurrent.TimeUnit;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = MainActivity.class.getSimpleName();
    private PlayerHelper mPlayerHelper;
    private ActivityMainBinding mViewDataBinding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mViewDataBinding = DataBindingUtil.setContentView(this, R.layout.activity_main);
        mPlayerHelper = new PlayerHelper(mViewDataBinding.surfaceView);

        init();

        setDataSource();
    }

    private void setDataSource() {
        File root = Environment.getExternalStorageDirectory();
        File videoFile = new File(root, "xinsh.mp4");
        mPlayerHelper.setDataSource(videoFile.getAbsolutePath());
//        mPlayerHelper.setDataSource("http://ivi.bupt.edu.cn/hls/cctv1hd.m3u8");
    }

    private void init() {
        mPlayerHelper.init();
        mPlayerHelper.setCallBack(new PlayerCallBack());
    }

    private class PlayerCallBack extends PlayerCallBackAdapter {

        @Override
        public void onPrepare() {
            super.onPrepare();
            refreshProgressBar();
        }

        @Override
        public void onResume() {
            mViewDataBinding.setIsPlaying(true);
        }
    }

    @SuppressLint("CheckResult")
    private void refreshProgressBar() {
        Observable.interval(1, TimeUnit.SECONDS).observeOn(AndroidSchedulers.mainThread())
            .subscribe(new Consumer<Long>() {
                @Override
                public void accept(Long aLong) throws Exception {
                    int totalDuration = mPlayerHelper.getTotalDuration();
                    int currentDuration = mPlayerHelper.getCurrentDuration();
                    Log.i(TAG, "currentTime: " + currentDuration);
                    mViewDataBinding.seekBar.setMax(totalDuration);
                    mViewDataBinding.seekBar.setProgress(currentDuration);
                    mViewDataBinding.setCurrentTime(CommonUtil.getTime(currentDuration));
                    mViewDataBinding.setTotalTime(CommonUtil.getTime(totalDuration));
                }
            });
    }

    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.play_state_iv:
                if(mViewDataBinding.getIsPlaying()){
                    mPlayerHelper.pause();
                } else {
                    mPlayerHelper.resume();
                }
                break;
        }
    }
}
