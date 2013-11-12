package com.molei.ipclient4droid;

import java.io.File;
import java.net.DatagramSocket;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.molei.ipclient4droid.statipool.Global;
import com.molei.util.UdpClientSocket;

public class MainActivity extends Activity {
	private TextView tv_ip;
	private TextView tv_msg;
	private EditText et_user;
	private EditText et_password;
	private Button btn_open;
	private Button btn_close;
	private TextView tv_flow;
	private TextView tv_balance;
	private String userName;
	private String password;
	private byte[] key;
	private String msgs;// 信息
	private String balance;// 余额
	private String flow;// 流量
	private DatagramSocket socket;
	UdpClientSocket client_5201;
	UdpClientSocket client_5200;

	private Thread initThread;
	private Thread openThread;
	private Thread closeThread;
	private IPClient ipclient;
	private UpdateUIHandler handler;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		handler = new UpdateUIHandler();
		tv_ip = (TextView) findViewById(R.id.tv_ip);
		tv_msg = (TextView) findViewById(R.id.tv_msg);
		et_user = (EditText) findViewById(R.id.et_user);
		et_password = (EditText) findViewById(R.id.et_password);
		btn_open = (Button) findViewById(R.id.btn_open);
		btn_close = (Button) findViewById(R.id.btn_close);
		tv_flow = (TextView) findViewById(R.id.tv_flow);
		tv_balance = (TextView) findViewById(R.id.tv_balance);
		btn_open.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub

				userName = et_user.getText().toString().trim();
				password = et_password.getText().toString().trim();

				openThread = new Thread() {

					@Override
					public void run() {
						// TODO Auto-generated method stub
						super.run();

						connect();

					}

				};
				openThread.start();
			}
		});
		initThread = new Thread() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				super.run();

				ipclient = new IPClient();
				byte[] bytes = ipclient.Send05();
				System.out.println("length---> " + bytes.length);
				try {
					if (client_5201 == null) {
						client_5201 = new UdpClientSocket(5201);
					}
					client_5201.send(Global.SERVER_IP, 5301, bytes);
					byte[] info = client_5201.receive(Global.SERVER_IP, 5301);
					String[] result = ipclient.Recv06(info);
					tv_ip.setText(result[2]);
					tv_msg.setText(result[1]);
					System.out.println(result[0] + "\n" + result[1] + "\n"
							+ result[2] + "\n");

				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}

		};
		initThread.start();

	}

	void connect() {
		ipclient = new IPClient();
		System.out.println("userName--->" + userName);
		byte[] sendpack1F = ipclient.Send1F(userName);
		System.out.println("length---> " + sendpack1F.length);
		try {
			if (client_5200 == null) {
				client_5200 = new UdpClientSocket(5200);
				client_5200.send(Global.SERVER_IP, 5300, sendpack1F);

				byte[] recvpack20 = client_5200.receive(Global.SERVER_IP, 5300);
				key = ipclient.Recv20(recvpack20);
				byte[] sendpack21 = ipclient.Send21(userName, password, key);
				client_5200.send(Global.SERVER_IP, 5300, sendpack21);
				byte[] recvpack22 = client_5200.receive(Global.SERVER_IP, 5300);
				String tips[] = ipclient.Recv22(recvpack22);
				msgs  = tips[1];
				handler.sendEmptyMessage(0);
				new Thread(new HeartbeatSendThread(client_5201)).start();
				new Thread(new HeartbeatRecvThread(client_5201)).start();
			} else {
				client_5200.close();
				client_5200 = null;
				connect();
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			client_5200.close();

		}
	}

	public String getSDPath() {
		File sdDir = null;
		System.out.println(Environment.getExternalStorageState());
		boolean sdCardExist = Environment.getExternalStorageState().equals(
				"mounted"); // 判断sd卡是否存在
		if (sdCardExist) {
			sdDir = Environment.getExternalStorageDirectory();// 获取跟目录
		}
		return sdDir.toString();

	}

	private class HeartbeatSendThread implements Runnable {
		private UdpClientSocket client = null;

		public HeartbeatSendThread(UdpClientSocket client) {
			this.client = client;

		}

		@Override
		public void run() {
			// TODO Auto-generated method stub
			System.out.println("heartbeat send thread start");
			ipclient = new IPClient();
			byte[] sendpack1e = ipclient.Send_1e(userName, key);
			try {
				while (true) {
					client.send(Global.SERVER_IP, 5301, sendpack1e);
					System.out.println("heartbeat send package");
					Thread.sleep(5000);
				}
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	private class HeartbeatRecvThread implements Runnable {
		private UdpClientSocket client = null;

		public HeartbeatRecvThread(UdpClientSocket client) {
			this.client = client;
		}

		@Override
		public void run() {
			// TODO Auto-generated method stub
			ipclient = new IPClient();
			System.out.println("heartbeat recv thread start");
			try {
				while (true) {
					byte[] data = client.receive(Global.SERVER_IP, 5301);
					System.out.println("heartbeat recv package");
					String[] result = ipclient.Recv_0x1f(data);
					flow = result[0];
					balance = result[1];
					System.out.println("flow--->"+ flow+" balance--->"+balance);
					handler.sendEmptyMessage(1);
					Thread.sleep(1000);
				}
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

	}

	private class UpdateUIHandler extends Handler {

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			if(msg.what == 0){
				tv_msg.setText(msgs);
			}
			else if(msg.what ==1){
			tv_flow.setText(flow);
			tv_balance.setText(balance);
			}
		}

	}
}
