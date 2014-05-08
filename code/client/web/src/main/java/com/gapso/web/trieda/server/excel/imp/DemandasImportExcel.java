package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
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

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.TriedaPar;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
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
	private Boolean criarAlunosVirtuais;

	public DemandasImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino,
		Boolean criarAlunosVirtuais )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();
		this.criarAlunosVirtuais = criarAlunosVirtuais;

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( CAMPUS_COLUMN_NAME );
		this.headerColumnsNames.add( TURNO_COLUMN_NAME );
		this.headerColumnsNames.add( CURSO_COLUMN_NAME );
		this.headerColumnsNames.add( MATRIZ_CURRICULAR_COLUMN_NAME );
		this.headerColumnsNames.add( PERIODO_COLUMN_NAME );
		this.headerColumnsNames.add( DISCIPLINA_COLUMN_NAME );
		this.headerColumnsNames.add( DEMANDA_COLUMN_NAME );
	}
	
	public DemandasImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();
		this.criarAlunosVirtuais = true;

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
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.DEMANDAS.getSheetName().equals( sheetName );
	}

	@Override
	protected List< String > getHeaderColumnsNames(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		return this.headerColumnsNames;
	}

	@Override
	protected DemandasImportExcelBean createExcelBean(
		Row header, Row row, int sheetIndex,
		Sheet sheet, Workbook workbook )
	{
		DemandasImportExcelBean bean
			= new DemandasImportExcelBean( row.getRowNum() + 1 );

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
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
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
		// [ ImportExcelError' -> Lista de linhas onde o erro ocorre ]
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
		List< DemandasImportExcelBean > sheetContent )
	{
		// Verifica se uma demanda ( Campus + Turno + Matriz Curricular +
		// Disciplina + Período ) é adicionada mais de uma vez no arquivo de entrada
		checkUniqueness( sheetContent );

		// Verifica se há referência a algum tipo de contrato não cadastrado
		checkNonRegisteredCampus( sheetContent );
		checkNonRegisteredTurno( sheetContent );
		checkNonRegisteredCurso( sheetContent );
		checkNonRegisteredDisciplina( sheetContent );
		checkNonRegisteredCurriculo( sheetContent );
		checkNonRegisteredDisciplinaEmCurricular( sheetContent );
		checkNonRegisteredOfertas( sheetContent );

		return getErrors().isEmpty();
	}

	private void checkUniqueness(
		List< DemandasImportExcelBean > sheetContent )
	{
		// Map com os códigos " Campus + Turno + Matriz Curricular + Período +
		// Disciplina" e as linhas em que o mesmo aparece no arquivo de entrada

		// [ "Campus + Turno + Matriz Curricular + Período +
		// Disciplina" -> Lista de Linhas do Arquivo de Entrada ]
		Map< String, List< Integer > > demandasToRowsMap
			= new HashMap< String, List< Integer > >();

		for ( DemandasImportExcelBean bean : sheetContent )
		{
			String demandaPeriodo = ( getCodeDemanda( bean ) + bean.getPeriodoStr() );
			List< Integer > rows = demandasToRowsMap.get( demandaPeriodo );

			if ( rows == null )
			{
				rows = new ArrayList< Integer >();
				demandasToRowsMap.put( demandaPeriodo, rows );
			}

			rows.add( bean.getRow() );
		}

		// Verifica se alguma demanda de disciplina / período
		// apareceu mais de uma vez no arquivo de entrada
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
			Campus.findByCenario( this.instituicaoEnsino, getCenario() ) );

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
		Map< String, Turno > turnosBDMap = Turno.buildTurnoNomeToTurnoMap(
			Turno.findAll( this.instituicaoEnsino, getCenario() ) );

		List< Integer > rowsWithErrors = new ArrayList<Integer >();

		for ( DemandasImportExcelBean bean : sheetContent )
		{
			Turno turno = turnosBDMap.get( bean.getTurnoStr() );

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
		List< DemandasImportExcelBean > sheetContent )
	{
		// [ CódigoCurso -> Curso ]
		Map< String, Curso > cursosBDMap = Curso.buildCursoCodigoToCursoMap(
			Curso.findByCenario( this.instituicaoEnsino, getCenario() ) );

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
		Map< String, Disciplina > disciplinasBDMap
			= Disciplina.buildDisciplinaCodigoToDisciplinaMap(
				Disciplina.findByCenario( this.instituicaoEnsino, getCenario() ) );

		List< Integer > rowsWithErrors
			= new ArrayList< Integer >();

		for ( DemandasImportExcelBean bean : sheetContent )
		{
			Disciplina disciplina
				= disciplinasBDMap.get( bean.getDisciplinaStr() );

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
		Map< String, Curriculo > curriculosBDMap	
			= Curriculo.buildCurriculoCodigoToCurriculoMap(
				Curriculo.findByCenario( this.instituicaoEnsino, getCenario() ) );

		List< Integer > rowsWithErrors
			= new ArrayList< Integer >();

		for ( DemandasImportExcelBean bean : sheetContent )
		{
			Curriculo curriculo
				= curriculosBDMap.get( bean.getMatrizCurricularStr() );

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
	
	private void checkNonRegisteredOfertas(
			List< DemandasImportExcelBean > sheetContent )
		{
		
		Map< String, Oferta > ofertasBDMap = Oferta.buildCampusTurnoCurriculoToOfertaMap(
			Oferta.findByCenario( this.instituicaoEnsino, getCenario() ) );
		
		List< Integer > rowsWithErrors
		= new ArrayList< Integer >();
		
		for ( DemandasImportExcelBean bean : sheetContent )
		{
			Oferta oferta = ofertasBDMap.get(
					getCodeOferta(bean) );

			if ( oferta == null )
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}
		
		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoOfertasCursosCampiEmOfertasEDemandas(
				rowsWithErrors.toString() ) );
		}
	}

	private void checkNonRegisteredDisciplinaEmCurricular(
		List< DemandasImportExcelBean > sheetContent )
	{
		// [ NaturalKeyCurriculo -> Curriculo ]
		Map< String, CurriculoDisciplina > curriculosDisciplinasBDMap
			= CurriculoDisciplina.buildNaturalKeyToCurriculoDisciplinaMap(
				CurriculoDisciplina.findByCenario( this.instituicaoEnsino, getCenario() ) );

		List< Integer > rowsWithErrors
			= new ArrayList< Integer >();

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
	@ProgressReportMethodScan(texto = "Atualizando banco de dados")
	private void updateDataBase( String sheetName,
		List< DemandasImportExcelBean > sheetContent )
	{
		Map< String, Demanda > demandasBDMap
			= Demanda.buildCampusTurnoCurriculoDisciplinaToDemandaMap(
				Demanda.findByCenario( this.instituicaoEnsino, getCenario() ) );

		Map< String, Oferta > ofertasBDMap
			= Oferta.buildCampusTurnoCurriculoToOfertaMap(
				Oferta.findByCenario( this.instituicaoEnsino, getCenario() ) );
		
		Map<TriedaPar<Oferta, Integer>, List<TriedaPar<Demanda, Integer>>> ofertaParPeriodoMapDemandaParQuantidade
			= new HashMap<TriedaPar<Oferta, Integer>, List<TriedaPar<Demanda, Integer>>>();

		for ( DemandasImportExcelBean demandasExcel : sheetContent )
		{
			String codeDemanda = getCodeDemanda( demandasExcel );
			String codeOferta = getCodeOferta( demandasExcel );

			Demanda demandaBD = demandasBDMap.get( codeDemanda );
			Oferta oferta = ofertasBDMap.get( codeOferta );

			TriedaPar<Oferta, Integer> key = TriedaPar.create(oferta, demandasExcel.getPeriodo());
			if ( demandaBD != null )
			{
				// Update
				demandaBD.merge();
				
				if (ofertaParPeriodoMapDemandaParQuantidade.get(key) == null)
				{
					TriedaPar<Demanda, Integer> newPar = TriedaPar.create(demandaBD, demandasExcel.getDemanda());
					List<TriedaPar<Demanda, Integer>> newList = new ArrayList<TriedaPar<Demanda, Integer>>();
					newList.add(newPar);
					ofertaParPeriodoMapDemandaParQuantidade.put(key, newList);
				}
				else
				{
					ofertaParPeriodoMapDemandaParQuantidade.get(key).add(TriedaPar.create(demandaBD, demandasExcel.getDemanda()));
				}
			}
			else
			{
				// Insert
				Demanda newDemanda = new Demanda();

				newDemanda.setDisciplina( demandasExcel.getDisciplina() );
				newDemanda.setOferta( oferta );

				newDemanda.persist();
				
				if (ofertaParPeriodoMapDemandaParQuantidade.get(key) == null)
				{
					TriedaPar<Demanda, Integer> newPar = TriedaPar.create(newDemanda, demandasExcel.getDemanda());
					List<TriedaPar<Demanda, Integer>> newList = new ArrayList<TriedaPar<Demanda, Integer>>();
					newList.add(newPar);
					ofertaParPeriodoMapDemandaParQuantidade.put(key, newList);
				}
				else
				{
					ofertaParPeriodoMapDemandaParQuantidade.get(key).add(TriedaPar.create(newDemanda, demandasExcel.getDemanda()));
				}
			}
		}
		//Criando Alunos Virtuais
		if (criarAlunosVirtuais)
		{
			for (Entry<TriedaPar<Oferta, Integer>, List<TriedaPar<Demanda, Integer>>> entry : ofertaParPeriodoMapDemandaParQuantidade.entrySet())
			{
				List<TriedaPar<Demanda, Integer>> demandas = entry.getValue();
				Collections.sort(demandas, new Comparator<TriedaPar<Demanda, Integer>>() {
					@Override
					public int compare(TriedaPar<Demanda, Integer> o1, TriedaPar<Demanda, Integer> o2) {
						return o1.getSegundo().compareTo(o2.getSegundo());
					}
				});
				List<Aluno> alunos = new ArrayList<Aluno>();
				for (TriedaPar<Demanda, Integer> demanda : demandas)
				{
					if (demanda.getSegundo() > 0)
					{
						int alunosCriados = alunos.size();
						for (int i = alunosCriados; i < demanda.getSegundo(); i++)
						{
							Aluno novoAluno = new Aluno();
							novoAluno.setCenario(getCenario());
							novoAluno.setInstituicaoEnsino(instituicaoEnsino);
							novoAluno.setNome("Aluno Virtual " + i);
							novoAluno.setMatricula("Aluno Virtual " + i);
							novoAluno.setFormando(false);
							novoAluno.setVirtual(true);
							novoAluno.setCriadoTrieda(true);
							
							novoAluno.persist();
							
							alunos.add(novoAluno);
						}
		
						for (int i = 0; i < demanda.getSegundo(); i++)
						{
							AlunoDemanda novoAlunoDemanda = new AlunoDemanda();
							novoAlunoDemanda.setAluno(alunos.get(i));
							novoAlunoDemanda.setDemanda(demanda.getPrimeiro());
							novoAlunoDemanda.setPeriodo(entry.getKey().getSegundo());
							novoAlunoDemanda.setPrioridade(1);
							novoAlunoDemanda.setAtendido(false);
							
							novoAlunoDemanda.persist();
						}
					}
				}
				for (Aluno aluno : alunos)
				{
					aluno.setNome("Aluno Virtual " + aluno.getId() + " " + entry.getKey().getPrimeiro().getCurriculo().getCurso().getNome() + " " 
							+  entry.getKey().getPrimeiro().getCurriculo().getCodigo() + " " +  entry.getKey().getSegundo() );
					aluno.setMatricula( aluno.getId().toString() );
					aluno.merge();
				}
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
			DEMANDA_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().demandaDeAlunosVirtual() );
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

	// Curso + Curriculo + Período + Disciplina
	public String getNaturalKeyStringDeCurriculoDisciplina(
		DemandasImportExcelBean bean )
	{
		return bean.getCursoStr()
			+ "-" + bean.getMatrizCurricularStr()
			+ "-" + bean.getPeriodo()
			+ "-" + bean.getDisciplinaStr();
	}
}
