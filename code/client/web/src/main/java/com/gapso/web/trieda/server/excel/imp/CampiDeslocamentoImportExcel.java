package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class CampiDeslocamentoImportExcel
	extends AbstractImportExcel< CampiDeslocamentoImportExcelBean >
{
	static public String TEMPO_COLUMN_NAME;
	private List< String > headerColumnsNames;
	
	public CampiDeslocamentoImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();
	
		createHeaderColumnNames();

	}
	
	private void createHeaderColumnNames() {
		List< Campus > campi = Campus.findByCenario(instituicaoEnsino, getCenario());
		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add(TEMPO_COLUMN_NAME);
		for (Campus campus : campi)
		{
			this.headerColumnsNames.add(campus.getNome());
		}
	}

	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.CAMPI_DESLOCAMENTO.getSheetName().equals( sheetName );
	}
	
	@Override
	protected List< String > getHeaderColumnsNames(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		return this.headerColumnsNames;
	}
	
	@Override
	protected CampiDeslocamentoImportExcelBean createExcelBean(
		Row header, Row row, int sheetIndex,
		Sheet sheet, Workbook workbook )
	{
		CampiDeslocamentoImportExcelBean bean
			= new CampiDeslocamentoImportExcelBean( row.getRowNum() + 1 );

	    for ( int cellIndex = row.getFirstCellNum();
	       	  cellIndex <= row.getLastCellNum(); cellIndex++ )
	    {
	        Cell cell = row.getCell( cellIndex );
	
	    	if ( cell != null )
	    	{
	    		Cell headerCell
	    			= header.getCell( cell.getColumnIndex() );
	
	    		if ( headerCell != null )
	    		{
	    			String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue( cell );
					
					if ( TEMPO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setCampusOrigemStr( cellValue );
					}
					else
					{
						for (String columnHeader : this.headerColumnsNames)
						{
							if (columnHeader.endsWith(columnName))
							{
								bean.addCampusDestinoStr(columnName, cellValue);
							}
						}
					}		
	    		}
	    	}
	    }
	
		return bean;
	}
	
	@Override
	protected String getHeaderToString()
	{
		return this.headerColumnsNames.toString();
	}
	
	@Override
	public String getSheetName()
	{
		return ExcelInformationType.CAMPI_DESLOCAMENTO.getSheetName();
	}
	
	@Override
	protected void processSheetContent(
		String sheetName, List< CampiDeslocamentoImportExcelBean > sheetContent )
	{
		if ( doSyntacticValidation( sheetName, sheetContent )
			&& doLogicValidation( sheetName, sheetContent ) )
		{
			updateDataBase( sheetName, sheetContent );
		}
	}
	
	private boolean doSyntacticValidation(
		String sheetName, List< CampiDeslocamentoImportExcelBean > sheetContent )
	{
		// Map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map< ImportExcelError, List< Integer > > syntacticErrorsMap
			= new HashMap< ImportExcelError, List< Integer > >();
	
		for ( CampiDeslocamentoImportExcelBean bean : sheetContent )
		{
			List< ImportExcelError > errorsBean
				= bean.checkSyntacticErrors();
	
			for ( ImportExcelError error : errorsBean )
			{
				List< Integer> rowsWithErrors
					= syntacticErrorsMap.get( error );
	
				if ( rowsWithErrors == null )
				{
					rowsWithErrors = new ArrayList< Integer >();
					syntacticErrorsMap.put( error, rowsWithErrors );
				}
	
				rowsWithErrors.add( bean.getRow() );
			}
		}
	
		// Coleta os erros e adiciona os mesmos na lista de mensagens
		for ( ImportExcelError error : syntacticErrorsMap.keySet() )
		{
			List< Integer > linhasComErro
				= syntacticErrorsMap.get( error );
	
			getErrors().add( error.getMessage(
				linhasComErro.toString(), getI18nMessages() ) );
		}
	
		return syntacticErrorsMap.isEmpty();
	}
	
	private boolean doLogicValidation(
		String sheetName, List< CampiDeslocamentoImportExcelBean > sheetContent )
	{
		checkUniqueness(sheetContent);
		
		checkCampusNaoCadastrado(sheetContent);
		
		checkOrigensDestinos(sheetContent);
		
		checkDeslocamentosMesmoCampus(sheetContent);
		
		
	
		return getErrors().isEmpty();
	}

	private void checkCampusNaoCadastrado(
			List<CampiDeslocamentoImportExcelBean> sheetContent) {
		// [ CodigoCampus -> Campus ]
		Map< String, Campus > campusBDMap = Campus.buildCampusNomeToCampusMap(
			Campus.findByCenario( this.instituicaoEnsino, getCenario() ) );

		if ( campusBDMap == null || campusBDMap.size() == 0 )
		{
			return;
		}

		List< Integer > rowsWithErrors = new ArrayList< Integer >();

		for ( CampiDeslocamentoImportExcelBean bean : sheetContent )
		{
			Campus campusOrigem = campusBDMap.get( bean.getCampusOrigemStr() );

			if ( campusOrigem != null )
			{
				bean.setCampusOrigem( campusOrigem );
			}
			else
			{
				rowsWithErrors.add( bean.getRow() );
			}
			
			for (Entry<String, String> destinos : bean.getCampusDestinoStrToTempoStrMap().entrySet())
			{
				Campus campusDestino = campusBDMap.get(destinos.getKey());
				
				if (campusDestino != null)
				{
					bean.addCampusDestino(campusDestino, Integer.parseInt(destinos.getValue()));
				}
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				"Campus",rowsWithErrors.toString() ) );
		}
		
	}

	private void checkOrigensDestinos(
			List<CampiDeslocamentoImportExcelBean> sheetContent) {

		List< Integer > rowsWithErrors = new ArrayList< Integer >();
		
		List<String> campiOrigem = this.headerColumnsNames;
				
		for (int i = 1; i<campiOrigem.size(); i++)
		{
			if (sheetContent.get(i-1).getCampusOrigem() != null)
			{
				if (!sheetContent.get(i-1).getCampusOrigem().getNome().equals(campiOrigem.get(i)) )
				{
					rowsWithErrors.add( sheetContent.get(i-1).getRow() );
				}
			}
		}
		
		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoDeslocamentoAssimetrico(
					rowsWithErrors.toString() ) );
		}
		
	}

	private void checkDeslocamentosMesmoCampus(
			List<CampiDeslocamentoImportExcelBean> sheetContent) {

		List< Integer > rowsWithErrors = new ArrayList< Integer >();
		for (CampiDeslocamentoImportExcelBean bean : sheetContent )
		{
			for (Entry<String, String> destino : bean.getCampusDestinoStrToTempoStrMap().entrySet())
			{
				if (bean.getCampusOrigemStr().equals(destino.getKey()) && Integer.parseInt(destino.getValue()) != 0)
				{
					rowsWithErrors.add( bean.getRow() );
				}
			}
		}
		if ( !rowsWithErrors.isEmpty() )
		{
			getWarnings().add( getI18nMessages().excelAvisoMesmaEntidadeDeslocamento(
					rowsWithErrors.toString() ) );
		}
	}

	private void checkUniqueness(
		List< CampiDeslocamentoImportExcelBean > sheetContent )
	{
		// Map com as divisoes de creditos e as
		// linhas em que a mesmo aparece no arquivo de entrada
		// [ CodigoTurno -> Lista de Linhas do Arquivo de Entrada ]
		Map< String, List< Integer > > divisaoCodigoToRowsMap
			= new HashMap< String, List< Integer > >();
	
		for ( CampiDeslocamentoImportExcelBean bean : sheetContent )
		{
			List< Integer > rows
				= divisaoCodigoToRowsMap.get( bean.getCampusOrigemStr() );
	
			if ( rows == null )
			{
				rows = new ArrayList< Integer >();
				divisaoCodigoToRowsMap.put(
					bean.getCampusOrigemStr(), rows );
			}
		
			rows.add( bean.getRow() );
		}
	
		// Verifica se alguma divisao de credito apareceu
		// mais de uma vez no arquivo de entrada
		for ( Entry< String, List< Integer > > entry : divisaoCodigoToRowsMap.entrySet() )
		{
			if ( entry.getValue().size() > 1 )
			{
				getErrors().add( getI18nMessages().excelErroLogicoUnicidadeViolada(
					entry.getKey(), entry.getValue().toString() ) );
			}
		}
	}
	

	
	@Transactional
	private void updateDataBase( String sheetName,
		List< CampiDeslocamentoImportExcelBean > sheetContent )
	{
		List<DeslocamentoCampus> deslocamentos = DeslocamentoCampus.findAllByCampus(instituicaoEnsino, getCenario());
		
		for (DeslocamentoCampus deslocamento : deslocamentos)
		{
			deslocamento.remove();
		}
	
		for ( CampiDeslocamentoImportExcelBean deslocamentoExcel : sheetContent )
		{
			for (Entry<Campus, Integer> destinos : deslocamentoExcel.getCampusDestinoToTempoMap().entrySet())
			{
				if (destinos.getValue() != 0)
				{
					DeslocamentoCampus novoDeslocamento = new DeslocamentoCampus();
					novoDeslocamento.setOrigem(deslocamentoExcel.getCampusOrigem());
					novoDeslocamento.setDestino(destinos.getKey());
					novoDeslocamento.setTempo(destinos.getValue());
					
					novoDeslocamento.persist();
				}
			}
		}
	}
	
	private void resolveHeaderColumnNames()
	{
		if ( TEMPO_COLUMN_NAME == null )
		{
			TEMPO_COLUMN_NAME = HtmlUtils.htmlUnescape( "Tempo de deslocamento (min)" );
		}
	}
}