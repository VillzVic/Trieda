package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.gapso.trieda.domain.Campus;

public class CampiDeslocamentoImportExcelBean extends AbstractImportExcelBean
	implements Comparable< CampiDeslocamentoImportExcelBean >
{
	private String campusOrigemStr;
	private Map<String, String> campusDestinoStrToTempoStrMap = new HashMap<String, String>();

	private Campus campusOrigem;
	private Map<Campus, Integer> campusDestinoToTempoMap = new HashMap<Campus, Integer>();
	
	public CampiDeslocamentoImportExcelBean( int row )
	{
		super( row );
	}
	
	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();
	
		if ( !tudoVazio() )
		{
			checkMandatoryField( this.campusOrigemStr, ImportExcelError.CAMPI_DESLOCAMENTO_CAMPUS_ORIGEM_VAZIO, erros );

		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}
	
		return erros;
	}
	
	private boolean tudoVazio()
	{
		return (isEmptyField( this.campusOrigemStr ) );
	}
	
	public String getCampusOrigemStr()
	{
		return this.campusOrigemStr;
	}
	
	public void setCampusOrigemStr( String campusOrigemStr )
	{
		this.campusOrigemStr = campusOrigemStr;
	}
	
	public Map<String, String> getCampusDestinoStrToTempoStrMap()
	{
		return this.campusDestinoStrToTempoStrMap;
	}
	
	public void addCampusDestinoStr( String campus, String tempo )
	{
		this.campusDestinoStrToTempoStrMap.put(campus, tempo);
	}
	
	public void setCampusOrigem( Campus campusOrigem )
	{
		this.campusOrigem = campusOrigem;
	}
	
	public Campus getCampusOrigem()
	{
		return this.campusOrigem;
	}
	
	public void addCampusDestino( Campus campus, Integer tempo )
	{
		this.campusDestinoToTempoMap.put(campus, tempo);
	}

	public Map<Campus, Integer> getCampusDestinoToTempoMap()
	{
		return this.campusDestinoToTempoMap;
	}
		
	@Override
	public int compareTo( CampiDeslocamentoImportExcelBean o )
	{
		return this.getDeslocamentoKey().compareTo(o.getDeslocamentoKey());
	}
	
	public String getDeslocamentoKey()
	{
		return getCampusOrigemStr() + "-" + getCampusDestinoStrToTempoStrMap();
	}
}
