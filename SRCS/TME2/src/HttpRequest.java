package TME2;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

public class HttpRequest extends Thread {

	final private Socket socket;

	public HttpRequest(Socket so) {
		socket = so;
	}

	// GET/PUT nomfichier httpversion ...
	public void processRequest(InputStream is) {		
		System.out.println("avant le try\n");
		try (InputStreamLiner isl = new InputStreamLiner(is)){
			String s = null;
			if ( (s = isl.readLine(InputStreamLiner.UNIX)) != null) {
				System.out.println("httpRequest " + s);
				String tmp[] = s.split(" ");
				if (tmp[0].compareTo("GET") == 0) {
					processGet(tmp[1]);
				}
				if (tmp[0].compareTo("PUT") == 0) {
					processPut(tmp[1]);
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		} 
	}

	public void processPut(String s) {
		String ligne = "";
		File f = new File(s);
		FileOutputStream fos;
		try (InputStream is = socket.getInputStream();InputStreamLiner isl = new InputStreamLiner(is);){
			if (!f.exists()) {
				f.createNewFile();
				System.out.println("fichier créée\n");
			}
			fos = new FileOutputStream(f);
			while ( (ligne = isl.readLine(InputStreamLiner.UNIX)) != null) {
				System.out.println(ligne);
				fos.write(ligne.getBytes());
			}
			fos.close();
			System.out.println("fin processPut \n");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void processGet(String s) throws IOException {
		String ligne;
		File f = new File(s);
		try {
			OutputStream output = socket.getOutputStream();
			InputStream is = new FileInputStream(f);			
			InputStreamLiner isl = new InputStreamLiner(is);
			while ((ligne =  isl.readLine(InputStreamLiner.UNIX)) != null)  {
				System.out.println(ligne);
				output.write(ligne.getBytes());
				output.flush();
			}
			isl.close();
		} catch (IOException e) {
			System.out.println("processGet IOException \n ");
			throw new IOException(e.getMessage());
		}
	}

	@Override
	public void run() {
		try {
			processRequest(socket.getInputStream());
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				if (socket != null) {
					socket.close();
				}
			} catch (IOException e) {}
		}

	}
}
