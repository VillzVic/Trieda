package com.gapso.web.trieda.server.util;

import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Encryption
{
	public static String toMD5( String text )
	{
        String sen = "";  
        MessageDigest md = null;

        try
        {  
            md = MessageDigest.getInstance( "MD5" );  
        } catch ( NoSuchAlgorithmException e )
        {  
            e.printStackTrace();  
        }

        BigInteger hash = new BigInteger( 1, md.digest( text.getBytes() ) );  
        sen = hash.toString( 16 );

        return sen;
	}
}
