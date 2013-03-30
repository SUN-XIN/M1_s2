package TME2;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;


public class Client {
	private static Socket socket;
	private static int port;
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		port = 1024;
		File f = new File("test");
		String ligne = "PUT test HTML/1.0";
		try {
			InputStream is = new FileInputStream(f);
			InputStreamLiner isl = new InputStreamLiner(is);
			socket = new Socket("localhost",port);
			OutputStream os = socket.getOutputStream();
			os.write(ligne.getBytes());
			while ((ligne = isl.readLine(InputStreamLiner.UNIX)) != null)  {
				System.out.println(ligne);
				os.write(ligne.getBytes());
				os.flush();
			}
			os.close();
			isl.close();
		} catch (UnknownHostException e) {
			System.err.println("main UnKnowHostException");
			e.printStackTrace();
		} catch (IOException e) {
			System.err.println("main IOException");
			e.printStackTrace();
		}
	}

}
