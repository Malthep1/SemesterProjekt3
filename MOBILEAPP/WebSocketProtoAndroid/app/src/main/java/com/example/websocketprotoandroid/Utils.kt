package com.example.websocketprotoandroid

import android.app.NotificationManager
import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import android.util.Log
import androidx.core.app.NotificationCompat
import java.io.BufferedReader
import java.io.FileNotFoundException
import java.io.InputStreamReader
import java.io.OutputStreamWriter

fun readFromFile(context : Context, filename : String) : String{
    var returnVal : String = ""
    try{
        val inputStreamReader : InputStreamReader = InputStreamReader(context.openFileInput(filename))
        val bufferedReader : BufferedReader = BufferedReader(inputStreamReader)
        var receiveString : String? = ""
        val stringBuilder : StringBuilder = StringBuilder()
        var done = false
        while(!done){
            receiveString = bufferedReader.readLine()
            if(receiveString == null){
                done = true
            }
            else{
                stringBuilder.append("").append(receiveString)
            }
        }
        inputStreamReader.close()
        returnVal = stringBuilder.toString().trim()
    }
    catch(ex : FileNotFoundException){
        Log.e("File read", "Cannot read file")
    }
    return returnVal
}

fun writeToFile(filename : String, data : String, context : Context){
    val outputStreamWriter : OutputStreamWriter = OutputStreamWriter(context.openFileOutput(filename, Context.MODE_PRIVATE))
    outputStreamWriter.write(data)
    outputStreamWriter.close()
}

private val NOTIFICATION_ID = 0

fun NotificationManager.sendNotification(messageBody: String, applicationContext : Context){
    val contentIntent = Intent(applicationContext, MainActivity::class.java)
    val contentPendingIntent = PendingIntent.getActivity(applicationContext, NOTIFICATION_ID, contentIntent, PendingIntent.FLAG_UPDATE_CURRENT)
    val builder = NotificationCompat.Builder(applicationContext, applicationContext.getString(R.string.not_channel_id))
        .setSmallIcon(R.drawable.paw_orange)
        .setContentTitle(applicationContext.getString(R.string.title))
        .setContentText(messageBody)
        .setContentIntent(contentPendingIntent)
        .setAutoCancel(true)

    notify(NOTIFICATION_ID, builder.build())
}