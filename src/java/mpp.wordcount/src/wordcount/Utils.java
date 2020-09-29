package wordcount;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import com.google.common.collect.Lists;

public class Utils {

	public static int countWords(File f) throws IOException {
		int words = 0;

		try(BufferedReader reader = new BufferedReader(new FileReader(f))) {
			String line;
			while ((line = reader.readLine()) != null) {
				words += line.split("\\s").length;			
			}
		} 
		
		return words;
	}
	
	public static List<File> getFiles(String root) {
		List<File> result = new ArrayList<>();
		
		File[] files = new File(root).listFiles();
		for (File file : files) {
			if (file.isFile() && file.getName().endsWith(".txt")) {
				result.add(file);
			}
		}
		
		return result;
	}

	public static List<List<File>> split(List<File> files, int numThreads) {
		return Lists.partition(files, numThreads);
	}
	
}
