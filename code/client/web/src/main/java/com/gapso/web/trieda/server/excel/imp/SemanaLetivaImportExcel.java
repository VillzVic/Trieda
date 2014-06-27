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

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class SemanaLetivaImportExcel
extends AbstractImportExcel< SemanaLetivaImportExcelBean >
{
	static public String CODIGO_COLUMN_NAME;
	static public String DESCRICAO_COLUMN_NAME;
	static public String DURACAO_COLUMN_NAME;
	static public String PERMITE_INTEVALO_COLUMN_NAME;
	private List< String > headerColumnsNames;

	public SemanaLetivaImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( CODIGO_COLUMN_NAME );
		this.headerColumnsNames.add( DESCRICAO_COLUMN_NAME );
		this.headerColumnsNames.add( DURACAO_COLUMN_NAME );
		this.headerColumnsNames.add( PERMITE_INTEVALO_COLUMN_NAME );
		
	}

	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.SEMANA_LETIVA.getSheetName().equals( sheetName );
	}

	@Override
	protected List< String > getHeaderColumnsNames(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		return this.headerColumnsNames;
	}

	@Override
	protected SemanaLetivaImportExcelBean createExcelBean(
		Row header, Row row, int sheetIndex,
		Sheet sheet, Workbook workbook )
	{
		SemanaLetivaImportExcelBean bean
			= new SemanaLetivaImportExcelBean( row.getRowNum() + 1 );

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

					if ( CODIGO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setCodigoStr( cellValue );
					}
					if ( DESCRICAO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDescricaoStr( cellValue );
					}
					if ( DURACAO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDuracaoStr( cellValue );
					}
					if ( PERMITE_INTEVALO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setPermiteIntervaloStr( cellValue );
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
		return ExcelInformationType.SEMANA_LETIVA.getSheetName();
	}
	
	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected void processSheetContent(
		String sheetName, List< SemanaLetivaImportExcelBean > sheetContent )
	{
		if ( doSyntacticValidation( sheetName, sheetContent )
			&& doLogicValidation( sheetName, sheetContent ) )
		{
			getProgressReport().setInitNewPartial("Atualizando banco de dados");
			updateDataBase( sheetName, sheetContent );
			getProgressReport().setPartial("Fim de Atualizando banco de dados");
		}
	}

	private boolean doSyntacticValidation(
		String sheetName, List< SemanaLetivaImportExcelBean > sheetContent )
	{
		// Map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map< ImportExcelError, List< Integer > > syntacticErrorsMap
			= new HashMap< ImportExcelError, List< Integer > >();

		for ( SemanaLetivaImportExcelBean bean : sheetContent )
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
		String sheetName, List< SemanaLetivaImportExcelBean > sheetContent )
	{
		// Verifica se algum turno apareceu
		// mais de uma vez no arquivo de entrada
		checkUniqueness(sheetContent);

		return getErrors().isEmpty();
	}
	
	private void checkUniqueness(
		List< SemanaLetivaImportExcelBean > sheetContent )
	{
		// Map com os códigos dos turnos e as
		// linhas em que a mesmo aparece no arquivo de entrada
		// [ CodigoTurno -> Lista de Linhas do Arquivo de Entrada ]
		Map< String, List< Integer > > turnoCodigoToRowsMap
			= new HashMap< String, List< Integer > >();

		for ( SemanaLetivaImportExcelBean bean : sheetContent )
		{
			List< Integer > rows
				= turnoCodigoToRowsMap.get( bean.getCodigoStr() );

			if ( rows == null )
			{
				rows = new ArrayList< Integer >();
				turnoCodigoToRowsMap.put(
					bean.getCodigoStr(), rows );
			}
		
			rows.add( bean.getRow() );
		}

		// Verifica se algum turno apareceu
		// mais de uma vez no arquivo de entrada
		for ( Entry< String, List< Integer > > entry : turnoCodigoToRowsMap.entrySet() )
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
		List< SemanaLetivaImportExcelBean > sheetContent )
	{
		Map< String, SemanaLetiva > semanaLetivaBDMap = SemanaLetiva.buildSemanaLetivaCodigoToSemanaLetivaMap(
			SemanaLetiva.findByCenario( this.instituicaoEnsino, getCenario() ) );

		for ( SemanaLetivaImportExcelBean semanaLetivaExcel : sheetContent )
		{
			SemanaLetiva semanaLetivaBD = semanaLetivaBDMap.get( semanaLetivaExcel.getCodigoStr() );
			if ( semanaLetivaBD != null )
			{
				// Update
				semanaLetivaBD.setCodigo( semanaLetivaExcel.getCodigoStr() );
				semanaLetivaBD.setDescricao( semanaLetivaExcel.getDescricaoStr() );
				semanaLetivaBD.setTempo( semanaLetivaExcel.getDuracao() );
				semanaLetivaBD.setPermiteIntervaloAula( semanaLetivaExcel.getPermiteIntervalo() );

				semanaLetivaBD.merge();
			}
			else
			{
				// Insert
				SemanaLetiva newSemanaLetiva = new SemanaLetiva();

				newSemanaLetiva.setCodigo( semanaLetivaExcel.getCodigoStr() );
				newSemanaLetiva.setDescricao( semanaLetivaExcel.getDescricaoStr() );
				newSemanaLetiva.setTempo( semanaLetivaExcel.getDuracao() );
				newSemanaLetiva.setPermiteIntervaloAula( semanaLetivaExcel.getPermiteIntervalo() );

				newSemanaLetiva.setCenario( getCenario() );
				newSemanaLetiva.setInstituicaoEnsino( instituicaoEnsino );

				newSemanaLetiva.persist();
			}
		}
	}
	
	private void resolveHeaderColumnNames()
	{
		if ( CODIGO_COLUMN_NAME == null )
		{
			CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape( "Código Semana Letiva" );
			DESCRICAO_COLUMN_NAME = HtmlUtils.htmlUnescape( "Descrição" );
			DURACAO_COLUMN_NAME = HtmlUtils.htmlUnescape( "Duração do Tempo de Aula (min)" );
			PERMITE_INTEVALO_COLUMN_NAME = HtmlUtils.htmlUnescape( "Permite Intervalo Entre Aula de 2 Créditos?" );
		}
	}
}