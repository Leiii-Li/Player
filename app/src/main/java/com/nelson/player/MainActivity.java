package com.nelson.player;

import android.annotation.SuppressLint;
import android.os.Environment;
import android.util.Log;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import androidx.databinding.DataBindingUtil;
import com.nelson.player.databinding.ActivityMainBinding;
import com.nelson.player.player.PlayerCallBackAdapter;
import com.nelson.player.player.PlayerHelper;
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
                    mViewDataBinding.setCurrentTime(getTime(currentDuration));
                    mViewDataBinding.setTotalTime(getTime(totalDuration));
                }
            });
    }

    public static String getTime(long second) {
        long hours = second / 3600;//转换小时数
        second = second % 3600;//剩余秒数
        long minutes = second / 60;//转换分钟
        second = second % 60;//剩余秒数
        if (hours > 0) {
            return unitFormat(hours) + ":" + unitFormat(minutes) + ":" + unitFormat(second);
        } else {
            return unitFormat(minutes) + ":" + unitFormat(second);
        }
    }

    private static String unitFormat(long i) {
        String retStr;
        if (i >= 0 && i < 10) {
            retStr = "0" + i;
        } else {
            retStr = "" + i;
        }
        return retStr;
    }
}
