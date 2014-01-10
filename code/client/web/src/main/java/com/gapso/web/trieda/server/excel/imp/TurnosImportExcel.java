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
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class TurnosImportExcel
	extends AbstractImportExcel< TurnosImportExcelBean >
{
	static public String NOME_COLUMN_NAME;
	private List< String > headerColumnsNames;

	public TurnosImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( NOME_COLUMN_NAME );
	}

	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.TURNOS.getSheetName().equals( sheetName );
	}

	@Override
	protected List< String > getHeaderColumnsNames(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		return this.headerColumnsNames;
	}

	@Override
	protected TurnosImportExcelBean createExcelBean(
		Row header, Row row, int sheetIndex,
		Sheet sheet, Workbook workbook )
	{
		TurnosImportExcelBean bean
			= new TurnosImportExcelBean( row.getRowNum() + 1 );

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

					if ( NOME_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setNomeStr( cellValue );
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
		return ExcelInformationType.TURNOS.getSheetName();
	}
	
	@Override
	protected void processSheetContent(
		String sheetName, List< TurnosImportExcelBean > sheetContent )
	{
		if ( doSyntacticValidation( sheetName, sheetContent )
			&& doLogicValidation( sheetName, sheetContent ) )
		{
			updateDataBase( sheetName, sheetContent );
		}
	}

	private boolean doSyntacticValidation(
		String sheetName, List< TurnosImportExcelBean > sheetContent )
	{
		// Map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map< ImportExcelError, List< Integer > > syntacticErrorsMap
			= new HashMap< ImportExcelError, List< Integer > >();

		for ( TurnosImportExcelBean bean : sheetContent )
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
		String sheetName, List< TurnosImportExcelBean > sheetContent )
	{
		// Verifica se algum turno apareceu
		// mais de uma vez no arquivo de entrada
		checkUniqueness(sheetContent);

		return getErrors().isEmpty();
	}

	private void checkUniqueness(
		List< TurnosImportExcelBean > sheetContent )
	{
		// Map com os códigos dos turnos e as
		// linhas em que a mesmo aparece no arquivo de entrada
		// [ CodigoTurno -> Lista de Linhas do Arquivo de Entrada ]
		Map< String, List< Integer > > turnoCodigoToRowsMap
			= new HashMap< String, List< Integer > >();

		for ( TurnosImportExcelBean bean : sheetContent )
		{
			List< Integer > rows
				= turnoCodigoToRowsMap.get( bean.getNomeStr() );

			if ( rows == null )
			{
				rows = new ArrayList< Integer >();
				turnoCodigoToRowsMap.put(
					bean.getNomeStr(), rows );
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
		List< TurnosImportExcelBean > sheetContent )
	{
		Map< String, Turno > turnosBDMap = Turno.buildTurnoNomeToTurnoMap(
			Turno.findByCenario( this.instituicaoEnsino, getCenario() ) );

		for ( TurnosImportExcelBean turnoExcel : sheetContent )
		{
			Turno turnoBD = turnosBDMap.get( turnoExcel.getNomeStr() );
			if ( turnoBD != null )
			{
				// Update
				turnoBD.setNome( turnoExcel.getNomeStr() );

				turnoBD.merge();
			}
			else
			{
				// Insert
				Turno newTurno = new Turno();

				newTurno.setNome( turnoExcel.getNomeStr() );
				newTurno.setCenario( getCenario() );
				newTurno.setInstituicaoEnsino( instituicaoEnsino );

				newTurno.persist();
			}
		}
	}
	
	private void resolveHeaderColumnNames()
	{
		if ( NOME_COLUMN_NAME == null )
		{
			NOME_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().nome() );
		}
	}
}