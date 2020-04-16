package com.nelson.player;

import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
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
import io.reactivex.disposables.Disposable;
import io.reactivex.functions.Consumer;
import java.io.File;
import java.nio.ByteBuffer;
import java.util.concurrent.TimeUnit;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = MainActivity.class.getSimpleName();
    private PlayerHelper mPlayerHelper;
    private ActivityMainBinding mViewDataBinding;
    private boolean isRecording = false;
    private int mRecordTime = 0;
    private Disposable mRecordSubscribe;

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

        @Override
        public void captureImage(byte[] data, int width, int height) {
            final Bitmap bitmap = Bitmap.createBitmap(width, height, Config.ARGB_8888);
            bitmap.copyPixelsFromBuffer(ByteBuffer.wrap(data));
            mViewDataBinding.getRoot().post(new Runnable() {
                @Override
                public void run() {
                    if (bitmap != null) {
                        mViewDataBinding.displayIv.setImageBitmap(bitmap);
                    }
                }
            });
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
                if (mViewDataBinding.getIsPlaying()) {
                    mPlayerHelper.pause();
                    mViewDataBinding.setIsPlaying(false);
                } else {
                    mViewDataBinding.setIsPlaying(true);
                    mPlayerHelper.resume();
                }
                break;
            case R.id.capture_img_btn:
                mPlayerHelper.captureImage();
                break;
            case R.id.record_btn:
                if (isRecording) {
                    stopRecord();
                } else {
                    startRecord();
                }
                break;
        }
    }

    private void startRecord() {
        File rootFile = Environment.getExternalStorageDirectory();
        File videoFile = new File(rootFile, "record.mp4");
        if (videoFile.exists()) {
            videoFile.delete();
        }
        mRecordTime = 0;
        isRecording = true;
        mViewDataBinding.setIsRecord(isRecording);
        mPlayerHelper.startRecord(videoFile.getAbsolutePath());
        if (mRecordSubscribe != null && !mRecordSubscribe.isDisposed()) {
            mRecordSubscribe.dispose();
        }
        mRecordSubscribe = Observable.interval(1, TimeUnit.SECONDS).observeOn(AndroidSchedulers.mainThread())
            .subscribe(new Consumer<Long>() {
                @Override
                public void accept(Long aLong) throws Exception {
                    mRecordTime++;
                    mViewDataBinding.recordTimeTv.setText(CommonUtil.getTime(mRecordTime));
                    mViewDataBinding.recordIv.setBackgroundResource(
                        mRecordTime % 2 == 0 ? R.drawable.record_high : R.drawable.record_normal);
                }
            });
    }

    private void stopRecord() {
        isRecording = false;
        mPlayerHelper.stopRecord();
        mViewDataBinding.setIsRecord(isRecording);
    }
}
