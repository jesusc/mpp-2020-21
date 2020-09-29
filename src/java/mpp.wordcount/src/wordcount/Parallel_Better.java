package wordcount;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Parallel_Better {
	
	private List<File> files;
	
	private Lock lock = new ReentrantLock();

	private int count;

	public Parallel_Better(List<File> files) {
		this.files = files;
	}

	public void doCount() {	
		int numThreads = Runtime.getRuntime().availableProcessors();
		List<List<File>> split = Utils.split(files, numThreads);
		List<Counter> threads = new ArrayList<>();
		
		for (List<File> files : split) {
			Counter counter = new Counter(files);
			threads.add(counter);
			counter.start();
		}
		
		this.count = 0;
		for (Counter counter : threads) {
			try {
				counter.join();
				count += counter.getLocalCount();
			} catch (InterruptedException e) { }
		}
		
	}


	public int getCount() {
		return count;
	}
	
	public static void main(String[] args) {
		List<File> files = Utils.getFiles(args[0]);

		long init = System.currentTimeMillis();

		Parallel_Better counter = new Parallel_Better(files);
		counter.doCount();
		int count = counter.getCount();
		
		long end = System.currentTimeMillis();

		double time = (end - init) / (1_000.0);
		
		System.out.println("Total palabras: " + count + " en " + files.size() + " ficheros");
		System.out.println(String.format("%.2f", time));
	}
	
	
	public class Counter extends Thread {
		
		private List<File> files;
		private int localCount;

		public Counter(List<File> files) {
			this.files = files;
		}
		
		@Override
		public void run() {
			localCount = 0;
			for (File file : files) {
				try {
					localCount += Utils.countWords(file);
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			
		}

		public int getLocalCount() {
			return localCount;
		}
		
	}
	

}
