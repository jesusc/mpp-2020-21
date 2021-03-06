package wordcount;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

public class Parallel {
	
	private List<File> files;
	
	// Variable compartida por los hilos
	private int count;
	

	public Parallel(List<File> files) {
		this.files = files;
	}

	public void doCount() {
		count = 0;
		
		
		int numThreads = Runtime.getRuntime().availableProcessors();
		List<List<File>> split = Utils.split(files, numThreads);
		List<Counter> threads = new ArrayList<>();
		
		for (List<File> files : split) {
			Counter counter = new Counter(files);
			threads.add(counter);
			counter.start();
		}
		
		for (Counter counter : threads) {
			try {
				counter.join();
			} catch (InterruptedException e) { }
		}
		
	}


	public int getCount() {
		return count;
	}
	
	public static void main(String[] args) {
		List<File> files = Utils.getFiles(args[0]);

		long init = System.currentTimeMillis();

		Parallel counter = new Parallel(files);
		counter.doCount();
		int count = counter.getCount();
		
		long end = System.currentTimeMillis();

		double time = (end - init) / (1_000.0);
		
		System.out.println("Total palabras: " + count + " en " + files.size() + " ficheros");
		System.out.println(String.format("%.2f", time));
	}
	
	
	
	public class Counter extends Thread {
		
		private List<File> files;

		public Counter(List<File> files) {
			this.files = files;
		}
		
		@Override
		public void run() {
			int localCount = 0;
			for (File file : files) {
				try {
					localCount += Utils.countWords(file);
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			
			AtomicInteger atomic = new AtomicInteger();
			//atomic.accumulateAndGet(x, accumulatorFunction)
			count = count + localCount;
		}
		
	}
	
}
