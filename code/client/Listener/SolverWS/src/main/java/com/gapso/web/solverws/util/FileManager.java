package com.gapso.web.solverws.util;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class FileManager {

	public static boolean createFile(Long uniqueID, BufferedInputStream bis) {
		try {
			BufferedOutputStream out = new BufferedOutputStream(new FileOutputStream("input"+uniqueID.toString()));
			int data;
			byte[] buff = new byte[32 * 1024];
			while ((data = bis.read(buff)) != -1) {
				out.write(buff, 0, data);
			}
			out.close();
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	public static byte[] getContentOutputFile(Long uniqueID) throws IOException {
		File file = new File("output"+uniqueID.toString()+"F");
		InputStream is = new FileInputStream(file);

		// Get the size of the file
		long length = file.length();

		// You cannot create an array using a long type.
		// It needs to be an int type.
		// Before converting to an int type, check
		// to ensure that file is not larger than Integer.MAX_VALUE.
		if (length > Integer.MAX_VALUE) {
			throw new IOException("Could not completely read file (very large) " + file.getName());
		}

		// Create the byte array to hold the data
		byte[] bytes = new byte[(int) length];

		// Read in the bytes
		int offset = 0;
		int numRead = 0;
		while (offset < bytes.length && (numRead = is.read(bytes, offset, bytes.length - offset)) >= 0) {
			offset += numRead;
		}

		// Ensure all the bytes have been read in
		if (offset < bytes.length) {
			throw new IOException("Could not completely read file " + file.getName());
		}

		is.close();
		return bytes;
	}

	
	public static boolean isExistInputFile(Long uniqueID) {
		return (new File("input"+uniqueID.toString())).exists();
	}
	public static boolean isExistOutputFile(Long uniqueID) {
		return (new File("output"+uniqueID.toString() + "F")).exists();
	}
	
	public static boolean removeInputFile(Long uniqueID) {
		return (new File("input"+uniqueID.toString())).delete();
	}
	public static boolean removeOutputFile(Long uniqueID) {
		return (new File("output"+uniqueID.toString() + "F")).delete();
	}
	public static boolean removeFiles(Long uniqueID) {
		return removeInputFile(uniqueID) && removeOutputFile(uniqueID);
	}

}
