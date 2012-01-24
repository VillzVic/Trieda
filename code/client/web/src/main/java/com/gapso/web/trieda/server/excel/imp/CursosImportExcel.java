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
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class CursosImportExcel
	extends AbstractImportExcel< CursosImportExcelBean >
{
	static public String CODIGO_COLUMN_NAME;
	static public String NOME_COLUMN_NAME;
	static public String TIPO_COLUMN_NAME;
	static public String MIN_DOUTOR_COLUMN_NAME;
	static public String MIN_MESTRE_COLUMN_NAME;
	static public String MIN_TEMPO_INTEGRAL_PARCIAL_COLUMN_NAME;
	static public String MIN_TEMPO_INTEGRAL_COLUMN_NAME;
	static public String MAX_DISC_PROF_COLUMN_NAME;
	static public String MAIS_DE_UMA_DISC_PROF_COLUMN_NAME;

	private List< String > headerColumnsNames;

	public CursosImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( CODIGO_COLUMN_NAME );
		this.headerColumnsNames.add( NOME_COLUMN_NAME );
		this.headerColumnsNames.add( TIPO_COLUMN_NAME );
		this.headerColumnsNames.add( MIN_DOUTOR_COLUMN_NAME );
		this.headerColumnsNames.add( MIN_MESTRE_COLUMN_NAME );
		this.headerColumnsNames.add( MIN_TEMPO_INTEGRAL_PARCIAL_COLUMN_NAME );
		this.headerColumnsNames.add( MIN_TEMPO_INTEGRAL_COLUMN_NAME );
		this.headerColumnsNames.add( MAX_DISC_PROF_COLUMN_NAME );
		this.headerColumnsNames.add( MAIS_DE_UMA_DISC_PROF_COLUMN_NAME );
	}

	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.CURSOS.getSheetName().equals( sheetName );
	}

	@Override
	protected List<String> getHeaderColumnsNames(
		int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook )
	{
		return this.headerColumnsNames;
	}

	@Override
	protected CursosImportExcelBean createExcelBean(
		HSSFRow header, HSSFRow row, int sheetIndex,
		HSSFSheet sheet, HSSFWorkbook workbook )
	{
		CursosImportExcelBean bean = new CursosImportExcelBean( row.getRowNum() + 1 );

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

					if ( CODIGO_COLUMN_NAME.equals( columnName ) )
					{
						bean.setCodigoStr( cellValue );
					} else if ( NOME_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setNomeStr( cellValue );
					}
					else if ( TIPO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setTipoStr( cellValue );
					}
					else if ( MIN_DOUTOR_COLUMN_NAME.equals( columnName ) )
					{
						bean.setMinDoutorPrecentStr( cellValue );
					}
					else if ( MIN_MESTRE_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setMinMestrePrecentStr( cellValue );
					}
					else if ( MIN_TEMPO_INTEGRAL_PARCIAL_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setMinTempoIntegralParcialPrecentStr( cellValue );
					}
					else if ( MIN_TEMPO_INTEGRAL_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setMinTempoIntegralPrecentStr( cellValue );
					}
					else if ( MAX_DISC_PROF_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setMaxDisciplinasProfessorStr( cellValue );
					}
					else if ( MAIS_DE_UMA_DISC_PROF_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setMaisDeUmaDisciplinaProfessorStr( cellValue );
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
		return ExcelInformationType.CURSOS.getSheetName();
	}

	@Override
	protected void processSheetContent( String sheetName,
		List< CursosImportExcelBean > sheetContent )
	{
		if ( doSyntacticValidation( sheetName, sheetContent )
			&& doLogicValidation( sheetName, sheetContent ) )
		{
			updateDataBase( sheetName, sheetContent );
		}
	}

	private boolean doSyntacticValidation( String sheetName,
		List< CursosImportExcelBean > sheetContent )
	{
		// Map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map< ImportExcelError, List< Integer > > syntacticErrorsMap
			= new HashMap< ImportExcelError, List< Integer > >();

		for ( CursosImportExcelBean bean : sheetContent )
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

		// Coleta os erros e adiciona os mesmos na lista de mensagens
		for ( ImportExcelError error : syntacticErrorsMap.keySet() )
		{
			List< Integer > linhasComErro = syntacticErrorsMap.get( error );
			getErrors().add( error.getMessage( linhasComErro.toString(), getI18nMessages() ) );
		}

		return syntacticErrorsMap.isEmpty();
	}

	private boolean doLogicValidation( String sheetName,
		List< CursosImportExcelBean > sheetContent )
	{
		// Verifica se algum curso apareceu mais de uma vez no arquivo de entrada
		checkUniqueness( sheetContent );

		// Verifica se há referência a algum tipo de curso não cadastrado
		checkNonRegisteredTipoCurso( sheetContent );
		
		return getErrors().isEmpty();
	}

	private void checkUniqueness( List< CursosImportExcelBean > sheetContent )
	{
		// Map com os códigos dos cursos e as linhas em que o mesmo aparece no arquivo de entrada
		// [ CódigoCurso -> Lista de Linhas do Arquivo de Entrada ]
		Map< String, List< Integer > > cursoCodigoToRowsMap
			= new HashMap< String, List< Integer > >();

		for ( CursosImportExcelBean bean : sheetContent )
		{
			List< Integer > rows = cursoCodigoToRowsMap.get( bean.getCodigoStr() );
			if ( rows == null )
			{
				rows = new ArrayList< Integer >();
				cursoCodigoToRowsMap.put( bean.getCodigoStr(), rows );
			}

			rows.add( bean.getRow() );
		}

		// Verifica se algum curso apareceu mais de uma vez no arquivo de entrada
		for ( Entry< String, List< Integer > > entry : cursoCodigoToRowsMap.entrySet() )
		{
			if ( entry.getValue().size() > 1 )
			{
				getErrors().add( getI18nMessages().excelErroLogicoUnicidadeViolada(
					entry.getKey(), entry.getValue().toString() ) );
			}
		}
	}

	private void checkNonRegisteredTipoCurso(
		List< CursosImportExcelBean > sheetContent )
	{
		InstituicaoEnsino instituicaoEnsino = this.getCenario().getInstituicaoEnsino();

		// [ CódigoTipoCurso -> TipoCurso ]
		Map< String, TipoCurso > tiposCursoBDMap
			= TipoCurso.buildTipoCursoCodigoToTipoCursoMap(
				TipoCurso.findAll( instituicaoEnsino ) );

		List< Integer > rowsWithErrors = new ArrayList< Integer >();
		for ( CursosImportExcelBean bean : sheetContent )
		{
			TipoCurso tipoCurso = tiposCursoBDMap.get( bean.getTipoStr() );

			if ( tipoCurso != null )
			{
				bean.setTipo( tipoCurso );
			}
			else
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}
		
		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				TIPO_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}

	@Transactional
	private void updateDataBase( String sheetName,
		List< CursosImportExcelBean > sheetContent )
	{
		Map< String, Curso > cursosBDMap = Curso.buildCursoCodigoToCursoMap(
			Curso.findByCenario( this.instituicaoEnsino, getCenario() ) );

		int count = 0, total=sheetContent.size(); System.out.print(" "+total);
		for ( CursosImportExcelBean cursoExcel : sheetContent )
		{
			Curso cursoBD = cursosBDMap.get( cursoExcel.getCodigoStr() );
			if ( cursoBD != null )
			{
				// Update
				cursoBD.setNome( cursoExcel.getNomeStr() );
				cursoBD.setNumMinDoutores( cursoExcel.getMinDoutorPrecent() );
				cursoBD.setNumMinMestres( cursoExcel.getMinMestrePrecent() );
				cursoBD.setMinTempoIntegralParcial( cursoExcel.getMinTempoIntegralParcialPrecent() );
				cursoBD.setMinTempoIntegral( cursoExcel.getMinTempoIntegralPrecent() );
				cursoBD.setMaxDisciplinasPeloProfessor( cursoExcel.getMaxDisciplinasProfessor() );
				cursoBD.setAdmMaisDeUmDisciplina( cursoExcel.getMaisDeUmaDisciplinaProfessor() );
				cursoBD.setTipoCurso( cursoExcel.getTipo() );

				cursoBD.merge();
			}
			else
			{
				// Insert
				Curso newCurso = new Curso();

				newCurso.setCenario( getCenario() );
				newCurso.setCodigo( cursoExcel.getCodigoStr() );
				newCurso.setNome( cursoExcel.getNomeStr() );
				newCurso.setNumMinDoutores( cursoExcel.getMinDoutorPrecent() );
				newCurso.setNumMinMestres( cursoExcel.getMinMestrePrecent() );
				newCurso.setMinTempoIntegralParcial( cursoExcel.getMinTempoIntegralParcialPrecent() );
				newCurso.setMinTempoIntegral( cursoExcel.getMinTempoIntegralPrecent() );
				newCurso.setMaxDisciplinasPeloProfessor( cursoExcel.getMaxDisciplinasProfessor() );
				newCurso.setAdmMaisDeUmDisciplina( cursoExcel.getMaisDeUmaDisciplinaProfessor() );
				newCurso.setTipoCurso( cursoExcel.getTipo() );
				
				newCurso.persist();
			}
			
			count++;total--;if (count == 100) {System.out.println("   Faltam "+total+" cursos"); count = 0;}
		}
	}
	
	private void resolveHeaderColumnNames()
	{
		if ( CODIGO_COLUMN_NAME == null )
		{
			CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoCurso() );
			NOME_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().nomeCurso() );
			TIPO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().tipoCurso() );
			MIN_DOUTOR_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().minPercentualDoutor() );
			MIN_MESTRE_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().minPercentualMestre() );
			MIN_TEMPO_INTEGRAL_PARCIAL_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().minTempoIntegralParcial() );
			MIN_TEMPO_INTEGRAL_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().minTempoIntegral() );
			MAX_DISC_PROF_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().maxDisciplinasProfessor() );
			MAIS_DE_UMA_DISC_PROF_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().maisDeUmaDisciplinaProfessor() );			
		}
	}
}
