package com.example.websocketprotoandroid

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.navigation.fragment.findNavController
import com.example.websocketprotoandroid.databinding.FragmentRequestReplyBinding
import com.example.websocketprotoandroid.databinding.FragmentSecondBinding

/**
 * A simple [Fragment] subclass as the second destination in the navigation.
 */
class ShowUserChoice : Fragment() {
    // Binds the fragment to the correct .xml file containing GUI elements
    // These can later be accessed via binding.elementname
    private var _binding: FragmentRequestReplyBinding? = null
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {

        _binding = FragmentRequestReplyBinding.inflate(inflater, container, false)

        return binding.root

    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        var text : TextView = view.findViewById(R.id.text_display_reply)

        if(WebSocketManager.reply){
            text.setText(R.string.display_treat_allowed)
        }
        else{
            text.setText(R.string.display_treat_denied)
        }

    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}