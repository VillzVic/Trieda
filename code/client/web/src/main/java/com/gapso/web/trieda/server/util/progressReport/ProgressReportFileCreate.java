package com.gapso.web.trieda.server.util.progressReport;

import java.io.File;
import java.util.Random;

public class ProgressReportFileCreate {
	
	public static String geraIndice(){
		String letras = "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789";
		Random random = new Random();
		String armazenaChaves = "";
		int index;
		
		for(int i = 0; i < 9; i++){
			index = random.nextInt(letras.length());
			armazenaChaves += letras.substring(index, index + 1);
		}
		
		return armazenaChaves;
	}
	
	public static String getNewKey(){
		String newKey;
		File f;
		
		do{
			newKey = geraIndice();
			f = new File("trieda/" + newKey + ".txt");
			System.out.println(f.exists());
		}
		while(f.exists());
	
		return newKey;
	}
	
	public static File getFile(String fileName){
		return new File("trieda/" + fileName + ".txt");
	}
	
}
