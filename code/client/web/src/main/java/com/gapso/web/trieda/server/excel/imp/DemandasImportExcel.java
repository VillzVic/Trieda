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

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class DemandasImportExcel
	extends AbstractImportExcel< DemandasImportExcelBean >
{
	static public String CAMPUS_COLUMN_NAME;
	static public String TURNO_COLUMN_NAME;
	static public String CURSO_COLUMN_NAME;
	static public String MATRIZ_CURRICULAR_COLUMN_NAME;
	static public String PERIODO_COLUMN_NAME;
	static public String DISCIPLINA_COLUMN_NAME;
	static public String DEMANDA_COLUMN_NAME;

	private List< String > headerColumnsNames;

	public DemandasImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages )
	{
		super( cenario, i18nConstants, i18nMessages );
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( CAMPUS_COLUMN_NAME );
		this.headerColumnsNames.add( TURNO_COLUMN_NAME );
		this.headerColumnsNames.add( CURSO_COLUMN_NAME );
		this.headerColumnsNames.add( MATRIZ_CURRICULAR_COLUMN_NAME );
		this.headerColumnsNames.add( PERIODO_COLUMN_NAME );
		this.headerColumnsNames.add( DISCIPLINA_COLUMN_NAME );
		this.headerColumnsNames.add( DEMANDA_COLUMN_NAME );
	}

	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.DEMANDAS.getSheetName().equals( sheetName );
	}

	@Override
	protected List< String > getHeaderColumnsNames(
		int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook )
	{
		return this.headerColumnsNames;
	}

	@Override
	protected DemandasImportExcelBean createExcelBean(
		HSSFRow header, HSSFRow row, int sheetIndex,
		HSSFSheet sheet, HSSFWorkbook workbook )
	{
		DemandasImportExcelBean bean = new DemandasImportExcelBean( row.getRowNum() + 1 );

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

					if ( CAMPUS_COLUMN_NAME.equals( columnName ) )
					{
						bean.setCampusStr( cellValue );
					}
					else if ( TURNO_COLUMN_NAME.equals( columnName ) )
					{
						bean.setTurnoStr( cellValue );
					}
					else if ( CURSO_COLUMN_NAME.equals( columnName ) )
					{
						bean.setCursoStr( cellValue );
					}
					else if ( MATRIZ_CURRICULAR_COLUMN_NAME.equals( columnName ) )
					{
						bean.setMatrizCurricularStr( cellValue );
					}
					else if ( PERIODO_COLUMN_NAME.equals( columnName ) )
					{
						bean.setPeriodoStr( cellValue );
					}
					else if ( DISCIPLINA_COLUMN_NAME.equals( columnName ) )
					{
						bean.setDisciplinaStr( cellValue );
					}
					else if ( DEMANDA_COLUMN_NAME.equals( columnName ) )
					{
						bean.setDemandaStr( cellValue );
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
		return ExcelInformationType.DEMANDAS.getSheetName();
	}

	@Override
	protected void processSheetContent( String sheetName,
		List< DemandasImportExcelBean > sheetContent )
	{
		if ( doSyntacticValidation( sheetName, sheetContent )
			&& doLogicValidation( sheetName, sheetContent ) )
		{
			updateDataBase( sheetName, sheetContent );
		}
	}

	private boolean doSyntacticValidation( String sheetName,
		List< DemandasImportExcelBean > sheetContent )
	{
		// Map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map< ImportExcelError, List< Integer > > syntacticErrorsMap
			= new HashMap< ImportExcelError, List< Integer > >();

		for ( DemandasImportExcelBean bean : sheetContent )
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

				rowsWithErrors.add(bean.getRow() );
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
		List< DemandasImportExcelBean > sheetContent )
	{
		// Verifica se uma demanda ( Campus + Turno + Matriz Curricular +
		// Disciplina ) é adicionada mais de uma vez no arquivo de entrada
		checkUniqueness( sheetContent );

		// Verifica se há referência a algum tipo de contrato não cadastrado
		checkNonRegisteredCampus( sheetContent );
		checkNonRegisteredTurno( sheetContent );
		checkNonRegisteredCurso( sheetContent );
		checkNonRegisteredDisciplina( sheetContent );
		checkNonRegisteredCurriculo( sheetContent );
		checkNonRegisteredDisciplinaEmCurricular( sheetContent );

		return getErrors().isEmpty();
	}

	private void checkUniqueness( List< DemandasImportExcelBean > sheetContent )
	{
		// Map com os códigos " Campus + Turno + Matriz Curricular + Período +
		// Disciplina" e as linhas em que o mesmo aparece no arquivo de entrada

		// [ "Campus + Turno + Matriz Curricular + Período +
		// Disciplina" -> Lista de Linhas do Arquivo de Entrada ]
		Map< String, List< Integer > > demandasToRowsMap
			= new HashMap< String, List< Integer > >();

		for ( DemandasImportExcelBean bean : sheetContent )
		{
			List< Integer > rows = demandasToRowsMap.get( getCodeDemanda( bean ) );
			if ( rows == null )
			{
				rows = new ArrayList< Integer >();
				demandasToRowsMap.put( getCodeDemanda( bean ), rows );
			}

			rows.add( bean.getRow() );
		}

		// Verifica se algum professor apareceu mais de uma vez no arquivo de entrada
		for ( Entry< String, List< Integer > > entry : demandasToRowsMap.entrySet() )
		{
			if ( entry.getValue().size() > 1 )
			{
				getErrors().add( getI18nMessages().excelErroLogicoUnicidadeViolada(
					entry.getKey(), entry.getValue().toString() ) );
			}
		}
	}

	private void checkNonRegisteredCampus(
		List< DemandasImportExcelBean > sheetContent )
	{
		// [ CódidoCampus -> Campus ]
		Map< String, Campus > campiBDMap = Campus.buildCampusCodigoToCampusMap(
			Campus.findByCenario( getCenario() ) );

		List< Integer > rowsWithErrors = new ArrayList< Integer >();
		for ( DemandasImportExcelBean bean : sheetContent )
		{
			Campus campus = campiBDMap.get( bean.getCampusStr() );
			if ( campus != null )
			{
				bean.setCampus( campus );
			}
			else
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				CAMPUS_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}

	private void checkNonRegisteredTurno(
		List< DemandasImportExcelBean > sheetContent )
	{
		// [ NomeTurno -> Turno ]
		Map< String, Turno > turnosBDMap = Turno.buildTurnoNomeToTurnoMap(Turno.findAll() );
		List< Integer > rowsWithErrors = new ArrayList<Integer >();

		for ( DemandasImportExcelBean bean : sheetContent )
		{
			Turno turno = turnosBDMap.get( bean.getTurnoStr() );
			if ( turno != null )
			{
				bean.setTurno(turno );
			}
			else
			{
				rowsWithErrors.add(bean.getRow() );
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				TURNO_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}

	private void checkNonRegisteredCurso(
		List< DemandasImportExcelBean > sheetContent )
	{
		// [ CódigoCurso -> Curso ]
		Map< String, Curso > cursosBDMap = Curso.buildCursoCodigoToCursoMap(
			Curso.findByCenario( getCenario() ) );
	
		List< Integer > rowsWithErrors = new ArrayList< Integer >();
		for ( DemandasImportExcelBean bean : sheetContent )
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

	private void checkNonRegisteredDisciplina(
		List< DemandasImportExcelBean > sheetContent )
	{
		// [ CódigoDisciplina -> Disciplina ]
		Map<String,Disciplina> disciplinasBDMap = Disciplina.buildDisciplinaCodigoToDisciplinaMap(
			Disciplina.findByCenario( getCenario() ) );

		List< Integer > rowsWithErrors = new ArrayList< Integer >();
		for ( DemandasImportExcelBean bean : sheetContent )
		{
			Disciplina disciplina = disciplinasBDMap.get(bean.getDisciplinaStr() );
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

	private void checkNonRegisteredCurriculo(
		List< DemandasImportExcelBean > sheetContent )
	{
		// [ CodigoCurriculo -> Curriculo ]
		Map< String, Curriculo > curriculosBDMap = Curriculo.buildCurriculoCodigoToCurriculoMap(
			Curriculo.findByCenario( getCenario() ) );
		
		List< Integer > rowsWithErrors = new ArrayList< Integer >();
		for ( DemandasImportExcelBean bean : sheetContent )
		{
			Curriculo curriculo = curriculosBDMap.get( bean.getMatrizCurricularStr() );

			if ( curriculo != null )
			{
				bean.setMatrizCurricular( curriculo );
			}
			else
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				MATRIZ_CURRICULAR_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}

	private void checkNonRegisteredDisciplinaEmCurricular( List< DemandasImportExcelBean > sheetContent )
	{
		// [ ChaveNaturalCurriculo -> Curriculo ]
		Map< String, CurriculoDisciplina > curriculosDisciplinasBDMap
			= CurriculoDisciplina.buildNaturalKeyToCurriculoDisciplinaMap(
				CurriculoDisciplina.findByCenario( getCenario() ) );
		
		List< Integer > rowsWithErrors = new ArrayList< Integer >();
		for ( DemandasImportExcelBean bean : sheetContent )
		{
			CurriculoDisciplina curriculoDisciplina = curriculosDisciplinasBDMap.get(
				getNaturalKeyStringDeCurriculoDisciplina( bean ) );

			if ( curriculoDisciplina == null )
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoDisciplinaEmMatrizCurricular(
				DISCIPLINA_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}

	@Transactional
	private void updateDataBase( String sheetName,
		List< DemandasImportExcelBean > sheetContent )
	{
		Map< String, Demanda > demandasBDMap
			= Demanda.buildCampusTurnoCurriculoDisciplinaToDemandaMap( Demanda.findAll() );

		Map< String, Oferta > ofertasBDMap = Oferta.buildCampusTurnoCurriculoToOfertaMap(
			Oferta.findByCenario( getCenario() ) );

		for ( DemandasImportExcelBean demandasExcel : sheetContent )
		{
			String codeDemanda = getCodeDemanda( demandasExcel );
			String codeOferta = getCodeOferta( demandasExcel );

			Demanda demandaBD = demandasBDMap.get( codeDemanda );
			Oferta oferta = ofertasBDMap.get( codeOferta );

			if ( oferta == null )
			{
				oferta = new Oferta();

				oferta.setCampus( demandasExcel.getCampus() );
				oferta.setTurno( demandasExcel.getTurno() );
				oferta.setCurriculo( demandasExcel.getMatrizCurricular() );
				oferta.setCurso( demandasExcel.getCurso() );
				oferta.setReceita( demandasExcel.getReceita() );

				oferta.persist();

				Oferta.entityManager().refresh( oferta );
				ofertasBDMap.put( codeOferta, oferta );
			}

			if ( demandaBD != null )
			{
				// Update
				demandaBD.setQuantidade( demandasExcel.getDemanda() );
				demandaBD.merge();
			}
			else
			{
				// Insert
				Demanda newDemanda = new Demanda();

				newDemanda.setDisciplina( demandasExcel.getDisciplina() );
				newDemanda.setOferta( oferta );
				newDemanda.setQuantidade( demandasExcel.getDemanda() );
				newDemanda.persist();
			}
		}
	}
	
	private void resolveHeaderColumnNames()
	{
		if ( CAMPUS_COLUMN_NAME == null )
		{
			CAMPUS_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoCampus() );
			TURNO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().turno() );
			CURSO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoCurso() );
			MATRIZ_CURRICULAR_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().matrizCurricular() );
			PERIODO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().periodo() );
			DISCIPLINA_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoDisciplina() );
			DEMANDA_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().demandaDeAlunos() );
		}
	}

	// Campus + Turno + Matriz Curricular + Disciplina
	private String getCodeDemanda( DemandasImportExcelBean bean )
	{
		return bean.getCampusStr()
			+ "-" + bean.getTurnoStr()
			+ "-" + bean.getMatrizCurricularStr()
			+ "-" + bean.getDisciplinaStr();
	}

	// Campus + Turno + Matriz Curricular
	private String getCodeOferta( DemandasImportExcelBean bean )
	{
		return bean.getCampusStr()
			+ "-" + bean.getTurnoStr()
			+ "-" + bean.getMatrizCurricularStr();
	}

	public String getNaturalKeyStringDeCurriculoDisciplina( DemandasImportExcelBean bean )
	{
		return bean.getCursoStr()
			+ "-" + bean.getMatrizCurricularStr()
			+ "-" + bean.getPeriodo()
			+ "-" + bean.getDisciplinaStr();
	}
}
