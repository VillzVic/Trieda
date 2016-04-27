package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.services.SolucaoDomainService;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.TriedaUtil;

@ProgressDeclarationAnnotation
public class AtendimentoImportExcel extends AbstractImportExcel<AtendimentoImportExcelBean>
{
	static public String CAMPUS_COLUMN_NAME;
	static public String AMBIENTE_COLUMN_NAME;
	static public String DIA_SEMANA_COLUMN_NAME;
	static public String HORARIO_INICIAL_COLUMN_NAME;
	static public String HORARIO_FINAL_COLUMN_NAME;
	static public String DISCIPLINA_AULA_COLUMN_NAME;
	static public String TURMA_COLUMN_NAME;
	static public String ATENDIMENTO_PROFESSOR_CPF_COLUMN_NAME;
	static public String ATENDIMENTO_CRED_PRATICOS_COLUMN_NAME;
	static public String ATENDIMENTO_CRED_TEORICOS_COLUMN_NAME;
	static public String TURNO_COLUMN_NAME;
	static public String CURSO_COLUMN_NAME;
	static public String CURRICULO_COLUMN_NAME;
	static public String MATRICULA_ALUNO_COLUMN_NAME;
	static public String PERIODO_COLUMN_NAME;
	static public String DISCIPLINA_DEMANDA_COLUMN_NAME;
	static public String PRIORIDADE_COLUMN_NAME;

	private List<String> headerColumnsNames;

	private SolucaoDomainService solucaoDomainService = new SolucaoDomainService();

	public AtendimentoImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		super(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(CAMPUS_COLUMN_NAME);
		this.headerColumnsNames.add(AMBIENTE_COLUMN_NAME);
		this.headerColumnsNames.add(DIA_SEMANA_COLUMN_NAME);
		this.headerColumnsNames.add(HORARIO_INICIAL_COLUMN_NAME);
		this.headerColumnsNames.add(HORARIO_FINAL_COLUMN_NAME);
		this.headerColumnsNames.add(DISCIPLINA_AULA_COLUMN_NAME);
		this.headerColumnsNames.add(TURMA_COLUMN_NAME);
		this.headerColumnsNames.add(ATENDIMENTO_PROFESSOR_CPF_COLUMN_NAME);
		this.headerColumnsNames.add(ATENDIMENTO_CRED_PRATICOS_COLUMN_NAME);
		this.headerColumnsNames.add(ATENDIMENTO_CRED_TEORICOS_COLUMN_NAME);
		this.headerColumnsNames.add(TURNO_COLUMN_NAME);
		this.headerColumnsNames.add(CURSO_COLUMN_NAME);
		this.headerColumnsNames.add(CURRICULO_COLUMN_NAME);
		this.headerColumnsNames.add(MATRICULA_ALUNO_COLUMN_NAME);
		this.headerColumnsNames.add(PERIODO_COLUMN_NAME);
		this.headerColumnsNames.add(DISCIPLINA_DEMANDA_COLUMN_NAME);
		this.headerColumnsNames.add(PRIORIDADE_COLUMN_NAME);
	}

	@Override
	protected List<String> getHeaderColumnsNames()
	{
		return this.headerColumnsNames;
	}

	@Override
	protected AtendimentoImportExcelBean createExcelBean(Row header, Row row)
	{
		AtendimentoImportExcelBean bean = new AtendimentoImportExcelBean(row.getRowNum() + 1);

		for (int cellIndex = row.getFirstCellNum(); cellIndex <= row.getLastCellNum(); cellIndex++)
		{
			Cell cell = row.getCell(cellIndex);
			if (cell != null)
			{
				Cell headerCell = header.getCell(cell.getColumnIndex());
				if (headerCell != null)
				{
					String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue(cell);

					if (CAMPUS_COLUMN_NAME.equals(columnName))
					{
						bean.setAula_campusCodigoStr(cellValue);
					}
					else if (AMBIENTE_COLUMN_NAME.equals(columnName))
					{
						bean.setAula_AmbienteCodigoStr(cellValue);
					}
					else if (DIA_SEMANA_COLUMN_NAME.equals(columnName))
					{
						bean.setAula_diaSemanaStr(cellValue);
					}
					else if (HORARIO_INICIAL_COLUMN_NAME.equals(columnName))
					{
						bean.setAula_horarioInicialStr(cellValue);
					}
					else if (HORARIO_FINAL_COLUMN_NAME.equals(columnName))
					{
						bean.setAula_horarioFinalStr(cellValue);
					}
					else if (DISCIPLINA_AULA_COLUMN_NAME.equals(columnName))
					{
						bean.setAula_disciplinaAulaCodigoStr(cellValue);
					}
					else if (TURMA_COLUMN_NAME.equals(columnName))
					{
						bean.setAula_turmaStr(cellValue);
					}
					else if (ATENDIMENTO_PROFESSOR_CPF_COLUMN_NAME.equals(columnName))
					{
						bean.setAula_cpfProfessorStr(cellValue);
					}
					else if (ATENDIMENTO_CRED_PRATICOS_COLUMN_NAME.equals(columnName))
					{
						bean.setAula_qtdCreditosPraticosStr(cellValue);
					}
					else if (ATENDIMENTO_CRED_TEORICOS_COLUMN_NAME.equals(columnName))
					{
						bean.setAula_qtdCreditosTeoricosStr(cellValue);
					}
					else if (TURNO_COLUMN_NAME.equals(columnName))
					{
						bean.setDem_codigoTurnoStr(cellValue);
					}
					else if (CURSO_COLUMN_NAME.equals(columnName))
					{
						bean.setDem_codigoCursoStr(cellValue);
					}
					else if (CURRICULO_COLUMN_NAME.equals(columnName))
					{
						bean.setDem_codigoCurriculoStr(cellValue);
					}
					else if (MATRICULA_ALUNO_COLUMN_NAME.equals(columnName))
					{
						bean.setDem_matriculaAlunoStr(cellValue);
					}
					else if (PERIODO_COLUMN_NAME.equals(columnName))
					{
						bean.setDem_periodoStr(cellValue);
					}
					else if (DISCIPLINA_DEMANDA_COLUMN_NAME.equals(columnName))
					{
						bean.setDem_disciplinaCodigoStr(cellValue);
					}
					else if (PRIORIDADE_COLUMN_NAME.equals(columnName))
					{
						bean.setDem_prioridadeAlunoStr(cellValue);
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
		return ExcelInformationType.ATENDIMENTOS_POR_ALUNO.getSheetName();
	}

	@Override
	protected boolean doSyntacticValidation(List<AtendimentoImportExcelBean> sheetContent)
	{
		// Map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map<ImportExcelError, List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError, List<Integer>>();
		for (AtendimentoImportExcelBean bean : sheetContent)
		{
			List<ImportExcelError> errorsBean = bean.checkSyntacticErrors();
			for (ImportExcelError error : errorsBean)
			{
				List<Integer> rowsWithErrors = syntacticErrorsMap.get(error);
				if (rowsWithErrors == null)
				{
					rowsWithErrors = new ArrayList<Integer>();
					syntacticErrorsMap.put(error, rowsWithErrors);
				}
				rowsWithErrors.add(bean.getRow());
			}
		}

		// Coleta os erros e adiciona os mesmos na lista de mensagens
		for (ImportExcelError error : syntacticErrorsMap.keySet())
		{
			List<Integer> linhasComErro = syntacticErrorsMap.get(error);
			getErrors().add(error.getMessage(linhasComErro.toString(), getI18nMessages()));
		}

		return syntacticErrorsMap.isEmpty();
	}

	@Override
	protected boolean doLogicValidation(List<AtendimentoImportExcelBean> sheetContent)
	{
		checkNonRegisteredCampus(sheetContent);
		checkNonRegisteredTurno(sheetContent);
		checkNonRegisteredCurso(sheetContent);
		checkNonRegisteredDisciplina(sheetContent);
		checkNonRegisteredCurriculo(sheetContent);
		checkNonRegisteredAmbiente(sheetContent);
		checkNonRegisteredProfessor(sheetContent);
		checkNonRegisteredAluno(sheetContent);
		checkNonRegisteredAlunoDemanda(sheetContent);

		return getErrors().isEmpty();
	}

	private void checkNonRegisteredCampus(List<AtendimentoImportExcelBean> sheetContent)
	{
		// [ CódidoCampus -> Campus ]
		Map<String, Campus> campiBDMap = Campus.buildCampusCodigoToCampusMap(Campus.findByCenario(this.instituicaoEnsino, getCenario()));
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (AtendimentoImportExcelBean bean : sheetContent)
		{
			Campus campus = campiBDMap.get(bean.getAula_campusCodigoStr());
			if (campus != null)
			{
				bean.setCampus(campus);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(CAMPUS_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredTurno(List<AtendimentoImportExcelBean> sheetContent)
	{
		// [ NomeTurno -> Turno ]
		Map<String, Turno> turnosBDMap = Turno.buildTurnoNomeToTurnoMap(Turno.findAll(this.instituicaoEnsino, getCenario()));
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (AtendimentoImportExcelBean bean : sheetContent)
		{
			Turno turno = turnosBDMap.get(bean.getDem_codigoTurnoStr());
			if (turno != null)
			{
				bean.setTurno(turno);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(TURNO_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredCurso(List<AtendimentoImportExcelBean> sheetContent)
	{
		// [ CódigoCurso -> Curso ]
		Map<String, Curso> cursosBDMap = Curso.buildCursoCodigoToCursoMap(Curso.findByCenario(this.instituicaoEnsino, getCenario()));
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (AtendimentoImportExcelBean bean : sheetContent)
		{
			Curso curso = cursosBDMap.get(bean.getDem_codigoCursoStr());
			if (curso != null)
			{
				bean.setCurso(curso);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(CURSO_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredDisciplina(List<AtendimentoImportExcelBean> sheetContent)
	{
		// [ CódigoDisciplina -> Disciplina ]
		Map<String, Disciplina> disciplinasBDMap = Disciplina.buildDisciplinaCodigoToDisciplinaMap(Disciplina.findByCenario(this.instituicaoEnsino, getCenario()));
		List<Integer> rowsWithErrorsDisAula = new ArrayList<Integer>();
		List<Integer> rowsWithErrorsDisDem = new ArrayList<Integer>();
		for (AtendimentoImportExcelBean bean : sheetContent)
		{
			Disciplina disciplinaAula = disciplinasBDMap.get(bean.getAula_disciplinaAulaCodigoStr());
			if (disciplinaAula != null)
			{
				bean.setDisciplinaAula(disciplinaAula);
			}
			else
			{
				rowsWithErrorsDisAula.add(bean.getRow());
			}

			Disciplina disciplinaDemanda = disciplinasBDMap.get(bean.getDem_disciplinaCodigoStr());
			if (disciplinaDemanda != null)
			{
				bean.setDisciplinaDemanda(disciplinaDemanda);
			}
			else
			{
				rowsWithErrorsDisDem.add(bean.getRow());
			}
		}

		if (!rowsWithErrorsDisAula.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(DISCIPLINA_AULA_COLUMN_NAME, rowsWithErrorsDisAula.toString()));
		}

		if (!rowsWithErrorsDisDem.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(DISCIPLINA_DEMANDA_COLUMN_NAME, rowsWithErrorsDisDem.toString()));
		}
	}

	private void checkNonRegisteredCurriculo(List<AtendimentoImportExcelBean> sheetContent)
	{
		// [ CodigoCurriculo -> Curriculo ]
		Map<String, Curriculo> curriculosBDMap = Curriculo.buildCurriculoCodigoToCurriculoMap(Curriculo.findByCenario(this.instituicaoEnsino, getCenario()));
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (AtendimentoImportExcelBean bean : sheetContent)
		{
			Curriculo curriculo = curriculosBDMap.get(bean.getDem_codigoCurriculoStr());
			if (curriculo != null)
			{
				bean.setCurriculo(curriculo);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(CURRICULO_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredAmbiente(List<AtendimentoImportExcelBean> sheetContent)
	{
		// [ CodidoSala -> Sala ]
		Map<String, Sala> salasBDMap = Sala.buildSalaCodigoToSalaMap(Sala.findByCenario(this.instituicaoEnsino, getCenario()));
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (AtendimentoImportExcelBean bean : sheetContent)
		{
			Sala sala = salasBDMap.get(bean.getAula_AmbienteCodigoStr());
			if (sala != null)
			{
				bean.setAmbiente(sala);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(AMBIENTE_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredProfessor(List<AtendimentoImportExcelBean> sheetContent)
	{
		// [ CpfProfessor -> Professor ]
		Map<String, Professor> professoresBDMap = Professor.buildProfessorCpfToProfessorMap(Professor.findByCenario(this.instituicaoEnsino, getCenario()));
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (AtendimentoImportExcelBean bean : sheetContent)
		{
			Professor professor = professoresBDMap.get(bean.getAula_cpfProfessorStr());
			if (professor != null)
			{
				bean.setProfessor(professor);
			}
			else
			{
				if (!bean.podeSerProfessorVirtual())
				{
					rowsWithErrors.add(bean.getRow());
				}
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(ATENDIMENTO_PROFESSOR_CPF_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredAluno(List<AtendimentoImportExcelBean> sheetContent)
	{
		// [ Matricula -> Aluno ]
		Map<String, Aluno> alunoBDMap = Aluno.buildMatriculaAlunoToAlunoMap(Aluno.findByCenario(instituicaoEnsino, getCenario()));
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (AtendimentoImportExcelBean bean : sheetContent)
		{
			Aluno aluno = alunoBDMap.get(bean.getDem_matriculaAlunoStr());
			if (aluno != null)
			{
				bean.setAluno(aluno);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(MATRICULA_ALUNO_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredAlunoDemanda(List<AtendimentoImportExcelBean> sheetContent)
	{
		Map<String, AlunoDemanda> alunoDemandaBDMap = AlunoDemanda.buildAlunoDemandaKeyToAlunoDemandaMap(instituicaoEnsino, getCenario());
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (AtendimentoImportExcelBean bean : sheetContent)
		{
			AlunoDemanda alunoDem = alunoDemandaBDMap.get(bean.getAlunoDemandaNaturalKeyString());
			if (alunoDem != null)
			{
				bean.setAlunoDemanda(alunoDem);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas("{Matricula,Disciplina}", rowsWithErrors.toString()));
		}
	}

	@Transactional
	@Override
	protected void updateDataBase(List<AtendimentoImportExcelBean> sheetContent)
	{
		// colhe professores virtuais
		Map<String, Set<Disciplina>> profVirtualIdToDisciplinasMap = new HashMap<String, Set<Disciplina>>();
		for (AtendimentoImportExcelBean bean : sheetContent)
		{
			if (bean.getProfessor() == null)
			{
				Set<Disciplina> disciplinasDoProfessorVirtual = profVirtualIdToDisciplinasMap.get(bean.getAula_cpfProfessorStr());
				if (disciplinasDoProfessorVirtual == null)
				{
					disciplinasDoProfessorVirtual = new HashSet<Disciplina>();
					profVirtualIdToDisciplinasMap.put(bean.getAula_cpfProfessorStr(), disciplinasDoProfessorVirtual);
				}
				disciplinasDoProfessorVirtual.add(bean.getDisciplinaAula());
			}
		}

		// cria Map de HorarioDisponivelCenario
		List<HorarioDisponivelCenario> hdcs = HorarioDisponivelCenario.findAll(this.instituicaoEnsino, this.cenario);
		Map<SemanaLetiva, Map<Semanas, Map<Turno, Map<String, HorarioDisponivelCenario>>>> semLetToDiaSemToTurnoToHIToHDCMap = new HashMap<SemanaLetiva, Map<Semanas, Map<Turno, Map<String, HorarioDisponivelCenario>>>>();
		Map<SemanaLetiva, Map<Turno, Set<HorarioAula>>> semLetToTurnoToHIsMap = new HashMap<SemanaLetiva, Map<Turno, Set<HorarioAula>>>();
		for (HorarioDisponivelCenario hdc : hdcs)
		{
			SemanaLetiva semanaLetiva = hdc.getHorarioAula().getSemanaLetiva();
			Semanas diaSemana = hdc.getDiaSemana();
			HorarioAula horAula = hdc.getHorarioAula();
			Turno turno = horAula.getTurno();
			String horarioAulaStr = TriedaUtil.shortTimeString(hdc.getHorarioAula().getHorario());

			// preenche semLetToDiaSemToTurnoToHIToHDCMap
			Map<Semanas, Map<Turno, Map<String, HorarioDisponivelCenario>>> diaSemToTurnoToHIToHDCMap = semLetToDiaSemToTurnoToHIToHDCMap.get(semanaLetiva);
			if (diaSemToTurnoToHIToHDCMap == null)
			{
				diaSemToTurnoToHIToHDCMap = new HashMap<Semanas, Map<Turno, Map<String, HorarioDisponivelCenario>>>();
				semLetToDiaSemToTurnoToHIToHDCMap.put(semanaLetiva, diaSemToTurnoToHIToHDCMap);
			}
			Map<Turno, Map<String, HorarioDisponivelCenario>> turnoToHIToHDCMap = diaSemToTurnoToHIToHDCMap.get(diaSemana);
			if (turnoToHIToHDCMap == null)
			{
				turnoToHIToHDCMap = new HashMap<Turno, Map<String, HorarioDisponivelCenario>>();
				diaSemToTurnoToHIToHDCMap.put(diaSemana, turnoToHIToHDCMap);
			}
			Map<String, HorarioDisponivelCenario> hiToHDCMap = turnoToHIToHDCMap.get(turno);
			if (hiToHDCMap == null)
			{
				hiToHDCMap = new HashMap<String, HorarioDisponivelCenario>();
				turnoToHIToHDCMap.put(turno, hiToHDCMap);
			}
			hiToHDCMap.put(horarioAulaStr, hdc);

			// preenche semLetToTurnoToHIsMap
			Map<Turno, Set<HorarioAula>> turnoToHIsMap = semLetToTurnoToHIsMap.get(semanaLetiva);
			if (turnoToHIsMap == null)
			{
				turnoToHIsMap = new HashMap<Turno, Set<HorarioAula>>();
				semLetToTurnoToHIsMap.put(semanaLetiva, turnoToHIsMap);
			}
			Set<HorarioAula> his = turnoToHIsMap.get(turno);
			if (his == null)
			{
				his = new HashSet<HorarioAula>();
				turnoToHIsMap.put(turno, his);
			}
			his.add(horAula);
		}

		// ordenada os horarios das semanas letivas
		Map<SemanaLetiva, Map<Turno, List<HorarioAula>>> semLetToTurnoToHIsOrdenadosMap = new HashMap<SemanaLetiva, Map<Turno, List<HorarioAula>>>();
		for (Entry<SemanaLetiva, Map<Turno, Set<HorarioAula>>> e1 : semLetToTurnoToHIsMap.entrySet())
		{
			Map<Turno, List<HorarioAula>> turnoToHIsOrdenadosMap = new HashMap<Turno, List<HorarioAula>>();
			semLetToTurnoToHIsOrdenadosMap.put(e1.getKey(), turnoToHIsOrdenadosMap);
			for (Entry<Turno, Set<HorarioAula>> e2 : e1.getValue().entrySet())
			{
				List<HorarioAula> hisOrdenados = new ArrayList<HorarioAula>(e2.getValue());
				Collections.sort(hisOrdenados);
				turnoToHIsOrdenadosMap.put(e2.getKey(), hisOrdenados);
			}
		}

		int count = 0, total = sheetContent.size();
		System.out.print(" " + total);
		Map<String, AtendimentoOperacional> keyToAtendimentoOpMap = new HashMap<String, AtendimentoOperacional>();
		Map<String, List<AtendimentoOperacional>> profVirtualKeyToAtendimentosOpMap = new HashMap<String, List<AtendimentoOperacional>>();
		for (AtendimentoImportExcelBean bean : sheetContent)
		{
			SemanaLetiva semanaLetiva = bean.getCurriculo().getSemanaLetiva();
			Semanas diaSemana = bean.getDiaSemana();
			Turno turno = bean.getTurno();
			String turma = bean.getAula_turmaStr();
			Sala ambiente = bean.getAmbiente();
			Oferta oferta = bean.getOferta();
			Disciplina dis = bean.getDisciplinaDemanda();
			Disciplina disSubs = bean.getDisciplinaSubstituta();
			Professor professorReal = bean.getProfessor();
			boolean temCredTeorico = bean.getQtdCreditosTeoricos() > 0;

			List<HorarioAula> horariosDeAulaOrdenados = semLetToTurnoToHIsOrdenadosMap.get(semanaLetiva).get(turno);

			String horarioInicio = bean.getAula_horarioInicialStr();
			HorarioDisponivelCenario hdc = getHDC(semanaLetiva, diaSemana, turno, horarioInicio, semLetToDiaSemToTurnoToHIToHDCMap);
			for (int i = 0; i < bean.getTotalCreditos(); i++)
			{
				if (i > 0)
				{
					HorarioAula proxHA = horariosDeAulaOrdenados.get(horariosDeAulaOrdenados.indexOf(hdc.getHorarioAula()));
					horarioInicio = TriedaUtil.shortTimeString(proxHA.getHorario());
					hdc = getHDC(semanaLetiva, diaSemana, turno, horarioInicio, semLetToDiaSemToTurnoToHIToHDCMap);
				}

				String keyAtendOp = AtendimentoOperacional.getKeyAtendOp(ambiente, hdc, temCredTeorico, turma, dis, disSubs, professorReal, bean.getAula_cpfProfessorStr(), oferta);
				AtendimentoOperacional atdOp = keyToAtendimentoOpMap.get(keyAtendOp);
				if (atdOp == null)
				{
					atdOp = criaAtendimentoOp(turma, ambiente, oferta, dis, disSubs, professorReal, temCredTeorico, hdc);
					keyToAtendimentoOpMap.put(keyAtendOp, atdOp);
				}
				atdOp.getAlunosDemanda().add(bean.getAlunoDemanda());

				if (professorReal == null)
				{
					List<AtendimentoOperacional> atendimentosDoProfessorVirtual = profVirtualKeyToAtendimentosOpMap.get(bean.getAula_cpfProfessorStr());
					if (atendimentosDoProfessorVirtual == null)
					{
						atendimentosDoProfessorVirtual = new ArrayList<AtendimentoOperacional>();
						profVirtualKeyToAtendimentosOpMap.put(bean.getAula_cpfProfessorStr(), atendimentosDoProfessorVirtual);
					}
					atendimentosDoProfessorVirtual.add(atdOp);
				}
			}

			count++;
			total--;
			if (count == 100)
			{
				System.out.println("   Faltam " + total + " Atendimentos");
				count = 0;
			}
		}

		this.solucaoDomainService.escreveNovaSolucaoNoBD(profVirtualIdToDisciplinasMap, profVirtualKeyToAtendimentosOpMap, keyToAtendimentoOpMap.values(), this.cenario, this.instituicaoEnsino);
	}

	private HorarioDisponivelCenario getHDC(SemanaLetiva semLet, Semanas diaSem, Turno turno, String hi,
					Map<SemanaLetiva, Map<Semanas, Map<Turno, Map<String, HorarioDisponivelCenario>>>> semLetToDiaSemToTurnoToHIToHDCMap)
	{
		Map<Semanas, Map<Turno, Map<String, HorarioDisponivelCenario>>> diaSemToTurnoToHIToHDCMap = semLetToDiaSemToTurnoToHIToHDCMap.get(semLet);
		if (diaSemToTurnoToHIToHDCMap != null)
		{
			Map<Turno, Map<String, HorarioDisponivelCenario>> turnoToHIToHDCMap = diaSemToTurnoToHIToHDCMap.get(diaSem);
			if (turnoToHIToHDCMap != null)
			{
				Map<String, HorarioDisponivelCenario> hiToHDCMap = turnoToHIToHDCMap.get(turno);
				if (hiToHDCMap != null)
				{
					return hiToHDCMap.get(hi);
				}
			}
		}
		return null;
	}

	private AtendimentoOperacional criaAtendimentoOp(String turma, Sala sala, Oferta oferta, Disciplina disciplina, Disciplina disciplinaSubstituta, Professor professor, Boolean creditoTeorico,
					HorarioDisponivelCenario hdc)
	{
		AtendimentoOperacional atendimentoOperacional = new AtendimentoOperacional();

		atendimentoOperacional.setInstituicaoEnsino(this.instituicaoEnsino);
		atendimentoOperacional.setCenario(this.cenario);
		atendimentoOperacional.setTurma(turma);
		atendimentoOperacional.setSala(sala);
		atendimentoOperacional.setOferta(oferta);
		atendimentoOperacional.setDisciplina(disciplina);
		atendimentoOperacional.setDisciplinaSubstituta(disciplinaSubstituta);
		atendimentoOperacional.setQuantidadeAlunos(1);
		if (professor != null)
		{
			atendimentoOperacional.setProfessor(professor);
		}
		atendimentoOperacional.setCreditoTeorico(creditoTeorico);
		atendimentoOperacional.setHorarioDisponivelCenario(hdc);
		atendimentoOperacional.setConfirmada(false);

		return atendimentoOperacional;
	}

	private void resolveHeaderColumnNames()
	{
		if (CAMPUS_COLUMN_NAME == null)
		{
			CAMPUS_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().campus());
			AMBIENTE_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().sala());
			DIA_SEMANA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().diaDaSemana());
			HORARIO_INICIAL_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().inicio());
			HORARIO_FINAL_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().fim());
			DISCIPLINA_AULA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().disciplinaAula());
			TURMA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().turma());
			ATENDIMENTO_PROFESSOR_CPF_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().professor());
			ATENDIMENTO_CRED_PRATICOS_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().prat());
			ATENDIMENTO_CRED_TEORICOS_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().teo());
			TURNO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().turno());
			CURSO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().curso());
			CURRICULO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().matrizCurricular());
			MATRICULA_ALUNO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().matriculaAluno());
			PERIODO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().periodo());
			DISCIPLINA_DEMANDA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().disciplina());
			PRIORIDADE_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().prioridadeAlunoDemanda());
		}
	}
}
