package TME2;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class Serveur {

	private static ServerSocket socket;
	private static int port;
		
	public static void main(String[] args) {
		try{
			port = 1024;
			socket = new ServerSocket(port);
			System.out.println("TCP server is running on "+port);
			while(true){
				//accept new TCP client
				Socket client = socket.accept();
				//open output stream
				System.out.println("New client, address " + client.getInetAddress() + " on " + client.getPort() + ".");
				//thread hr et passage socket en argument
				HttpRequest hr = new HttpRequest(client);
                hr.start();
			}	
		}
	    catch(IOException e){
	    	System.out.println("main IOException \n ");
	    	e.printStackTrace();
	    }
		
	}

}
