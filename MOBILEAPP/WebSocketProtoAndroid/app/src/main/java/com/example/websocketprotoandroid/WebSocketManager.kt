package com.example.websocketprotoandroid

import android.app.Activity
import android.app.Application
import android.app.NotificationManager
import android.content.Context
import android.content.Intent
import android.util.Log
import androidx.core.content.ContextCompat
import androidx.core.content.ContextCompat.startActivity
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import androidx.navigation.NavController
import androidx.navigation.Navigation
import androidx.navigation.findNavController
import org.java_websocket.client.WebSocketClient
import org.java_websocket.handshake.ServerHandshake
import java.lang.Exception
import java.net.URI
import java.util.concurrent.TimeUnit

class WebSocketManager () {

    companion object{
        const val WEB_SOCKET_URL = "ws://foodgiver.hopto.org:8080/chat"
        const val TAG = "Foodgiver"
        var connected : Boolean = false
        const val timeout : Long = 3000
        var mClient : WsClient? = null
        var treatRequestReceived = false
        var navController : NavController? = null
        var notificationManager : NotificationManager? = null
        var mActivity : Activity? = null
        var fg_id : String? = null
        var reply = false


        fun setupWebSocketManager(pNav : NavController, act : Activity, id : String){
            navController = pNav
            mActivity = act
            fg_id = id
        }

        class WsClient : WebSocketClient(URI(WEB_SOCKET_URL)){
            override fun onOpen(handshakeData: ServerHandshake?) {
                Log.d(TAG, "onOpen was called")
                connected = true
                subscribe()
            }

            fun sendAck(){
                val ack = "treat ack_"
                val message = "$ack$fg_id"
                send(message)
            }

            fun sendNack(){
                val nack = "treat nack_"
                val message = "$nack$fg_id"
                send(message)
            }

            override fun onMessage(message: String){
                Log.d(TAG, "Message from server: " + message)
                handleMessage(message)
            }

            override fun onClose(code: Int, reason: String?, remote: Boolean) {
                Log.d(TAG,"onClose called, code: " + code + " reason: " + reason)
                connected = false
            }

            override fun onError(ex: Exception?) {
                Log.d(TAG, "Error, throw exception here")
                Log.d(TAG, ex.toString())
            }

            private fun subscribe(){
                // Subscribe to foodgiver service to receive notifications
                // Append extra characters to not crash the server...
                //val str = "000"
                // nvm...
                send("auth_$fg_id\n")
            }

            private fun handleMessage(message: String){
                Log.d(TAG, "handling ws message")
                if(message == "treat request"){
                    // Set the variable to true: Used for determining whether the user has answered yet,
                        // and which Fragment should be navigated to on notification press/wake up
                    treatRequestReceived = true
                    val mAct = mActivity
                    if(mAct != null){
                        notificationManager = ContextCompat.getSystemService(mAct, NotificationManager::class.java) as NotificationManager
                        notificationManager?.sendNotification(mAct.getString(R.string.notification_treat), mAct.applicationContext)
                    }
                    navController?.navigate(R.id.action_to_fragment_treat_request)
                }
            }
        }
        fun connectClient(){
            if(!connected){
                mClient = WsClient()
                mClient?.connectBlocking(timeout, TimeUnit.MILLISECONDS)
            }
        }

        fun sendAck(){
            mClient?.sendAck()
            reply = true
        }

        fun sendNack(){
            mClient?.sendNack()
            reply = false
        }
    }
}