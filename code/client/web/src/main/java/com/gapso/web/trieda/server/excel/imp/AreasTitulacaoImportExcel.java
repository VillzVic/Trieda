package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class AreasTitulacaoImportExcel
	extends AbstractImportExcel< AreasTitulacaoImportExcelBean >
{
	static public String CODIGO_COLUMN_NAME;
	static public String DESCRICAO_COLUMN_NAME;

	private List< String > headerColumnsNames;

	public AreasTitulacaoImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( CODIGO_COLUMN_NAME );
		this.headerColumnsNames.add( DESCRICAO_COLUMN_NAME );
	}

	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.AREAS_TITULACAO.getSheetName().equals( sheetName );
	}

	@Override
	protected List< String > getHeaderColumnsNames(
		int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook )
	{
		return this.headerColumnsNames;
	}

	@Override
	protected AreasTitulacaoImportExcelBean createExcelBean(
		HSSFRow header, HSSFRow row, int sheetIndex,
		HSSFSheet sheet, HSSFWorkbook workbook )
	{
		AreasTitulacaoImportExcelBean bean
			= new AreasTitulacaoImportExcelBean( row.getRowNum() + 1 );

        for ( int cellIndex = row.getFirstCellNum();
              cellIndex <= row.getLastCellNum(); cellIndex++ )
        {
            HSSFCell cell = row.getCell( cellIndex );

        	if ( cell != null )
        	{
        		HSSFCell headerCell
        			= header.getCell( cell.getColumnIndex() );

        		if ( headerCell != null )
        		{
        			String columnName
        				= headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue( cell );

					if ( CODIGO_COLUMN_NAME.equals( columnName ) )
					{
						bean.setCodigoStr( cellValue );
					}
					else if ( DESCRICAO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDescricaoStr( cellValue );
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
		return ExcelInformationType.AREAS_TITULACAO.getSheetName();
	}

	@Override
	protected void processSheetContent(
		String sheetName, List< AreasTitulacaoImportExcelBean > sheetContent )
	{
		if ( doSyntacticValidation( sheetName, sheetContent )
			&& doLogicValidation( sheetName, sheetContent ) )
		{
			updateDataBase( sheetName, sheetContent );
		}
	}

	private boolean doSyntacticValidation(
		String sheetName, List< AreasTitulacaoImportExcelBean > sheetContent )
	{
		// Map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map< ImportExcelError, List< Integer > > syntacticErrorsMap
			= new HashMap< ImportExcelError, List< Integer > >();

		for ( AreasTitulacaoImportExcelBean bean : sheetContent )
		{
			List< ImportExcelError > errorsBean
				= bean.checkSyntacticErrors();

			for ( ImportExcelError error : errorsBean )
			{
				List< Integer > rowsWithErrors
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
		String sheetName, List< AreasTitulacaoImportExcelBean > sheetContent )
	{
		// Verifica se alguma area apareceu
		// mais de uma vez no arquivo de entrada
		checkUniqueness( sheetContent );

		return getErrors().isEmpty();
	}

	private void checkUniqueness(
		List< AreasTitulacaoImportExcelBean > sheetContent )
	{
		// Map com os códigos das areas e as linhas
		// em que a mesma aparece no arquivo de entrada
		// [ Código -> Lista de Linhas do Arquivo de Entrada ]
		Map< String, List< Integer > > codigoToRowsMap
			= new HashMap< String, List< Integer > >();
 
		for ( AreasTitulacaoImportExcelBean bean : sheetContent )
		{
			List< Integer > rows = codigoToRowsMap.get( bean.getCodigoStr() );

			if ( rows == null )
			{
				rows = new ArrayList< Integer >();
				codigoToRowsMap.put( bean.getCodigoStr(), rows );
			}

			rows.add( bean.getRow() );
		}

		// Verifica se alguma area apareceu
		// mais de uma vez no arquivo de entrada
		for ( Entry< String, List< Integer > > entry
			: codigoToRowsMap.entrySet() )
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
		List< AreasTitulacaoImportExcelBean > sheetContent )
	{
		Map< String, AreaTitulacao > areasTitulacaoBDMap
			= AreaTitulacao.buildAreaTitulacaoCodigoToAreaTitulacaoMap(
				AreaTitulacao.findAll( this.instituicaoEnsino ) );

		for ( AreasTitulacaoImportExcelBean areaExcel : sheetContent )
		{
			AreaTitulacao areaBD = areasTitulacaoBDMap.get( areaExcel.getCodigoStr() );

			if ( areaBD != null )
			{
				// Update
				areaBD.setDescricao( areaExcel.getDescricaoStr() );
				areaBD.merge();
			}
			else
			{
				// Insert
				AreaTitulacao newArea = new AreaTitulacao();

				newArea.setCodigo( areaExcel.getCodigoStr() );
				newArea.setDescricao( areaExcel.getDescricaoStr() );
				newArea.setInstituicaoEnsino( instituicaoEnsino );
				newArea.persist();
			}
		}
	}

	private void resolveHeaderColumnNames()
	{
		if ( CODIGO_COLUMN_NAME == null )
		{
			CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoAreaTitulacao() );
			DESCRICAO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().descricao() );
		}
	}
}
