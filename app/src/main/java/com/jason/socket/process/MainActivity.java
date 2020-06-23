package com.jason.socket.process;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

  // Used to load the 'native-lib' library on application startup.
  static {
    System.loadLibrary("native-lib");
  }

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);

    // Example of a call to a native method
    TextView tv = findViewById(R.id.sample_text);
    tv.setText(stringFromJNI());

    Intent intent = new Intent(MainActivity.this, KeepProcessService.class);
    startService(intent);
  }

  /**
   * A native method that is implemented by the 'native-lib' native library, which is packaged with
   * this application.
   */
  public native String stringFromJNI();
}
