package com.example.websocketprotoandroid

import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.navigation.fragment.findNavController
import com.example.websocketprotoandroid.databinding.FragmentTreatRequestBinding

// TODO: Rename parameter arguments, choose names that match
// the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
private const val ARG_PARAM1 = "param1"
private const val ARG_PARAM2 = "param2"

/**
 * A simple [Fragment] subclass.
 * Use the [ShowTreatRequest.newInstance] factory method to
 * create an instance of this fragment.
 */
class ShowTreatRequest : Fragment() {
    // Binds the fragment to the correct .xml file containing GUI elements
    // These can later be accessed via binding.elementname
    private var _binding: FragmentTreatRequestBinding? = null
    private val binding get() = _binding!!


    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        _binding = FragmentTreatRequestBinding.inflate(inflater, container, false)
        return binding.root

    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        Log.i("Treat fragment", "View has been created. Listening to U input")
        // WebSocketManager.setupWebSocketManager(Navigation.findNavController(view), this.activity)
        binding.buttonDeny.setOnClickListener {
            // Send message to the ws server that we deny
            Log.d("button deny", "button pressed")
            WebSocketManager.sendNack()
            WebSocketManager.treatRequestReceived = false
            this.findNavController().navigate(R.id.global_action_to_request_reply)
        }

        binding.buttonAllow.setOnClickListener {
            // Send message to the ws server that we allow the treat
            Log.d("button allow", "button pressed")
            WebSocketManager.treatRequestReceived = false
            WebSocketManager.sendAck()
            this.findNavController().navigate(R.id.global_action_to_request_reply)
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}