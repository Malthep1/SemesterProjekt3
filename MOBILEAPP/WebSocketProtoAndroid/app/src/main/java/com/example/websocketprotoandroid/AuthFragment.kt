package com.example.websocketprotoandroid

import android.app.NotificationManager
import android.content.Context
import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.navigation.fragment.findNavController
import com.example.websocketprotoandroid.databinding.FragmentAuthBinding

/**
 * A simple [Fragment] subclass.
 * Use the [AuthFragment.newInstance] factory method to
 * create an instance of this fragment.
 * NOT NEEDED: we don't dynamically create or destroy fragments
 */
class AuthFragment : Fragment() {
    // Binds the fragment to the correct .xml file containing GUI elements
    // These can later be accessed via binding.elementname
    private var _binding: FragmentAuthBinding? = null
    private val binding get() = _binding!!


    // Does something before the view is created
    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment:
        // Displays graphical representation of the bound .xml
        _binding = FragmentAuthBinding.inflate(inflater, container, false)

        return binding.root
    }

    // Does something after the view has been created
    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        Log.d("Auth", "VIEW CREATED")
        val editText = binding.editTextAuth
        var authstring : String? = null

        // If the button is pressed, do something
        binding.buttonAuth.setOnClickListener {
            Log.w("Auth", "Button pressed")
            // Convert the contents of editText (user input) to string
            authstring = editText?.text.toString()
            val act = activity
            if(authstring != null && authstring != ""){
                // Write the contents of user input to config.txt
                writeToFile("config.txt", authstring!!, act!!.applicationContext )
                // Write "true" to auth.txt
                writeToFile("auth.txt", "true", act!!.applicationContext)
                // Display a small message on the bottom of the screen: Device has been saved
                Toast.makeText( context,"Your device number has been saved!", Toast.LENGTH_SHORT).show()
                // Switch to connect fragment
                findNavController().navigate(R.id.action_to_connect_screen)
                Log.i("Auth", "Saved message: ")
                Log.i("Auth", authstring.toString().trim())
            }

            Log.w("Auth", authstring.toString())

        }
    }
}