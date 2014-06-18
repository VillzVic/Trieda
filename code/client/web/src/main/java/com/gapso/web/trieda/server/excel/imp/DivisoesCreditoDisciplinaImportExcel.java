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
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.DivisaoCredito;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class DivisoesCreditoDisciplinaImportExcel
	extends AbstractImportExcel< DivisoesCreditoDisciplinaImportExcelBean >
{
	static public String DISCIPLINA_CODIGO_COLUMN_NAME;
	static public String TOTAL_CREDITOS_COLUMN_NAME;
	static public String DIA1_COLUMN_NAME;
	static public String DIA2_COLUMN_NAME;
	static public String DIA3_COLUMN_NAME;
	static public String DIA4_COLUMN_NAME;
	static public String DIA5_COLUMN_NAME;
	static public String DIA6_COLUMN_NAME;
	static public String DIA7_COLUMN_NAME;
	private List< String > headerColumnsNames;
	
	public DivisoesCreditoDisciplinaImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();
	
		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( DISCIPLINA_CODIGO_COLUMN_NAME );
		this.headerColumnsNames.add( TOTAL_CREDITOS_COLUMN_NAME );
		this.headerColumnsNames.add( DIA1_COLUMN_NAME );
		this.headerColumnsNames.add( DIA2_COLUMN_NAME );
		this.headerColumnsNames.add( DIA3_COLUMN_NAME );
		this.headerColumnsNames.add( DIA4_COLUMN_NAME );
		this.headerColumnsNames.add( DIA5_COLUMN_NAME );
		this.headerColumnsNames.add( DIA6_COLUMN_NAME );
		this.headerColumnsNames.add( DIA7_COLUMN_NAME );
	}
	
	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.DIVISOES_CREDITO_DISCIPLINA.getSheetName().equals( sheetName );
	}
	
	@Override
	protected List< String > getHeaderColumnsNames(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		return this.headerColumnsNames;
	}
	
	@Override
	protected DivisoesCreditoDisciplinaImportExcelBean createExcelBean(
		Row header, Row row, int sheetIndex,
		Sheet sheet, Workbook workbook )
	{
		DivisoesCreditoDisciplinaImportExcelBean bean
			= new DivisoesCreditoDisciplinaImportExcelBean( row.getRowNum() + 1 );
	
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
	
					if ( DISCIPLINA_CODIGO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDisciplinaCodigoStr( cellValue );
					}
					if ( TOTAL_CREDITOS_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setCreditoStr( cellValue );
					}
					else if ( DIA1_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDia1Str( cellValue );
					}
					else if ( DIA2_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDia2Str( cellValue );
					}
					else if ( DIA3_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDia3Str( cellValue );
					}
					else if ( DIA4_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDia4Str( cellValue );
					}
					else if ( DIA5_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDia5Str( cellValue );
					}
					else if ( DIA6_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDia6Str( cellValue );
					}
					else if ( DIA7_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDia7Str( cellValue );
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
		return ExcelInformationType.DIVISOES_CREDITO_DISCIPLINA.getSheetName();
	}
	
	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected void processSheetContent(
		String sheetName, List< DivisoesCreditoDisciplinaImportExcelBean > sheetContent )
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
		String sheetName, List< DivisoesCreditoDisciplinaImportExcelBean > sheetContent )
	{
		// Map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map< ImportExcelError, List< Integer > > syntacticErrorsMap
			= new HashMap< ImportExcelError, List< Integer > >();
	
		for ( DivisoesCreditoDisciplinaImportExcelBean bean : sheetContent )
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
		String sheetName, List< DivisoesCreditoDisciplinaImportExcelBean > sheetContent )
	{
		// Verifica se algum turno apareceu
		// mais de uma vez no arquivo de entrada
		checkUniqueness(sheetContent);
		
		// Verifica se as disciplinas estao cadastradas
		checkNonRegisteredDisciplina(sheetContent);
		
		// Verifica se a soma dos creditos de cada dia
		// equivale a quantidade total de creditos
		checkSomaCreditos(sheetContent);
		
		// Verifica se a soma dos creditos de cada dia
		// equivale a quantidade total de creditos da disciplina
		checkTotalCreditosDisciplina(sheetContent);
	
		return getErrors().isEmpty();
	}
	
	private void checkNonRegisteredDisciplina(
		List< DivisoesCreditoDisciplinaImportExcelBean > sheetContent )
	{
		// [ CodigoDisciplina -> Disciplina ]
		Map< String, Disciplina > disciplinaBDMap = Disciplina.buildDisciplinaCodigoToDisciplinaMap(
			Disciplina.findByCenario( this.instituicaoEnsino, getCenario() ) );

		if ( disciplinaBDMap == null || disciplinaBDMap.size() == 0 )
		{
			return;
		}

		List< Integer > rowsWithErrors = new ArrayList< Integer >();

		for ( DivisoesCreditoDisciplinaImportExcelBean bean : sheetContent )
		{
			Disciplina disciplina = disciplinaBDMap.get( bean.getDisciplinaCodigoStr() );

			if ( disciplina != null )
			{
				bean.setDisciplina(disciplina);
			}
			else
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				DISCIPLINA_CODIGO_COLUMN_NAME,rowsWithErrors.toString() ) );
		}
	}
	
	private void checkUniqueness(
		List< DivisoesCreditoDisciplinaImportExcelBean > sheetContent )
	{
		// Map com as divisoes de creditos e as
		// linhas em que a mesmo aparece no arquivo de entrada
		// [ CodigoTurno -> Lista de Linhas do Arquivo de Entrada ]
		Map< String, List< Integer > > divisaoCodigoToRowsMap
			= new HashMap< String, List< Integer > >();
	
		for ( DivisoesCreditoDisciplinaImportExcelBean bean : sheetContent )
		{
			List< Integer > rows
				= divisaoCodigoToRowsMap.get( bean.getDivisaoKey() );
	
			if ( rows == null )
			{
				rows = new ArrayList< Integer >();
				divisaoCodigoToRowsMap.put(
					bean.getDivisaoKey(), rows );
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
	
	private void checkSomaCreditos(
		List< DivisoesCreditoDisciplinaImportExcelBean > sheetContent )
	{
		
		List<Integer> rowsWithError = new ArrayList<Integer>();
		for ( DivisoesCreditoDisciplinaImportExcelBean bean : sheetContent )
		{
			int somaCreditos = bean.getDia1() + bean.getDia2() + bean.getDia3()
					+ bean.getDia4() + bean.getDia5() + bean.getDia6()
					+ bean.getDia7();
			if (bean.getCreditos() != somaCreditos)
			{
				rowsWithError.add(bean.getRow());
			}
		}
	
		for ( Integer row : rowsWithError )
		{
			getErrors().add( getI18nMessages().excelErroLogicoRegraCreditoInvalida(
				row.toString() ) );
		}
	}
	
	private void checkTotalCreditosDisciplina(
		List< DivisoesCreditoDisciplinaImportExcelBean > sheetContent )
	{
		// [ CodigoDisciplina -> Disciplina ]
		Map< String, Disciplina > disciplinaBDMap = Disciplina.buildDisciplinaCodigoToDisciplinaMap(
			Disciplina.findByCenario( this.instituicaoEnsino, getCenario() ) );	
		List<Integer> rowsWithError = new ArrayList<Integer>();
		for ( DivisoesCreditoDisciplinaImportExcelBean bean : sheetContent )
		{
			if (disciplinaBDMap.get(bean.getDisciplinaCodigoStr()) != null &&
					bean.getCreditos() != disciplinaBDMap.get(bean.getDisciplinaCodigoStr()).getTotalCreditos())
			{
				rowsWithError.add(bean.getRow());
			}
		}
	
		for ( Integer row : rowsWithError )
		{
			getErrors().add( getI18nMessages().excelErroLogicoTotalCreditosDiferenteTotalDisciplina(
				row.toString() ) );
		}
	}
	
	@Transactional
	private void updateDataBase( String sheetName,
		List< DivisoesCreditoDisciplinaImportExcelBean > sheetContent )
	{
		Map< String, DivisaoCredito > divisoesBDMap = DivisaoCredito.buildDivisaoCreditoKeyToDivisaoCreditoMap(
				DivisaoCredito.findWithDisciplina(getCenario(), null, null, instituicaoEnsino) );
		
		for ( DivisoesCreditoDisciplinaImportExcelBean divisaoExcel : sheetContent )
		{
			DivisaoCredito divisaoBD = divisoesBDMap.get( divisaoExcel.getDivisaoKey() );
			if ( divisaoBD != null )
			{
				// Update
				divisaoBD.setCreditos( divisaoExcel.getCreditos() );
				divisaoBD.setDia1( divisaoExcel.getDia1() );
				divisaoBD.setDia2( divisaoExcel.getDia2() );
				divisaoBD.setDia3( divisaoExcel.getDia3() );
				divisaoBD.setDia4( divisaoExcel.getDia4() );
				divisaoBD.setDia5( divisaoExcel.getDia5() );
				divisaoBD.setDia6( divisaoExcel.getDia6() );
				divisaoBD.setDia7( divisaoExcel.getDia6() );
	
				divisaoBD.merge();
			}
			else
			{
				// Insert
				DivisaoCredito newDivisao = new DivisaoCredito();
				newDivisao.setInstituicaoEnsino( instituicaoEnsino );
	
				newDivisao.setCreditos( divisaoExcel.getCreditos() );
				newDivisao.setDia1( divisaoExcel.getDia1() );
				newDivisao.setDia2( divisaoExcel.getDia2() );
				newDivisao.setDia3( divisaoExcel.getDia3() );
				newDivisao.setDia4( divisaoExcel.getDia4() );
				newDivisao.setDia5( divisaoExcel.getDia5() );
				newDivisao.setDia6( divisaoExcel.getDia6() );
				newDivisao.setDia7( divisaoExcel.getDia6() );
	
				newDivisao.setDisciplina(divisaoExcel.getDisciplina());
				newDivisao.persist();
				
				divisaoExcel.getDisciplina().setDivisaoCreditos(newDivisao);
			}
		}
	}
	
	private void resolveHeaderColumnNames()
	{
		if ( DISCIPLINA_CODIGO_COLUMN_NAME == null )
		{
			DISCIPLINA_CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoDisciplina() );
			TOTAL_CREDITOS_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().totalDecreditos() );
			DIA1_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().dia() + " 1" );
			DIA2_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().dia() + " 2" );
			DIA3_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().dia() + " 3" );
			DIA4_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().dia() + " 4" );
			DIA5_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().dia() + " 5" );
			DIA6_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().dia() + " 6" );
			DIA7_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().dia() + " 7" );
		}
	}
}