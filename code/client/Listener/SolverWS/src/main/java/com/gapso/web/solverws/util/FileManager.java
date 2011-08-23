package com.gapso.web.solverws.util;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class FileManager
{
	private static final Logger logger
		= LoggerFactory.getLogger( FileManager.class );

	private static final String INPUT = "input";
	private static final String OUTPUT = "output";
	private final String baseDir;

	public FileManager()
	{
		this( System.getProperty( "java.io.tmpdir" ) );
	}

	public FileManager( String baseDir )
	{
		this.baseDir = baseDir;

	}

	private File getFile( String prefix,
		long id, boolean isFinal )
	{
		final StringBuffer name = new StringBuffer();

		name.append( baseDir ).append( File.separator )
			.append( prefix ).append( Long.toString( id ) );

		if ( isFinal )
		{
			name.append("F");
		}

		final String filename = name.toString();
		logger.info( "Filename: {}", filename );

		return new File( filename );
	}

	public boolean createFile( String filename,
		Long uniqueID, BufferedInputStream bis )
	{
		try
		{
			BufferedOutputStream out = new BufferedOutputStream(
				new FileOutputStream( getFile( filename, uniqueID, false ) ) );

			int data;
			byte[] buff = new byte[ 32 * 1024 ];

			while ( ( data = bis.read( buff ) ) != -1 )
			{
				out.write( buff, 0, data );
			}

			out.close();
		}
		catch ( IOException e )
		{
			e.printStackTrace();
			return false;
		}

		return true;
	}

	public boolean createFile(
		Long uniqueID, BufferedInputStream bis )
	{
		return createFile( INPUT, uniqueID, bis );
	}

	public byte [] getContentOutputFile( Long uniqueID )
		throws IOException
	{
		File file = getFile( OUTPUT, uniqueID, true );
		return readFile( file );
	}

	private byte [] readFile( File file )
		throws FileNotFoundException, IOException
	{
		InputStream is = new FileInputStream( file );

		// Get the size of the file
		long length = file.length();

		// You cannot create an array using a long type.
		// It needs to be an int type.
		// Before converting to an int type, check
		// to ensure that file is not larger than Integer.MAX_VALUE.
		if ( length > Integer.MAX_VALUE )
		{
			throw new IOException(
				"Could not completely read file (very large) " + file.getName() );
		}

		// Create the byte array to hold the data
		byte [] bytes = new byte[ (int)length ];

		// Read in the bytes
		int offset = 0;
		int numRead = 0;

		while ( offset < bytes.length
			&& ( numRead = is.read( bytes, offset, bytes.length - offset ) ) >= 0 )
		{
			offset += numRead;
		}

		// Ensure all the bytes have been read in
		if ( offset < bytes.length )
		{
			throw new IOException(
				"Could not completely read file " + file.getName() );
		}

		is.close();
		return bytes;
	}

	public boolean isExistInputFile( Long uniqueID )
	{
		return ( getFile( INPUT, uniqueID, false ) ).exists();
	}

	public boolean isExistOutputFile( Long uniqueID )
	{
		return ( getFile( OUTPUT, uniqueID, true ) ).exists();
	}

	public boolean removeInputFile( Long uniqueID )
	{
		return ( getFile( INPUT, uniqueID, false ) ).delete();
	}

	public boolean removeOutputFile( Long uniqueID )
	{
		return ( getFile( OUTPUT, uniqueID, true ) ).delete();
	}

	public boolean removeFiles( Long uniqueID )
	{
		return ( removeInputFile( uniqueID ) && removeOutputFile( uniqueID ) );
	}

	public byte [] getFile( String filename, long round )
		throws FileNotFoundException, IOException
	{
		File file = getFile( filename, round, false );
		return readFile( file );
	}
}
