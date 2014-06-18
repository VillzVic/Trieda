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
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class DisciplinasCoRequisitosImportExcel
	extends AbstractImportExcel< DisciplinasRequisitosImportExcelBean >
{
	static public String CURRICULO_COLUMN_NAME;
	static public String PERIODO_COLUMN_NAME;
	static public String DISCIPLINA_COLUMN_NAME;
	static public String DISCIPLINA_REQUISITO_COLUMN_NAME;
	
	private List< String > headerColumnsNames;
	
	public DisciplinasCoRequisitosImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();
	
		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( CURRICULO_COLUMN_NAME );
		this.headerColumnsNames.add( PERIODO_COLUMN_NAME );
		this.headerColumnsNames.add( DISCIPLINA_COLUMN_NAME );
		this.headerColumnsNames.add( DISCIPLINA_REQUISITO_COLUMN_NAME );
	}
	
	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.DISCIPLINAS_CO_REQUISITOS.getSheetName().equals( sheetName );
	}
	
	@Override
	protected List< String > getHeaderColumnsNames(
		int sheetIndex, Sheet sheet, Workbook workbook )
		{
		return this.headerColumnsNames;
	}
	
	@Override
	protected DisciplinasRequisitosImportExcelBean createExcelBean(
		Row header, Row row, int sheetIndex,
		Sheet sheet, Workbook workbook )
	{
		DisciplinasRequisitosImportExcelBean bean
			= new DisciplinasRequisitosImportExcelBean( row.getRowNum() + 1 );
	
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
	
					if ( CURRICULO_COLUMN_NAME.equals( columnName ) )
					{
						bean.setCurriculoStr( cellValue );
					}
					else if ( PERIODO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setPeriodoStr( cellValue );
					}
					else if ( DISCIPLINA_COLUMN_NAME.equals( columnName ) )
					{
						bean.setDisciplinaStr( cellValue );
					}
					else if ( DISCIPLINA_REQUISITO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDisciplinaRequisitoStr( cellValue );
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
		return ExcelInformationType.DISCIPLINAS_PRE_REQUISITOS.getSheetName();
	}
	
	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected void processSheetContent(
		String sheetName, List< DisciplinasRequisitosImportExcelBean > sheetContent )
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
		String sheetName, List< DisciplinasRequisitosImportExcelBean > sheetContent )
	{
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre
	
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map< ImportExcelError, List< Integer > > syntacticErrorsMap
			= new HashMap< ImportExcelError, List< Integer > >();
	
		for ( DisciplinasRequisitosImportExcelBean bean : sheetContent )
		{
			List< ImportExcelError > errorsBean = bean.checkSyntacticErrors();
	
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
			List< Integer > linhasComErro = syntacticErrorsMap.get( error );
			getErrors().add( error.getMessage(
				linhasComErro.toString(), getI18nMessages() ) );
		}
	
		return syntacticErrorsMap.isEmpty();
	}
	
	private boolean doLogicValidation(
		String sheetName, List< DisciplinasRequisitosImportExcelBean > sheetContent )
	{
		// Verifica se algum CurriculoDisciplina-DisciplinaRequisito apareceu
		// mais de uma vez no arquivo de entrada
		checkUniqueness( sheetContent );
	
		// Verifica se há referência a
		// algum CurriculoDisciplina não cadastrado
		checkNonRegisteredCurriculoDisciplina( sheetContent );
		
		// Verifica se há referência a
		// alguma disciplina não cadastrada
		checkNonRegisteredDisciplina( sheetContent );
		
		return getErrors().isEmpty();
	}
	
	private void checkUniqueness(
		List< DisciplinasRequisitosImportExcelBean > sheetContent )
	{
		// Map com os códigos das disciplinas e as
		// linhas em que o mesmo aparece no arquivo de entrada
	
		// [ CurriculoDisciplina-DisciplinaRequisito -> Lista de Linhas do Arquivo de Entrada ]
		Map< String, List< Integer > > disciplinaRequisitoToRowsMap
			= new HashMap< String, List< Integer > >();
	
		for ( DisciplinasRequisitosImportExcelBean bean : sheetContent )
		{
			List< Integer > rows = disciplinaRequisitoToRowsMap.get( bean.getCurriculoStr()
					+ "-" + bean.getPeriodoStr() + "-" + bean.getDisciplinaStr() + "-" +	bean.getDisciplinaRequisitoStr() );
	
			if ( rows == null )
			{
				rows = new ArrayList< Integer >();
				disciplinaRequisitoToRowsMap.put( bean.getCurriculoStr() + "-" + bean.getPeriodoStr() + "-" +
						bean.getDisciplinaStr() + "-" +	bean.getDisciplinaRequisitoStr(), rows );
			}
	
			rows.add( bean.getRow() );
		}
	
		// Verifica se alguma disciplina apareceu
		// mais de uma vez no arquivo de entrada
		for ( Entry< String, List< Integer > > entry
			: disciplinaRequisitoToRowsMap.entrySet() )
		{
			if ( entry.getValue().size() > 1 )
			{
				getErrors().add( getI18nMessages().excelErroLogicoUnicidadeViolada(
					entry.getKey(), entry.getValue().toString() ) );
			}
		}
	}
	
	private void checkNonRegisteredDisciplina(
		List< DisciplinasRequisitosImportExcelBean > sheetContent )
	{
		// [ CodigoDisciplina -> Disciplina ]
		Map< String, Disciplina > disciplinaBDMap
			= Disciplina.buildDisciplinaCodigoToDisciplinaMap(
					Disciplina.findByCenario(instituicaoEnsino, getCenario()) );
	
		List< Integer > rowsWithErrors = new ArrayList< Integer >();
	
		for ( DisciplinasRequisitosImportExcelBean bean : sheetContent )
		{
			Disciplina disciplina
				= disciplinaBDMap.get( bean.getDisciplinaRequisitoStr() );
	
			if ( disciplina != null )
			{
				bean.setDisciplinaRequisito( disciplina );
			}
			else
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}
	
		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				DISCIPLINA_REQUISITO_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}
	
	private void checkNonRegisteredCurriculoDisciplina(
		List< DisciplinasRequisitosImportExcelBean > sheetContent )
	{
		// [ CurriculoDisciplinaNaturalKey -> CurriculoDisciplina ]
		Map< String, CurriculoDisciplina > curriculoDisciplinaBDMap
			= CurriculoDisciplina.buildNaturalKeySemCursoToCurriculoDisciplinaMap(
					CurriculoDisciplina.findByCenario(instituicaoEnsino, getCenario()));
	
		List< Integer > rowsWithErrors = new ArrayList< Integer >();
	
		for ( DisciplinasRequisitosImportExcelBean bean : sheetContent )
		{
			CurriculoDisciplina curriculoDisciplina
				= curriculoDisciplinaBDMap.get( bean.getCurriculoStr() + "-" +
						bean.getPeriodo().toString() + "-" + bean.getDisciplinaStr() );
			
			if ( curriculoDisciplina != null )
			{
				bean.setCurriculoDisciplina( curriculoDisciplina );
			}
			else
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}
	
		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
					CURRICULO_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}
	
	@Transactional
	@ProgressReportMethodScan(texto = "Atualizando banco de dados")
	private void updateDataBase(
		String sheetName, List< DisciplinasRequisitosImportExcelBean > sheetContent )
	{
		// [ CurriculoDisciplinaNaturalKey -> CurriculoDisciplina ]
		Map< String, CurriculoDisciplina > curriculoDisciplinaBDMap
			= CurriculoDisciplina.buildNaturalKeySemCursoToCurriculoDisciplinaMap(
					CurriculoDisciplina.findByCenario(instituicaoEnsino, getCenario()));
	
		for ( DisciplinasRequisitosImportExcelBean disciplinaExcel : sheetContent )
		{
			CurriculoDisciplina curriculoDisciplina
			= curriculoDisciplinaBDMap.get( disciplinaExcel.getCurriculoStr() + "-" +
					disciplinaExcel.getPeriodo().toString() + "-" + disciplinaExcel.getDisciplinaStr() );
	
			if ( curriculoDisciplina != null )
			{
				// Update
				curriculoDisciplina.getCoRequisitos().add( disciplinaExcel.getDisciplinaRequisito() );
			}
		}
	}
	
	private void resolveHeaderColumnNames()
	{
		if ( CURRICULO_COLUMN_NAME == null )
		{
			CURRICULO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().matrizCurricular() );
			PERIODO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().periodo() );
			DISCIPLINA_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoDisciplina() );
			DISCIPLINA_REQUISITO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoDisciplinaCoRequisito() );
		}
	}
}

