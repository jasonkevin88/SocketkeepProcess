package com.jason.socket.process;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.Process;
import android.util.Log;

import java.util.Timer;
import java.util.TimerTask;

import androidx.annotation.Nullable;

/**
 * Description:KeepProcessService
 *
 * @author 陈宝阳
 * @create 2020/6/23 14:03
 */
public class KeepProcessService extends Service {

  private static final String TAG = "BAO";
  int i = 0;
  @Nullable
  @Override
  public IBinder onBind(Intent intent) {
    return null;
  }

  @Override
  public void onCreate() {
    super.onCreate();

    Watcher watcher = new Watcher();
    watcher.watch(String.valueOf(Process.myUid()));
    watcher.connect();
    Timer timer = new Timer();
    //定时器
    timer.scheduleAtFixedRate(
        new TimerTask() {
          public void run() {
            Log.i(TAG, "服务  "+i);
            i++;
          }
        }, 0, 1000 * 3);
  }

}
