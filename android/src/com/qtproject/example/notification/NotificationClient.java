package com.qtproject.example.notification;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.app.PendingIntent;
import android.graphics.Color;
import android.graphics.BitmapFactory;
import android.app.NotificationChannel;

import android.app.AlarmManager;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;

import java.util.Calendar;

public class NotificationClient
{
    public NotificationClient() {}

    public static void notify(Context context, String message, String dateTime, String time) {
        try {
            AlarmManager alarms = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
            Receiver receiver = new Receiver();
            IntentFilter filter = new IntentFilter("ALARM_ACTION");
            context.registerReceiver(receiver, filter);
            Intent intent = new Intent("ALARM_ACTION");
            intent.putExtra("param", message);
            PendingIntent operation = PendingIntent.getBroadcast(context,0 , intent, 0);
            long wakeupTime = calculateWakeupTime(dateTime,time);
            alarms.set(AlarmManager.RTC_WAKEUP, wakeupTime, operation);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static long calculateWakeupTime(String dateTime, String time) {
        String[] dateArray = dateTime.split("-");
        Log.d("TAG", "DateArray size: "+dateArray.length);
        String[] timeArray = time.split(":");
        Log.d("TAG", "TimeArray size: "+timeArray.length);
        Calendar calendar = Calendar.getInstance();
        calendar.clear();
        calendar.set(toInt(dateArray[2]),toInt(dateArray[1]) - 1,toInt(dateArray[0]),toInt(timeArray[0]), toInt(timeArray[1]));
        Log.d("lol", "WakeupTimeInMilliSeconds: " + calendar.getTimeInMillis());
        long thirtyMinutesInMs = 1000 * 60 * 30;
        //Notify will pop 30minutes before event
        return calendar.getTimeInMillis() - thirtyMinutesInMs;
    }

    private static int toInt(String str){
        try {
            return Integer.parseInt(str);
        }catch (Error e){
            Log.d("TAG", "toInt, Error e: ");
            return  0;
        }
    }

}
