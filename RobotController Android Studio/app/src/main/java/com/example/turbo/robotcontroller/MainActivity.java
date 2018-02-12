package com.example.turbo.robotcontroller;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.Toast;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {
    private final String DEVICE_ADDRESS = "20:15:11:23:93:85"; //MAC Address of Bluetooth Module
    private final UUID PORT_UUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");

    private BluetoothDevice device;
    private BluetoothSocket socket;
    private OutputStream outputStream;

    Button forward, backward, left, right, stop, irSensors, irLogic, followLine, avoidObstacle, distance, bluetoothConnect;

    SeekBar servoSeekBar = (SeekBar)findViewById(R.id.servoSeekBar);

    static String command; //string variable that will store value to be transmitted to the bluetooth module


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //declaration of button variables

        forward = (Button) findViewById(R.id.forward);
        backward = (Button) findViewById(R.id.back);
        left = (Button) findViewById(R.id.left);
        right = (Button) findViewById(R.id.right);
        stop = (Button) findViewById(R.id.stop);
        irSensors = (Button) findViewById(R.id.irs);
        irLogic = (Button) findViewById(R.id.irl);
        followLine = (Button) findViewById(R.id.follow);
        avoidObstacle = (Button) findViewById(R.id.avoid);
        distance = (Button) findViewById(R.id.distance);
        bluetoothConnect = (Button) findViewById(R.id.bluetoothConnect);


        forward.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    socket.getOutputStream().write("tf /r/n".toString().getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        });

        backward.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    socket.getOutputStream().write("tb /r/n".toString().getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        });
        left.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    socket.getOutputStream().write("tl /r/n".toString().getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        });
        right.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    socket.getOutputStream().write("tr /r/n".toString().getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        });
        stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    socket.getOutputStream().write("ts /r/n".toString().getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        });
        irSensors.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    socket.getOutputStream().write("io /r/n".toString().getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        });
        irLogic.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    socket.getOutputStream().write("il /r/n".toString().getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        });
        followLine.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    socket.getOutputStream().write("ix /r/n".toString().getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        });
        avoidObstacle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    socket.getOutputStream().write("ib /r/n".toString().getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        });
        distance.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    socket.getOutputStream().write("is /r/n".toString().getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        });
        bluetoothConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    socket.getOutputStream().write("tf".toString().getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        });

    }






    //Initializes bluetooth module
    public boolean BTinit()
    {
        boolean found = false;

        BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        if(bluetoothAdapter == null) //Checks if the device supports bluetooth
        {
            Toast.makeText(getApplicationContext(), "Device doesn't support bluetooth", Toast.LENGTH_SHORT).show();
        }

        if(!bluetoothAdapter.isEnabled()) //Checks if bluetooth is enabled. If not, the program will ask permission from the user to enable it
        {
            Intent enableAdapter = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableAdapter,0);

            try
            {
                Thread.sleep(1000);
            }
            catch(InterruptedException e)
            {
                e.printStackTrace();
            }
        }

        Set<BluetoothDevice> bondedDevices = bluetoothAdapter.getBondedDevices();

        if(bondedDevices.isEmpty()) //Checks for paired bluetooth devices
        {
            Toast.makeText(getApplicationContext(), "Please pair the device first", Toast.LENGTH_SHORT).show();
        }
        else
        {
            for(BluetoothDevice iterator : bondedDevices)
            {
                if(iterator.getAddress().equals(DEVICE_ADDRESS))
                {
                    device = iterator;
                    found = true;
                    break;
                }
            }
        }

        return found;
    }

    public boolean BTconnect()
    {
        boolean connected = true;

        try
        {
            socket = device.createRfcommSocketToServiceRecord(PORT_UUID); //Creates a socket to handle the outgoing connection
            socket.connect();

            Toast.makeText(getApplicationContext(),
                    "Connection to bluetooth device successful", Toast.LENGTH_LONG).show();
        }
        catch(IOException e)
        {
            e.printStackTrace();
            connected = false;
        }

        if(connected)
        {
            try
            {
                outputStream = socket.getOutputStream(); //gets the output stream of the socket
            }
            catch(IOException e)
            {
                e.printStackTrace();
            }
        }

        return connected;
    }

    @Override
    protected void onStart()
    {
        super.onStart();
    }

}