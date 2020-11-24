package mpp.livescripts;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.stream.Collectors;

public class RubyRunner {

	public void run(String scriptText) throws IOException, TimeoutException, InterruptedException {
        File scriptFile = File.createTempFile("run", ".rb");
        File stdout = File.createTempFile("stdout", ".txt");

        scriptText = preprocess(scriptText);
        System.out.println("Evaluando: ");
        System.out.println(scriptText);
        
        try(FileWriter writer = new FileWriter(scriptFile)) {
        	writer.append(scriptText);
        }
        
        ProcessBuilder builder = new ProcessBuilder("ruby", scriptFile.getAbsolutePath());
        
        builder.redirectOutput(stdout);
        builder.redirectErrorStream(true); 

        Process process = builder.start();
        
        boolean waitOk = process.waitFor(5, TimeUnit.SECONDS);
        if (!waitOk) {
            process.destroy();
            try {
                process.getInputStream().close();
                process.getOutputStream().close();
                //stdin.delete();
                //stdout.delete();
            } catch (IOException e) {
                /* Ignored */
            }
            throw new TimeoutException("Time out!");
        }

        int ret = process.exitValue();
        try {
            process.getInputStream().close();
            process.getOutputStream().close();
        } catch (IOException e) {
            // Nada
        }        
        
        if (ret == 0) {
            try(BufferedReader reader = new BufferedReader(new FileReader(stdout))) {
            	String result = reader.lines().collect(Collectors.joining("\n"));
        		System.out.println(result);
            }
        }
        
        scriptFile.delete();
        stdout.delete();
	}

	private String preprocess(String scriptText) {
		return scriptText.replaceAll("#>\\s*(.+)\\s*", "\nputs $1\n");
	}
	
}
