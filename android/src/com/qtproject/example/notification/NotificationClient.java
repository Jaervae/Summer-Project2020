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
import android.os.SystemClock;
import java.util.Calendar;

public class NotificationClient
{
    public static final String NOTIFICATION_CHANNEL_ID = "10001" ;
    private final static String default_notification_channel_id = "default" ;


    public NotificationClient() {}

    public static void notify(Context context, String message, String date, String time) {
        try{
            long delay = calculateDelay(date, time);
            if (delay == 0){
                Log.d("TAG", "Even is already in the past");
                return;
            }
            //long delay= calculateWakeupTime(dateTime,time);
            scheduleNotification(getNotification(message, context), delay, context);
        }catch(Exception e) {
            e.printStackTrace();
        }
    }


    private static void scheduleNotification (Notification notification , long delay, Context context) {
        Intent notificationIntent = new Intent( context, MyNotificationPublisher.class ) ;
        notificationIntent.putExtra(MyNotificationPublisher.NOTIFICATION_ID , 1 ) ;
        notificationIntent.putExtra(MyNotificationPublisher.NOTIFICATION , notification) ;
        PendingIntent pendingIntent = PendingIntent.getBroadcast ( context , 0 , notificationIntent , PendingIntent. FLAG_UPDATE_CURRENT ) ;
        long futureInMillis = SystemClock.elapsedRealtime () + delay ;
        AlarmManager alarmManager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE ) ;
        assert alarmManager != null;
        alarmManager.set(AlarmManager. ELAPSED_REALTIME_WAKEUP , futureInMillis , pendingIntent) ;
    }

    private static Notification getNotification (String content, Context context) {
        Notification.Builder builder = new Notification.Builder( context, default_notification_channel_id ) ;
        builder.setContentTitle( "Incoming event" ) ;
        builder.setContentText(content) ;
        builder.setSmallIcon(R.drawable.icon);
        builder.setLargeIcon(BitmapFactory.decodeResource(context.getResources(), R.drawable.icon));
        builder.setAutoCancel( true ) ;
        builder.setChannelId( NOTIFICATION_CHANNEL_ID ) ;
        return builder.build() ;
    }

    private static long calculateDelay(String mDate, String mTime){
        String[] dateArray = mDate.split("-");
        Log.d("TAG", "DateArray size: "+dateArray.length);
        String[] timeArray = mTime.split(":");
        Log.d("TAG", "TimeArray size: "+timeArray.length);
        Calendar currentDate = Calendar.getInstance();
        Calendar eventDate = Calendar.getInstance();
        eventDate.clear();
        eventDate.set(toInt(dateArray[2]),toInt(dateArray[1]) - 1,toInt(dateArray[0]),toInt(timeArray[0]), toInt(timeArray[1]));
        long thirtyMinutesInMs = 1000 * 60 * 30;
        if (eventDate.getTimeInMillis() < currentDate.getTimeInMillis()){
            return 0;
        }else
           return eventDate.getTimeInMillis() - currentDate.getTimeInMillis() - thirtyMinutesInMs;

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
