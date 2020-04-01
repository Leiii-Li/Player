package com.nelson.player;

import android.view.View;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import com.nelson.player.player.PlayerNative;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView textView = findViewById(R.id.sample_text);
        textView.setText(PlayerNative.getVersion());
    }

}
