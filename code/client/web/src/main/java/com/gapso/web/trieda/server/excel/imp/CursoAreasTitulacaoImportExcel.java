package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class CursoAreasTitulacaoImportExcel
	extends AbstractImportExcel< CursoAreasTitulacaoImportExcelBean >
{
	static public String CURSO_COLUMN_NAME;
	static public String AREA_TITULACAO_COLUMN_NAME;

	private List< String > headerColumnsNames;

	public CursoAreasTitulacaoImportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( CURSO_COLUMN_NAME );
		this.headerColumnsNames.add( AREA_TITULACAO_COLUMN_NAME );
	}

	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.CURSO_AREAS_TITULACAO.getSheetName().equals( sheetName );
	}

	@Override
	protected List< String > getHeaderColumnsNames(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		return this.headerColumnsNames;
	}

	@Override
	protected CursoAreasTitulacaoImportExcelBean createExcelBean(
		Row header, Row row, int sheetIndex,
		Sheet sheet, Workbook workbook )
	{
		CursoAreasTitulacaoImportExcelBean bean
			= new CursoAreasTitulacaoImportExcelBean( row.getRowNum() + 1 );

        for ( int cellIndex = row.getFirstCellNum();
              cellIndex <= row.getLastCellNum(); cellIndex++ )
        {
            Cell cell = row.getCell( cellIndex );

        	if ( cell != null )
        	{
        		Cell headerCell = header.getCell( cell.getColumnIndex() );

        		if ( headerCell != null )
        		{
        			String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue( cell );

					if ( CURSO_COLUMN_NAME.equals( columnName ) )
					{
						bean.setCursoStr( cellValue );
					}
					else if( AREA_TITULACAO_COLUMN_NAME.equals( columnName ) )
					{
						bean.setAreaTitulacaoStr(cellValue );
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
		return ExcelInformationType.CURSO_AREAS_TITULACAO.getSheetName();
	}
	
	@Override
	protected void processSheetContent( String sheetName,
		List< CursoAreasTitulacaoImportExcelBean > sheetContent )
	{
		if ( doSyntacticValidation( sheetName, sheetContent )
			&& doLogicValidation( sheetName, sheetContent ) )
		{
			updateDataBase( sheetName, sheetContent );
		}
	}

	private boolean doSyntacticValidation( String sheetName,
		List< CursoAreasTitulacaoImportExcelBean > sheetContent )
	{
		// Map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map< ImportExcelError, List< Integer > > syntacticErrorsMap
			= new HashMap< ImportExcelError, List< Integer > >();

		for ( CursoAreasTitulacaoImportExcelBean bean : sheetContent )
		{
			List< ImportExcelError > errorsBean = bean.checkSyntacticErrors();

			for ( ImportExcelError error : errorsBean )
			{
				List< Integer > rowsWithErrors = syntacticErrorsMap.get( error );

				if ( rowsWithErrors == null )
				{
					rowsWithErrors = new ArrayList< Integer >();
					syntacticErrorsMap.put( error, rowsWithErrors );
				}

				rowsWithErrors.add( bean.getRow() );
			}
		}
		
		// coleta os erros e adiciona os mesmos na lista de mensagens
		for (ImportExcelError error : syntacticErrorsMap.keySet()) {
			List<Integer> linhasComErro = syntacticErrorsMap.get(error);
			getErrors().add(error.getMessage(linhasComErro.toString(),getI18nMessages()));
		}
		
		return syntacticErrorsMap.isEmpty();
	}

	private boolean doLogicValidation(
		String sheetName, List< CursoAreasTitulacaoImportExcelBean > sheetContent )
	{
		// Verifica se há referência a algum registro não cadastrado
		checkNonRegisteredCurso(sheetContent );
		checkNonRegisteredAreaTitulacao( sheetContent );

		return getErrors().isEmpty();
	}

	private void checkNonRegisteredCurso(
		List< CursoAreasTitulacaoImportExcelBean > sheetContent )
	{
		// [ CódidoCurso -> Curso ]
		Map< String, Curso > cursosBDMap = Curso.buildCursoCodigoToCursoMap(
			Curso.findByCenario( this.instituicaoEnsino, getCenario() ) );

		List< Integer > rowsWithErrors = new ArrayList< Integer >();

		for ( CursoAreasTitulacaoImportExcelBean bean : sheetContent )
		{
			Curso curso = cursosBDMap.get( bean.getCursoStr() );

			if ( curso != null )
			{
				bean.setCurso( curso );
			}
			else
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				CURSO_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}

	private void checkNonRegisteredAreaTitulacao(
		List< CursoAreasTitulacaoImportExcelBean > sheetContent )
	{
		// [ CódidoAreaTitulacao -> AreaTitulacao ]
		Map< String, AreaTitulacao > areasBDMap
			= AreaTitulacao.buildAreaTitulacaoCodigoToAreaTitulacaoMap(
			AreaTitulacao.findByCenario( this.instituicaoEnsino, getCenario() ) );

		List< Integer > rowsWithErrors = new ArrayList< Integer >();
	
		for ( CursoAreasTitulacaoImportExcelBean bean : sheetContent )
		{
			AreaTitulacao areaTitulacao
				= areasBDMap.get( bean.getAreaTitulacaoStr() );

			if ( areaTitulacao != null )
			{
				bean.setAreaTitulacao( areaTitulacao );
			}
			else
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				AREA_TITULACAO_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}

	@Transactional
	private void updateDataBase( String sheetName,
		List< CursoAreasTitulacaoImportExcelBean > sheetContent )
	{
		Set< AreaTitulacao > areasMerge = new HashSet< AreaTitulacao >();

		for ( CursoAreasTitulacaoImportExcelBean cursoAreasTitulacaoImportExcelBean : sheetContent )
		{
			AreaTitulacao areaTitulacao = cursoAreasTitulacaoImportExcelBean.getAreaTitulacao();
			Curso curso = cursoAreasTitulacaoImportExcelBean.getCurso();
			areaTitulacao.getCursos().add( curso );
			areasMerge.add( areaTitulacao );
		}

		for ( AreaTitulacao areaTitulacao : areasMerge )
		{
			areaTitulacao.merge();
		}
	}

	private void resolveHeaderColumnNames()
	{
		if ( CURSO_COLUMN_NAME == null )
		{
			CURSO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().curso() );
			AREA_TITULACAO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().areaTitulacao() );
		}
	}
}
