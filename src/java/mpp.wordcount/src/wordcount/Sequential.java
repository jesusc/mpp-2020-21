package wordcount;

import java.io.File;
import java.io.IOException;
import java.util.List;

public class Sequential {

	public static void main(String[] args) throws IOException {
		
		List<File> files = Utils.getFiles(args[0]);
		int count = 0;
		
		long init = System.currentTimeMillis();
		
		
		for (File file : files) {
			count += Utils.countWords(file);
		}
		
		long end = System.currentTimeMillis();
		double time = (end - init) / (1_000.0);
		
		System.out.println("Total palabras: " + count + " en " + files.size() + " ficheros");
		System.out.println(String.format("%.2f", time));
	}
	
}
