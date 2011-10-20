package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class AlunosDemandaImportExcel
	extends AbstractImportExcel< AlunosDemandaImportExcelBean >
{
	static public String CAMPUS_COLUMN_NAME;
	static public String CURSO_COLUMN_NAME;
	static public String CURRICULO_COLUMN_NAME;
	static public String TURNO_COLUMN_NAME;
	static public String DISCIPLINA_COLUMN_NAME;
	static public String NOME_ALUNO_COLUMN_NAME;
	static public String MATRICULA_ALUNO_COLUMN_NAME;
	static public String PERIODO_COLUMN_NAME;
	static public String PRIORIDADE_COLUMN_NAME;

	private List< String > headerColumnsNames;

	public AlunosDemandaImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( CAMPUS_COLUMN_NAME );
		this.headerColumnsNames.add( TURNO_COLUMN_NAME );
		this.headerColumnsNames.add( CURSO_COLUMN_NAME );
		this.headerColumnsNames.add( CURRICULO_COLUMN_NAME );
		this.headerColumnsNames.add( PERIODO_COLUMN_NAME );
		this.headerColumnsNames.add( DISCIPLINA_COLUMN_NAME );
		this.headerColumnsNames.add( MATRICULA_ALUNO_COLUMN_NAME );
		this.headerColumnsNames.add( NOME_ALUNO_COLUMN_NAME );
		this.headerColumnsNames.add( PRIORIDADE_COLUMN_NAME );
	}

	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.ALUNOS_DEMANDA.getSheetName().equals( sheetName );
	}

	@Override
	protected List< String > getHeaderColumnsNames(
		int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook )
	{
		return this.headerColumnsNames;
	}

	@Override
	protected AlunosDemandaImportExcelBean createExcelBean(
		HSSFRow header, HSSFRow row, int sheetIndex,
		HSSFSheet sheet, HSSFWorkbook workbook )
	{
		AlunosDemandaImportExcelBean bean
			= new AlunosDemandaImportExcelBean( row.getRowNum() + 1 );

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

					if ( CAMPUS_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setCodigoCampusStr( cellValue );
					}
					else if ( TURNO_COLUMN_NAME.equals( columnName ) )
					{
						bean.setCodigoTurnoStr( cellValue );
					}
					else if ( CURSO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setCodigoCursoStr( cellValue );
					}
					else if ( CURRICULO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setCodigoCurriculoStr( cellValue );
					}
					else if ( PERIODO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setPeriodoStr( cellValue );
					}
					else if ( DISCIPLINA_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDisciplinaCodigoStr( cellValue );
					}
					else if ( MATRICULA_ALUNO_COLUMN_NAME.endsWith( columnName ) )
					{
						cell.setCellType( HSSFCell.CELL_TYPE_STRING );
						cellValue = getCellValue( cell );
						bean.setMatriculaAlunoStr( cellValue );
					}
					else if ( NOME_ALUNO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setNomeAlunoStr( cellValue );
					}
					else if ( PRIORIDADE_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setPrioridadeAlunoStr( cellValue );
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
		return ExcelInformationType.ALUNOS_DEMANDA.getSheetName();
	}

	@Override
	protected void processSheetContent(
		String sheetName, List< AlunosDemandaImportExcelBean > sheetContent )
	{
		if ( doSyntacticValidation( sheetName, sheetContent )
			&& doLogicValidation( sheetName, sheetContent ) )
		{
			updateDataBase( sheetName, sheetContent );
		}
	}

	private boolean doSyntacticValidation(
		String sheetName, List< AlunosDemandaImportExcelBean > sheetContent )
	{
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map< ImportExcelError, List< Integer > > syntacticErrorsMap
			= new HashMap< ImportExcelError, List< Integer > >();

		for ( AlunosDemandaImportExcelBean bean : sheetContent )
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
		String sheetName, List< AlunosDemandaImportExcelBean > sheetContent )
	{
		checkNonRegisteredCampus( sheetContent );
		checkNonRegisteredTurno( sheetContent );
		checkNonRegisteredCurso( sheetContent );
		checkNonRegisteredDisciplina( sheetContent );
		checkNonRegisteredCurriculo( sheetContent );

		return getErrors().isEmpty();
	}

	private void checkNonRegisteredCampus(
		List< AlunosDemandaImportExcelBean > sheetContent )
	{
		// [ CódidoCampus -> Campus ]
		Map< String, Campus > campiBDMap = Campus.buildCampusCodigoToCampusMap(
			Campus.findByCenario( this.instituicaoEnsino, getCenario() ) );

		List< Integer > rowsWithErrors = new ArrayList< Integer >();

		for ( AlunosDemandaImportExcelBean bean : sheetContent )
		{
			Campus campus = campiBDMap.get( bean.getCodigoCampusStr() );

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
		List< AlunosDemandaImportExcelBean > sheetContent )
	{
		// [ NomeTurno -> Turno ]
		Map< String, Turno > turnosBDMap = Turno.buildTurnoNomeToTurnoMap(
			Turno.findAll( this.instituicaoEnsino ) );

		List< Integer > rowsWithErrors = new ArrayList<Integer >();

		for ( AlunosDemandaImportExcelBean bean : sheetContent )
		{
			Turno turno = turnosBDMap.get( bean.getCodigoTurnoStr() );

			if ( turno != null )
			{
				bean.setTurno( turno );
				}
			else
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				TURNO_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}

	private void checkNonRegisteredCurso(
		List< AlunosDemandaImportExcelBean > sheetContent )
	{
		// [ CódigoCurso -> Curso ]
		Map< String, Curso > cursosBDMap = Curso.buildCursoCodigoToCursoMap(
			Curso.findByCenario( this.instituicaoEnsino, getCenario() ) );

		List< Integer > rowsWithErrors = new ArrayList< Integer >();

		for ( AlunosDemandaImportExcelBean bean : sheetContent )
		{
			Curso curso = cursosBDMap.get( bean.getCodigoCursoStr() );

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
		List< AlunosDemandaImportExcelBean > sheetContent )
	{
		// [ CódigoDisciplina -> Disciplina ]
		Map< String, Disciplina > disciplinasBDMap
			= Disciplina.buildDisciplinaCodigoToDisciplinaMap(
				Disciplina.findByCenario( this.instituicaoEnsino, getCenario() ) );

		List< Integer > rowsWithErrors
			= new ArrayList< Integer >();

		for ( AlunosDemandaImportExcelBean bean : sheetContent )
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

	private void checkNonRegisteredCurriculo(
		List< AlunosDemandaImportExcelBean > sheetContent )
	{
		// [ CodigoCurriculo -> Curriculo ]
		Map< String, Curriculo > curriculosBDMap	
			= Curriculo.buildCurriculoCodigoToCurriculoMap(
				Curriculo.findByCenario( this.instituicaoEnsino, getCenario() ) );

		List< Integer > rowsWithErrors
			= new ArrayList< Integer >();

		for ( AlunosDemandaImportExcelBean bean : sheetContent )
		{
			Curriculo curriculo = curriculosBDMap.get( bean.getCodigoCurriculoStr() );

			if ( curriculo != null )
			{
				bean.setCurriculo( curriculo );
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
	private void updateDataBase(
		String sheetName, List< AlunosDemandaImportExcelBean > sheetContent )
	{
		Map< String, Aluno > alunosBDMap
			= Aluno.buildMatriculaAlunoToAlunoMap(
				Aluno.findAll( this.instituicaoEnsino ) );

		Map< String, Demanda > demandasBDMap
			= Demanda.buildCampusTurnoCurriculoDisciplinaToDemandaMap(
				Demanda.findAll( this.instituicaoEnsino ) );

		for ( AlunosDemandaImportExcelBean alunosDemandaExcel : sheetContent )
		{
			String codeDemanda = getCodeDemanda( alunosDemandaExcel );
			String codeAluno = alunosDemandaExcel.getMatriculaAlunoStr();

			Demanda demandaBD = demandasBDMap.get( codeDemanda );
			Aluno alunoBD = alunosBDMap.get( codeAluno );

			if ( alunoBD == null )
			{
				alunoBD = new Aluno();

				alunoBD.setInstituicaoEnsino( this.instituicaoEnsino );
				alunoBD.setCenario( this.cenario );
				alunoBD.setNome( alunosDemandaExcel.getNomeAlunoStr() );
				// alunoBD.setCpf( alunosDemandaExcel.getCpfAlunoStr() );
				alunoBD.setMatricula( alunosDemandaExcel.getMatriculaAlunoStr() );

				alunoBD.persist();
				Aluno.entityManager().refresh( alunoBD );
				alunosBDMap.put( codeAluno, alunoBD );
			}

			if ( demandaBD != null && alunoBD != null )
			{
				AlunoDemanda alunoDemandaBD = AlunoDemanda.findByDemandaAndAluno(
					this.instituicaoEnsino, demandaBD, alunoBD );

				if ( alunoDemandaBD != null )
				{
					// Update
					alunoDemandaBD.setPrioridade( alunosDemandaExcel.getPrioridade() );
					demandaBD.merge();
				}
				else
				{
					// Insert
					AlunoDemanda newAlunoDemanda = new AlunoDemanda();

					newAlunoDemanda.setAluno( alunoBD );
					newAlunoDemanda.setDemanda( demandaBD );
					newAlunoDemanda.setAtendido( false );
					newAlunoDemanda.setPrioridade( alunosDemandaExcel.getPrioridade() );

					newAlunoDemanda.persist();
				}
			}
		}
	}

	// Campus + Turno + Matriz Curricular + Disciplina
	private String getCodeDemanda( AlunosDemandaImportExcelBean bean )
	{
		return bean.getCodigoCampusStr()
			+ "-" + bean.getCodigoTurnoStr()
			+ "-" + bean.getCodigoCurriculoStr()
			+ "-" + bean.getDisciplinaCodigoStr();
	}

	private void resolveHeaderColumnNames()
	{
		if ( CAMPUS_COLUMN_NAME == null )
		{
			CAMPUS_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoCampus() );
			CURSO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoCurso() );
			CURRICULO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().matrizCurricular() );
			TURNO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().turno() );
			DISCIPLINA_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoDisciplina() );
			NOME_ALUNO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().nomeAluno() );
			MATRICULA_ALUNO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().matriculaAluno() );
			PERIODO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().periodo() );
			PRIORIDADE_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().prioridadeAlunoDemanda() );
		}
	}
}
