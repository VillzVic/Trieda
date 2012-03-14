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

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class EquivalenciasImportExcel
	extends AbstractImportExcel< EquivalenciasImportExcelBean >
{
	static public String CURSOU_COLUMN_NAME;
	static public String ELIMINA_COLUMN_NAME;

	private List< String > headerColumnsNames;

	public EquivalenciasImportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( CURSOU_COLUMN_NAME );
		this.headerColumnsNames.add( ELIMINA_COLUMN_NAME );
	}

	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.EQUIVALENCIAS.getSheetName().equals( sheetName );
	}

	@Override
	protected List< String > getHeaderColumnsNames(
		int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook )
	{
		return this.headerColumnsNames;
	}

	@Override
	protected EquivalenciasImportExcelBean createExcelBean(
		HSSFRow header, HSSFRow row, int sheetIndex,
		HSSFSheet sheet, HSSFWorkbook workbook )
	{
		EquivalenciasImportExcelBean bean
			= new EquivalenciasImportExcelBean( row.getRowNum() + 1 );

        for ( int cellIndex = row.getFirstCellNum();
        	  cellIndex <= row.getLastCellNum(); cellIndex++ )
        {
            HSSFCell cell = row.getCell( cellIndex );

        	if ( cell != null )
        	{
        		HSSFCell headerCell = header.getCell( cell.getColumnIndex() );

        		if ( headerCell != null )
        		{
        			String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue( cell );

					if ( CURSOU_COLUMN_NAME.equals( columnName ) )
					{
						bean.setCursouStr( cellValue );
					}
					else if ( ELIMINA_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setEliminaStr( cellValue );
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
		return ExcelInformationType.EQUIVALENCIAS.getSheetName();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected void processSheetContent(
		String sheetName, List< EquivalenciasImportExcelBean > sheetContent )
	{
		if ( doSyntacticValidation( sheetName, sheetContent )
			&& doLogicValidation( sheetName, sheetContent ) )
		{
			updateDataBase( sheetName, sheetContent );
		}
	}

	private boolean doSyntacticValidation(
		String sheetName, List< EquivalenciasImportExcelBean > sheetContent )
	{
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map< ImportExcelError, List< Integer > > syntacticErrorsMap
			= new HashMap< ImportExcelError, List< Integer > >();

		for ( EquivalenciasImportExcelBean bean : sheetContent )
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
		String sheetName, List< EquivalenciasImportExcelBean > sheetContent )
	{
		// Verifica se alguma disciplina apareceu
		// mais de uma vez no arquivo de entrada
		// checkUniqueness( sheetContent );

		// Verifica se há referência a alguma unidade não cadastrada
		checkNonRegisteredDisciplina( sheetContent );

		return getErrors().isEmpty();
	}

	// FIXME
	// A disciplina 'cursou' pode aparacer mais de
	// uma vez na planilha de importação, pois ela
	// pode ser equivalente a mais de uma disciplina
	@SuppressWarnings( "unused" )
	private void checkUniqueness(
		List< EquivalenciasImportExcelBean > sheetContent )
	{
		// Map com os códigos dos cursou e as linhas
		// em que a mesmo aparece no arquivo de entrada

		// [ CódigoCursou -> Lista de Linhas do Arquivo de Entrada ]
		Map< String, List< Integer > > codigoToRowsMap
			= new HashMap< String, List< Integer > >();

		for ( EquivalenciasImportExcelBean bean : sheetContent )
		{
			List< Integer > rows
				= codigoToRowsMap.get( bean.getCursouStr() );

			if ( rows == null )
			{
				rows = new ArrayList< Integer >();
				codigoToRowsMap.put( bean.getCursouStr(), rows );
			}

			rows.add( bean.getRow() );
		}

		// Verifica se algum cursou apareceu
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

	private void checkNonRegisteredDisciplina(
		List< EquivalenciasImportExcelBean > sheetContent )
	{
		// [ CódigoDisciplina -> Disciplina ]
		Map< String, Disciplina > disciplinaBDMap
			= Disciplina.buildDisciplinaCodigoToDisciplinaMap(
				Disciplina.findByCenario( this.instituicaoEnsino, getCenario() ) );

		List< Integer > rowsWithErrorsCursou = new ArrayList< Integer >();
		List< Integer > rowsWithErrorsElimina = new ArrayList< Integer >();

		for ( EquivalenciasImportExcelBean bean : sheetContent )
		{
			Disciplina disciplinaCursou
				= disciplinaBDMap.get( bean.getCursouStr() );

			Disciplina disciplinaElimina
				= disciplinaBDMap.get( bean.getEliminaStr() );

			if ( disciplinaCursou != null )
			{
				bean.setDisciplinaCursou( disciplinaCursou );
			}
			else
			{
				rowsWithErrorsCursou.add( bean.getRow() );
			}

			if ( disciplinaElimina != null )
			{
				bean.getDisciplinasElimina().add( disciplinaElimina );
			}
			else
			{
				rowsWithErrorsElimina.add( bean.getRow() );
			}
		}
		
		if ( !rowsWithErrorsCursou.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				CURSOU_COLUMN_NAME, rowsWithErrorsCursou.toString() ) );
		}

		if ( !rowsWithErrorsElimina.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				ELIMINA_COLUMN_NAME, rowsWithErrorsElimina.toString() ) );
		}

		/*
		for ( EquivalenciasImportExcelBean bean : sheetContent )
		{
			String [] eliminaList = bean.getEliminaStr().split( ";" );

			for ( String eliminaString : eliminaList )
			{
				Disciplina disciplina = disciplinaBDMap.get( eliminaString.trim() );

				if ( disciplina != null )
				{
					bean.getDisciplinasElimina().add( disciplina );
				}
				else
				{
					rowsWithErrorsCursou.add( bean.getRow() );
				}
			}
		}
		*/

		/*
		if ( !rowsWithErrorsCursou.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				CURSOU_COLUMN_NAME, rowsWithErrorsCursou.toString() ) );
		}

		if ( !rowsWithErrorsElimina.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				ELIMINA_COLUMN_NAME, rowsWithErrorsElimina.toString() ) );
		}
		*/
	}

	@Transactional
	@ProgressReportMethodScan(texto = "Atualizando banco de dados")
	private void updateDataBase( String sheetName,
		List< EquivalenciasImportExcelBean > sheetContent )
	{
		Map< String, Equivalencia > equivalenciasBDMap
			= Equivalencia.buildEquivalenciaCursouCodigoToEquivalenciaMap(
				Equivalencia.findAll( this.instituicaoEnsino ) );

		int count = 0, total=sheetContent.size(); System.out.print(" "+total);
		for ( EquivalenciasImportExcelBean equivalenciaExcel : sheetContent )
		{
			if ( equivalenciaExcel.getDisciplinaCursou() == null
				|| equivalenciaExcel.getDisciplinasElimina() == null
				|| equivalenciaExcel.getDisciplinasElimina().size() == 0 )
			{
				continue;
			}

			Equivalencia equivalenciaBD = equivalenciasBDMap.get(
				equivalenciaExcel.getCursouStr() );

			if ( equivalenciaBD != null )
			{
				// Update
				equivalenciaBD.setElimina(
					equivalenciaExcel.getDisciplinasElimina() );

				equivalenciaBD.merge();
			}
			else
			{
				// Insert
				Equivalencia newEquivalencia = new Equivalencia();

				newEquivalencia.setCursou( equivalenciaExcel.getDisciplinaCursou() );
				newEquivalencia.setElimina( equivalenciaExcel.getDisciplinasElimina() );

				newEquivalencia.persist();
			}
			
			count++;total--;if (count == 100) {System.out.println("   Faltam "+total+" equivalencias"); count = 0;}
		}
	}

	private void resolveHeaderColumnNames()
	{
		if ( CURSOU_COLUMN_NAME == null )
		{
			CURSOU_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().cursou() );
			ELIMINA_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().elimina() );
		}
	}
}
