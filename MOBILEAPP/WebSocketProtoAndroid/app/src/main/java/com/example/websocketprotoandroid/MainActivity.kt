package com.example.websocketprotoandroid

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.content.Context
import android.content.Intent
import android.graphics.Color
import android.os.Build
import android.os.Bundle
import android.util.Log
import com.google.android.material.snackbar.Snackbar
import androidx.appcompat.app.AppCompatActivity
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.navigateUp
import androidx.navigation.ui.setupActionBarWithNavController
import android.view.Menu
import android.view.MenuItem
import androidx.core.app.NotificationCompat
import androidx.core.view.WindowCompat
import com.example.websocketprotoandroid.databinding.ActivityMainBinding
import org.java_websocket.client.WebSocketClient
import java.net.URI
import java.net.URISyntaxException
import java.nio.ByteBuffer
import org.java_websocket.handshake.ServerHandshake
import java.io.*
import java.lang.Exception



class MainActivity : AppCompatActivity() {
    private lateinit var appBarConfiguration: AppBarConfiguration
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        var authenticated = false
        val dev_id : String? = null
        super.onCreate(savedInstanceState)
        // DEBUG
        val onFile = readFromFile(this, "auth.txt")
        Log.w("File contents", onFile)

        val file = File("config.txt")

        createNotificationChannel()

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        setSupportActionBar(binding.toolbar)

        val navController = findNavController(R.id.nav_host_fragment_content_main)

        appBarConfiguration = AppBarConfiguration(navController.graph)
        setupActionBarWithNavController(navController, appBarConfiguration)

        // Check if authenticated. If not authenticated, navigate to AuthFragment
        authenticated = checkAuthentication()
        if(!authenticated){
            Log.d("Main", "NOT AUTHENTICATED!!!")
            findNavController(R.id.nav_host_fragment_content_main).navigate(R.id.global_action_to_authenticate)
        }

        Log.d("Main", "End onCreate")
    }

    override fun onResume(){
        super.onResume()
        // When the app resumes, check if a treat request has been received,
        // If so, navigate to ShowTreatRequest fragment
        if(WebSocketManager.treatRequestReceived){
            findNavController(R.id.nav_host_fragment_content_main).navigate(R.id.global_action_to_treat_request)
            Log.d("Main", "treat received after waking main")
        }
        Log.w("Main", "Resumed main")
    }

    override fun onRestart(){
        super.onRestart()
        // Like onResume
        if(WebSocketManager.treatRequestReceived){
            findNavController(R.id.nav_host_fragment_content_main).navigate(R.id.global_action_to_treat_request)
            Log.d("Main", "treat received after waking main")
        }
        Log.w("Main", "Restarted main")
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Hide action bar, it's pointless for now
        supportActionBar?.hide()
        // Inflate the menu; this adds items to the action bar if it is present.
        //menuInflater.inflate(R.menu.menu_main, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        return when (item.itemId) {
            R.id.action_settings -> true
            else -> super.onOptionsItemSelected(item)
        }
    }

    // Necessary to display notifications. Can be used to set some settings for
    // Notifications, such as vibration, light, probably sounds etc.
    private fun createNotificationChannel(){
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O){
            val name = getString(R.string.not_channel_name)
            val descriptionText = getString(R.string.not_channel_desc)
            val importance = NotificationManager.IMPORTANCE_DEFAULT
            val channel = NotificationChannel(getString(R.string.not_channel_id), name, importance).apply{
                description = descriptionText
            }
            // Register the channel with the system
            val notificationManager : NotificationManager =
                getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
            channel.enableLights(true)
            channel.lightColor = Color.RED
            channel.enableVibration(true)
            channel.description = "Feed your pet, dummy!"
            notificationManager.createNotificationChannel(channel)
        }
    }

    private fun checkAuthentication() : Boolean{
        var authenticated = false
        val fileAuth = "auth.txt"
        val authResult = readFromFile(this, fileAuth)
        Log.w("Auth", "auth.txt file contents:")
        Log.i("Auth", authResult)
        if(authResult.trim() == "true"){
            authenticated = true
        }

        return authenticated
    }

    /*override fun onSupportNavigateUp(): Boolean {
        return false
        //val navController = findNavController(R.id.nav_host_fragment_content_main)

                //return navController.navigateUp(appBarConfiguration)
                //super.onSupportNavigateUp()
    }*/


}

