package com.example.eduskkuyoungminokswppa1;

import android.app.LauncherActivity;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.ColorSpace;
import android.net.Uri;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Adapter;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class ChatroomActivity extends AppCompatActivity {
    ArrayList<String>Items;
    ArrayAdapter<String>adapter;
    ListView listView;

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_chatroom);

        Items=new ArrayList<String>();
        adapter=new ArrayAdapter<String>(this,android.R.layout.simple_expandable_list_item_1,Items);
        ListView listView = (ListView) findViewById(R.id.chatBox);
        listView.setAdapter(adapter);

        Button add = (Button) findViewById(R.id.send);
        final EditText ed=(EditText)findViewById(R.id.type);

        add.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String str=ed.getText().toString();

                if (str.length() > 0) {
                    Items.add(str);
                    ed.setText("");
                    adapter.notifyDataSetChanged();
                }
            }
        });
    }
}

