import java.io.*;

public class Runtime{
	static BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
	public static String readString() throws IOException{
		return Runtime.reader.readLine();
	}
	
	public static int readInt() throws IOException{
		return Integer.parseInt(Runtime.reader.readLine());
	}
}