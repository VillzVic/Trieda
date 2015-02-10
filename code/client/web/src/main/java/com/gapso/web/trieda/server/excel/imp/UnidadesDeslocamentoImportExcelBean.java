package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.gapso.trieda.domain.Unidade;

public class UnidadesDeslocamentoImportExcelBean extends AbstractImportExcelBean
	implements Comparable< UnidadesDeslocamentoImportExcelBean >
{
	private String unidadeOrigemStr;
	private Map<String, String> unidadeDestinoStrToTempoStrMap = new HashMap<String, String>();
	private List<String> unidadesDestino = new ArrayList<String>();
	
	private Unidade unidadeOrigem;
	private Map<Unidade, Integer> unidadeDestinoToTempoMap = new HashMap<Unidade, Integer>();
	
	public UnidadesDeslocamentoImportExcelBean( int row )
	{
		super( row );
	}
	
	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();
	
		if ( !tudoVazio() )
		{
			checkMandatoryField( this.unidadeOrigemStr, ImportExcelError.UNIDADES_DESLOCAMENTO_UNIDADE_ORIGEM_VAZIO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}
	
		return erros;
	}
	
	private boolean tudoVazio()
	{
		return (isEmptyField( this.unidadeOrigemStr ) );
	}
	
	public String getUnidadeOrigemStr()
	{
		return this.unidadeOrigemStr;
	}
	
	public void setUnidadeOrigemStr( String unidadeOrigemStr )
	{
		this.unidadeOrigemStr = unidadeOrigemStr;
	}
	
	public Map<String, String> getUnidadeDestinoStrToTempoStrMap()
	{
		return this.unidadeDestinoStrToTempoStrMap;
	}
	
	public List<String> getUnidadesDestinos() {
		return this.unidadesDestino;
	}
	
	public void addUnidadeDestinoStr( String unidade, String tempo )
	{
		this.unidadeDestinoStrToTempoStrMap.put(unidade, tempo);
		this.unidadesDestino.add(unidade);
	}
	
	public void setUnidadeOrigem( Unidade unidadeOrigem )
	{
		this.unidadeOrigem = unidadeOrigem;
	}
	
	public Unidade getUnidadeOrigem()
	{
		return this.unidadeOrigem;
	}
	
	public void addUnidadeDestino( Unidade unidade, Integer tempo )
	{
		this.unidadeDestinoToTempoMap.put(unidade, tempo);
	}
	
	public Map<Unidade, Integer> getUnidadeDestinoToTempoMap()
	{
		return this.unidadeDestinoToTempoMap;
	}
		
	@Override
	public int compareTo( UnidadesDeslocamentoImportExcelBean o )
	{
		return this.getDeslocamentoKey().compareTo(o.getDeslocamentoKey());
	}
	
	public String getDeslocamentoKey()
	{
		return getUnidadeOrigemStr() + "-" + getUnidadeDestinoStrToTempoStrMap();
	}
}