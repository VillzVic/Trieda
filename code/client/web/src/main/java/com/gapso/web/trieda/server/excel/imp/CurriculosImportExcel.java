package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class CurriculosImportExcel
	extends AbstractImportExcel< CurriculosImportExcelBean >
{
	static public String CURSO_COLUMN_NAME;
	static public String CODIGO_COLUMN_NAME;
	static public String DESCRICAO_COLUMN_NAME;
	static public String DISCIPLINA_COLUMN_NAME;
	static public String PERIODO_COLUMN_NAME;
	static public String SEMANA_LETIVA_COLUMN_NAME;

	private List< String > headerColumnsNames;

	public CurriculosImportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( CURSO_COLUMN_NAME );
		this.headerColumnsNames.add( CODIGO_COLUMN_NAME );
		this.headerColumnsNames.add( DESCRICAO_COLUMN_NAME );
		this.headerColumnsNames.add( PERIODO_COLUMN_NAME );
		this.headerColumnsNames.add( DISCIPLINA_COLUMN_NAME );
		this.headerColumnsNames.add( SEMANA_LETIVA_COLUMN_NAME );
	}

	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.CURRICULOS.getSheetName().equals( sheetName );
	}

	@Override
	protected List< String > getHeaderColumnsNames(
		int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook )
	{
		return this.headerColumnsNames;
	}

	@Override
	protected CurriculosImportExcelBean createExcelBean(
		HSSFRow header, HSSFRow row, int sheetIndex,
		HSSFSheet sheet, HSSFWorkbook workbook )
	{
		CurriculosImportExcelBean bean
			= new CurriculosImportExcelBean( row.getRowNum() + 1 );

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

					if ( CURSO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setCursoCodigoStr( cellValue );
					}
					else if ( CODIGO_COLUMN_NAME.equals( columnName ) )
					{
						bean.setCodigoStr( cellValue );
					}
					else if ( DESCRICAO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDescricaoStr( cellValue );
					}
					else if ( DISCIPLINA_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDisciplinaCodigoStr( cellValue );
					}
					else if ( PERIODO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setPeriodoStr( cellValue );
					}
					else if ( SEMANA_LETIVA_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setSemanaLetivaCodigoStr( cellValue );
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
		return ExcelInformationType.CURRICULOS.getSheetName();
	}

	@Override
	protected void processSheetContent(
		String sheetName, List< CurriculosImportExcelBean > sheetContent )
	{
		if ( doSyntacticValidation( sheetName, sheetContent )
			&& doLogicValidation( sheetName, sheetContent ) )
		{
			updateDataBase( sheetName, sheetContent );
		}
	}

	private boolean doSyntacticValidation(
		String sheetName, List< CurriculosImportExcelBean > sheetContent )
	{
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map< ImportExcelError, List< Integer > > syntacticErrorsMap
			= new HashMap< ImportExcelError, List< Integer > >();

		for ( CurriculosImportExcelBean bean : sheetContent )
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
		String sheetName, List< CurriculosImportExcelBean > sheetContent )
	{
		// Verifica se alguma matriz curricular
		// apareceu mais de uma vez no arquivo de entrada
		checkUniquenessCurriculo( sheetContent );

		// Verifica se alguma disciplina
		// apareceu mais de uma vez para um par
		// ( Curriculo, Período ) no arquivo de entrada
		checkUniquenessDisciplina( sheetContent );

		// Verifica se há referência a algum curso não cadastrado
		checkNonRegisteredCurso( sheetContent );

		// Verifica se há referência a alguma disciplina não cadastrada
		checkNonRegisteredDisciplina( sheetContent );

		// Verifica se há referência a alguma semana letiva não cadastrada
		checkNonRegisteredSemanaLetiva( sheetContent );

		return getErrors().isEmpty();
	}

	private void checkUniquenessCurriculo(
		List< CurriculosImportExcelBean > sheetContent )
	{
		checkUniquenessCurriculoByCurso( sheetContent );
		checkUniquenessCurriculoByDescricao( sheetContent );
	}

	private void checkUniquenessCurriculoByCurso(
		List< CurriculosImportExcelBean > sheetContent )
	{
		// Map com o CodCurriculo e o conjunto de
		// pares ( CodCurriculo - CodCurso ) em que o mesmo aparece

		// [ CodCurriculo -> Conjunto de pares ( CodCurriculo - CodCurso ) ]
		Map< String, Set< String > > curriculoToParesCurriculoCursoMap
			= new HashMap< String, Set< String > >();

		// Map com o par ( CodCurriculo, CodCurso ) e as
		// linhas em que o mesmo aparece no arquivo de entrada

		// [ CodCurriculo - CodCurso -> Lista de Linhas do Arquivo de Entrada ]
		Map< String, List< Integer > > parCurriculoCursoToRowsMap
			= new HashMap< String, List< Integer > >();

		// Preenche os maps
		for ( CurriculosImportExcelBean bean : sheetContent )
		{
			// Preenche o map parCurriculoCursoToRowsMap
			String par = bean.getCodigoStr() + "-" + bean.getCursoCodigoStr();
			List< Integer > rows = parCurriculoCursoToRowsMap.get( par );

			if ( rows == null )
			{
				rows = new ArrayList< Integer >();
				parCurriculoCursoToRowsMap.put( par, rows );
			}

			rows.add( bean.getRow() );

			// Preenche o map curriculoToParesCurriculoCursoMap
			Set< String > pares = curriculoToParesCurriculoCursoMap.get(
				bean.getCodigoStr() );

			if ( pares == null )
			{
				pares = new HashSet< String >();
				curriculoToParesCurriculoCursoMap.put(
					bean.getCodigoStr(), pares );
			}

			pares.add( par );
		}

		// Verifica se alguma matriz curricular
		// apareceu mais de uma vez no arquivo de entrada
		for ( Entry< String, Set< String > > entry
			: curriculoToParesCurriculoCursoMap.entrySet() )
		{
			String codCurriculo = entry.getKey();
			Set< String > pares = entry.getValue();

			// Verifica se para um curriculo
			// existe mais de um par ( CodCurriculo, CodCurso )

			if ( pares.size() > 1 )
			{
				// Para cada par ( CodCurriculo, CodCurso ), coleta
				// as linhas em que os mesmos aparecem no arquivo de entrada
				List< Integer > allRows = new ArrayList< Integer >();

				for ( String par : pares )
				{
					List< Integer > rows
						= parCurriculoCursoToRowsMap.get( par );

					if ( rows != null )
					{
						allRows.addAll( rows );
					}
				}

				Collections.sort( allRows );

				getErrors().add( getI18nMessages().excelErroLogicoUnicidadeVioladaCurriculoPorCurso(
					codCurriculo, allRows.toString() ) );
			}
		}
	}

	private void checkUniquenessCurriculoByDescricao(
		List< CurriculosImportExcelBean > sheetContent )
	{
		// Map com o CodCurriculo e o conjunto de
		// pares ( CodCurriculo - Descricao ) em que o mesmo aparece

		// [ CodCurriculo -> Conjunto de pares ( CodCurriculo - Descricao ) ]
		Map< String, Set< String > > curriculoToParesCurriculoDescricaoMap
			= new HashMap< String, Set< String > >();

		// Map com o par ( CodCurriculo, Descricao ) e as
		// linhas em que o mesmo aparece no arquivo de entrada

		// [ CodCurriculo - Descricao -> Lista de Linhas do Arquivo de Entrada ]
		Map< String, List< Integer > > parCurriculoDescricaoToRowsMap
			= new HashMap< String, List< Integer > >();

		// Preenche os maps
		for ( CurriculosImportExcelBean bean : sheetContent )
		{
			// Preenche o map parCurriculoDescricaoToRowsMap
			String par = bean.getCodigoStr() + "-" + bean.getDescricaoStr();
			List< Integer > rows = parCurriculoDescricaoToRowsMap.get( par );

			if ( rows == null )
			{
				rows = new ArrayList< Integer >();
				parCurriculoDescricaoToRowsMap.put( par, rows );
			}

			rows.add( bean.getRow() );

			// Preenche o map curriculoToParesCurriculoDescricaoMap
			Set< String > pares = curriculoToParesCurriculoDescricaoMap.get(
				bean.getCodigoStr() );

			if ( pares == null )
			{
				pares = new HashSet< String >();
				curriculoToParesCurriculoDescricaoMap.put(
					bean.getCodigoStr(), pares );
			}

			pares.add( par );
		}

		// Verifica se alguma matriz curricular
		// apareceu mais de uma vez no arquivo de entrada
		for ( Entry< String, Set< String > > entry
			: curriculoToParesCurriculoDescricaoMap.entrySet() )
		{
			String codCurriculo = entry.getKey();
			Set< String > pares = entry.getValue();

			// Verifica se para um curriculo
			// existe mais de um par ( CodCurriculo, Descricao )
			if ( pares.size() > 1 )
			{
				// Para cada par ( CodCurriculo, Descricao ), coleta
				// as linhas em que os mesmos aparecem no arquivo de entrada
				List< Integer > allRows = new ArrayList< Integer >();

				for ( String par : pares )
				{
					List< Integer > rows = parCurriculoDescricaoToRowsMap.get( par );

					if ( rows != null )
					{
						allRows.addAll( rows );
					}
				}

				Collections.sort( allRows );

				getErrors().add( getI18nMessages().excelErroLogicoUnicidadeVioladaCurriculoPorDescricao(
					codCurriculo, allRows.toString() ) );
			}
		}
	}

	private void checkUniquenessDisciplina(
		List< CurriculosImportExcelBean > sheetContent )
	{
		// Codigo referente á issue http://jira.gapso.com.br/browse/TRIEDA-791

		// Map com o trio ( CodCurriculo, Periodo, CodDisciplina )
		// e os beans em que tal trio aparece no arquivo de entrada

		// [ CodCurriculo - Periodo - CodDisciplina -> Lista de Beans ]
		Map< String, List< CurriculosImportExcelBean > > trioToBeansMap
			= new HashMap< String, List< CurriculosImportExcelBean > >();

		for ( CurriculosImportExcelBean bean : sheetContent )
		{
			String key = bean.getCodigoStr()
				+ "-" + bean.getPeriodoStr()
				+ "-" + bean.getDisciplinaCodigoStr();

			List< CurriculosImportExcelBean > beans
				= trioToBeansMap.get( key );

			if ( beans == null )
			{
				beans = new ArrayList< CurriculosImportExcelBean >();
				trioToBeansMap.put( key, beans );
			}

			beans.add( bean );
		}

		// Verifica se algum trio ( CodCurriculo, Periodo, CodDisciplina )
		// apareceu mais de uma vez no arquivo de entrada
		for ( Entry< String, List< CurriculosImportExcelBean > > entry : trioToBeansMap.entrySet() )
		{
			if ( entry.getValue().size() > 1 )
			{
				CurriculosImportExcelBean firstBean = entry.getValue().get( 0 );
				List< Integer > rows = new ArrayList< Integer >();

				for ( CurriculosImportExcelBean bean : entry.getValue() )
				{
					rows.add( bean.getRow() );
				}

				Collections.sort( rows );

				getErrors().add( getI18nMessages().excelErroLogicoUnicidadeVioladaDisciplinaCurriculo(
					firstBean.getDisciplinaCodigoStr(), firstBean.getPeriodo().toString(),
					firstBean.getCodigoStr(), rows.toString() ) );
			}
		}
	}

	private void checkNonRegisteredCurso(
		List< CurriculosImportExcelBean > sheetContent )
	{
		// [ CodigoCurso -> Curso ]
		Map< String, Curso > cursosBDMap = Curso.buildCursoCodigoToCursoMap(
			Curso.findByCenario( this.instituicaoEnsino, getCenario() ) );

		if ( cursosBDMap == null || cursosBDMap.size() == 0 )
		{
			return;
		}

		List< Integer > rowsWithErrors = new ArrayList< Integer >();

		for ( CurriculosImportExcelBean bean : sheetContent )
		{
			Curso curso = cursosBDMap.get( bean.getCursoCodigoStr() );

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
				CURSO_COLUMN_NAME,rowsWithErrors.toString() ) );
		}
	}

	private void checkNonRegisteredDisciplina(
		List< CurriculosImportExcelBean > sheetContent )
	{
		// Codigo referente á issue http://jira.gapso.com.br/browse/TRIEDA-791

		// [ CodigoDisciplina -> Disciplina ]
		Map< String, Disciplina > disciplinasBDMap
			= Disciplina.buildDisciplinaCodigoToDisciplinaMap(
				Disciplina.findByCenario( this.instituicaoEnsino, getCenario() ) );

		List< Integer > rowsWithErrors
			= new ArrayList< Integer >();

		for ( CurriculosImportExcelBean bean : sheetContent )
		{
			Disciplina disciplina = disciplinasBDMap.get( bean.getDisciplinaCodigoStr() );
			if ( disciplina != null )
			{
				bean.setDisciplina( disciplina );
			}
			else
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				DISCIPLINA_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}

	private void checkNonRegisteredSemanaLetiva(
		List< CurriculosImportExcelBean > sheetContent )
	{
		// [ CodigoSemanaLetiva -> Semana Letiva ]
		Map< String, SemanaLetiva > semanasLetivasBDMap
			= SemanaLetiva.buildSemanaLetivaCodigoToSemanaLetivaMap(
				SemanaLetiva.findAll( this.instituicaoEnsino ) );

		List< Integer > rowsWithErrors
			= new ArrayList< Integer >();

		for ( CurriculosImportExcelBean bean : sheetContent )
		{
			SemanaLetiva semanaLetiva
				= semanasLetivasBDMap.get( bean.getSemanaLetivaCodigoStr() );

			if ( semanaLetiva != null )
			{
				bean.setSemanaLetiva( semanaLetiva );
			}
			else
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				SEMANA_LETIVA_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}

	@Transactional
	private void updateDataBase( String sheetName,
		List< CurriculosImportExcelBean > sheetContent )
	{
		// ATUALIZA CURRICULOS

		// [ CodCurriculo -> Curriculo ]
		Map< String, Curriculo > curriculosBDMap = Curriculo.buildCurriculoCodigoToCurriculoMap(
			Curriculo.findByCenario( this.instituicaoEnsino, getCenario() ) );

		// [ CodCurriculo -> CurriculosImportExcelBean ]
		Map< String, CurriculosImportExcelBean > curriculosExcelMap
			= CurriculosImportExcelBean.buildCurriculoCodigoToImportExcelBeanMap( sheetContent ); 

		for ( String codigoCurriculo : curriculosExcelMap.keySet() )
		{
			Curriculo curriculoBD = curriculosBDMap.get( codigoCurriculo );

			CurriculosImportExcelBean curriculoExcel
				= curriculosExcelMap.get( codigoCurriculo );

			if ( curriculoBD != null )
			{
				// Update
				curriculoBD.setDescricao( curriculoExcel.getDescricaoStr() );
				curriculoBD.setCurso( curriculoExcel.getCurso() );

				curriculoBD.merge();
			}
			else
			{
				// Insert
				Curriculo newCurriculo = new Curriculo();

				newCurriculo.setCenario( getCenario() );
				newCurriculo.setCodigo( curriculoExcel.getCodigoStr() );
				newCurriculo.setDescricao( curriculoExcel.getDescricaoStr() );
				newCurriculo.setCurso( curriculoExcel.getCurso() );
				newCurriculo.setSemanaLetiva( curriculoExcel.getSemanaLetiva() );

				newCurriculo.persist();

				Curriculo.entityManager().refresh( newCurriculo );
				curriculosBDMap.put( newCurriculo.getCodigo(), newCurriculo );
			}
		}

		// ATUALIZA CURRICULOS-DISCIPLINAS
		// Codigo referente á issue http://jira.gapso.com.br/browse/TRIEDA-791

		// [ CodCurso - CodCurriculo - Periodo - CodDisciplina -> CurriculoDisciplina ]
		Map< String, CurriculoDisciplina > curriculosDisciplinasBDMap
			= CurriculoDisciplina.buildNaturalKeyToCurriculoDisciplinaMap(
				CurriculoDisciplina.findByCenario( this.instituicaoEnsino, getCenario() ) );

		for ( CurriculosImportExcelBean curriculoExcel : sheetContent )
		{
			CurriculoDisciplina curriculoDisciplinaBD
				= curriculosDisciplinasBDMap.get( curriculoExcel.getNaturalKeyString() );

			if ( curriculoDisciplinaBD == null )
			{
				// Insert
				CurriculoDisciplina newCurriculoDisciplina = new CurriculoDisciplina();

				newCurriculoDisciplina.setPeriodo( curriculoExcel.getPeriodo() );
				newCurriculoDisciplina.setDisciplina( curriculoExcel.getDisciplina() );
				newCurriculoDisciplina.setCurriculo( curriculosBDMap.get( curriculoExcel.getCodigoStr() ) );

				if ( newCurriculoDisciplina.getPeriodo() != null
					&& newCurriculoDisciplina.getDisciplina() != null
					&& newCurriculoDisciplina.getCurriculo() != null )
				{
					newCurriculoDisciplina.persist();

					curriculosDisciplinasBDMap.put(
						curriculoExcel.getNaturalKeyString(), newCurriculoDisciplina );
				}
			}
		}
	}

	private void resolveHeaderColumnNames()
	{
		if ( CODIGO_COLUMN_NAME == null )
		{
			CURSO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoCurso() );
			CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoCurriculo() );
			DESCRICAO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().descricaoCurriculo() );
			PERIODO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().periodo() );
			DISCIPLINA_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().disciplina() );
			SEMANA_LETIVA_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().semanaLetiva() );
		}
	}
}
