package com.qtproject.example.notification;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.widget.Toast;

import java.util.Random;

public class Receiver extends BroadcastReceiver {

    private static NotificationManager m_notificationManager;
    private static Notification.Builder m_builder;

    @Override
    public void onReceive(Context context, Intent intent) {
        notify(context, intent.getStringExtra("param"));
    }

    public static void notify(Context context, String message) {
        try {
            m_notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);

            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                int importance = NotificationManager.IMPORTANCE_DEFAULT;
                NotificationChannel notificationChannel = new NotificationChannel("Qt", "Event", importance);
                m_notificationManager.createNotificationChannel(notificationChannel);
                m_builder = new Notification.Builder(context, notificationChannel.getId());
            } else {
                m_builder = new Notification.Builder(context);
            }

            m_builder.setSmallIcon(R.drawable.icon)
                    .setLargeIcon(BitmapFactory.decodeResource(context.getResources(), R.drawable.icon))
                    .setContentTitle("Event is aproaching!")
                    .setContentText(message)
                    .setDefaults(Notification.DEFAULT_SOUND)
                    .setColor(Color.GREEN)
                    .setAutoCancel(true);
            //Assign random id(0-50) to not stack notifications
            final int randomId = new Random().nextInt(51);
            m_notificationManager.notify(randomId, m_builder.build());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
