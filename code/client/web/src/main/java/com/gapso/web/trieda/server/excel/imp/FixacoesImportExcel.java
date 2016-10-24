package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Fixacao;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.google.gwt.i18n.client.DateTimeFormat;
import com.google.gwt.i18n.client.DateTimeFormat.PredefinedFormat;

@ProgressDeclarationAnnotation
public class FixacoesImportExcel extends AbstractImportExcel<FixacoesImportExcelBean>
{
	static public String CODIGO_COLUMN_NAME;
	static public String DESCRICAO_COLUMN_NAME;
	static public String CPF_PROFESSOR_COLUMN_NAME;
	static public String CODIGO_DISCIPLINA_COLUMN_NAME;
	static public String CODIGO_CAMPUS_COLUMN_NAME;
	static public String CODIGO_UNIDADE_COLUMN_NAME;
	static public String CODIGO_SALA_COLUMN_NAME;
	static public String HORARIOS_COLUMN_NAME;

	private List<String> headerColumnsNames;

	public FixacoesImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		super(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
		this.resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(CODIGO_COLUMN_NAME);
		this.headerColumnsNames.add(DESCRICAO_COLUMN_NAME);
		this.headerColumnsNames.add(CPF_PROFESSOR_COLUMN_NAME);
		this.headerColumnsNames.add(CODIGO_DISCIPLINA_COLUMN_NAME);
		this.headerColumnsNames.add(CODIGO_CAMPUS_COLUMN_NAME);
		this.headerColumnsNames.add(CODIGO_UNIDADE_COLUMN_NAME);
		this.headerColumnsNames.add(CODIGO_SALA_COLUMN_NAME);
		this.headerColumnsNames.add(HORARIOS_COLUMN_NAME);
	}

	@Override
	protected List<String> getHeaderColumnsNames()
	{
		return this.headerColumnsNames;
	}

	@Override
	protected FixacoesImportExcelBean createExcelBean(Row header, Row row)
	{
		FixacoesImportExcelBean bean = new FixacoesImportExcelBean(row.getRowNum() + 1);

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

					if (CODIGO_COLUMN_NAME.equals(columnName))
					{
						bean.setCodigoStr(cellValue);
					}
					else if (DESCRICAO_COLUMN_NAME.endsWith(columnName))
					{
						bean.setDescricaoStr(cellValue);
					}
					else if (CPF_PROFESSOR_COLUMN_NAME.equals(columnName))
					{
						bean.setProfessorStr(cellValue);
					}
					else if (CODIGO_DISCIPLINA_COLUMN_NAME.equals(columnName))
					{
						bean.setDisciplinaStr(cellValue);
					}
					else if (CODIGO_CAMPUS_COLUMN_NAME.equals(columnName))
					{
						bean.setCampusStr(cellValue);
					}
					else if (CODIGO_UNIDADE_COLUMN_NAME.equals(columnName))
					{
						bean.setUnidadeStr(cellValue);
					}
					else if (CODIGO_SALA_COLUMN_NAME.equals(columnName))
					{
						bean.setSalaStr(cellValue);
					}
					else if (HORARIOS_COLUMN_NAME.equals(columnName))
					{
						bean.setHorariosStr(cellValue);
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
		return ExcelInformationType.FIXACOES.getSheetName();
	}

	@Override
	protected boolean doSyntacticValidation(List<FixacoesImportExcelBean> sheetContent)
	{
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre

		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map<ImportExcelError, List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError, List<Integer>>();

		for (FixacoesImportExcelBean bean : sheetContent)
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
	protected boolean doLogicValidation(List<FixacoesImportExcelBean> sheetContent)
	{
		checkUniqueness(sheetContent);
		return getErrors().isEmpty();
	}

	private void checkUniqueness(List<FixacoesImportExcelBean> sheetContent)
	{
		Map<String, List<Integer>> fixacaoCodigoToRowsMap = new HashMap<String, List<Integer>>();

		for (FixacoesImportExcelBean bean : sheetContent)
		{
			List<Integer> rows = fixacaoCodigoToRowsMap.get(bean.getCodigoStr());

			if (rows == null)
			{
				rows = new ArrayList<Integer>();
				fixacaoCodigoToRowsMap.put(bean.getCodigoStr(), rows);
			}

			rows.add(bean.getRow());
		}

		for (Entry<String, List<Integer>> entry : fixacaoCodigoToRowsMap.entrySet())
		{
			if (entry.getValue().size() > 1)
			{
				getErrors().add(getI18nMessages().excelErroLogicoUnicidadeViolada(entry.getKey(), entry.getValue().toString()));
			}
		}
	}

	@Transactional
	@ProgressReportMethodScan(texto = "Atualizando Banco de Dados")
	@Override
	protected void updateDataBase(List<FixacoesImportExcelBean> sheetContent)
	{
		Map<String, Fixacao> fixacoes = this.montarHash(Fixacao.findByCenario(this.instituicaoEnsino, getCenario()));

		for (FixacoesImportExcelBean fixacaoExcel : sheetContent)
		{
			Fixacao fixacaoBD = fixacoes.get(fixacaoExcel.getCodigoStr());

			if (fixacaoBD != null)
			{
				// Update
				fixacaoBD.setInstituicaoEnsino(this.instituicaoEnsino);
				fixacaoBD.setDescricao(fixacaoExcel.getDescricaoStr());
				//fixacaoBD.setProfessor(Professor.findByNomeCpf(this.instituicaoEnsino, this.cenario, null, fixacaoExcel.getProfessorStr()));
				fixacaoBD.setDisciplina(Disciplina.findByCodigo(this.instituicaoEnsino, this.cenario, fixacaoExcel.getDisciplinaStr()));
				fixacaoBD.setCampus(Campus.findBy(this.instituicaoEnsino, this.cenario, fixacaoExcel.getCampusStr()));
				//fixacaoBD.setUnidade(Unidade.findBy(this.instituicaoEnsino, this.cenario, fixacaoExcel.getUnidadeStr()));
				//fixacaoBD.setSala(Sala.findByCodigo(this.instituicaoEnsino, this.cenario, fixacaoExcel.getSalaStr()));
				fixacaoBD.setHorarios(this.getHorarios(fixacaoExcel));

				fixacaoBD.merge();
				Fixacao.entityManager().refresh(fixacaoBD);
			}
			else
			{
				// Insert
				Fixacao newFixacao = new Fixacao();

				newFixacao.setInstituicaoEnsino(this.instituicaoEnsino);
				newFixacao.setDescricao(fixacaoExcel.getDescricaoStr());
				//newFixacao.setProfessor(Professor.findByNomeCpf(this.instituicaoEnsino, this.cenario, null, fixacaoExcel.getProfessorStr()));
				newFixacao.setDisciplina(Disciplina.findByCodigo(this.instituicaoEnsino, this.cenario, fixacaoExcel.getDisciplinaStr()));
				newFixacao.setCampus(Campus.findBy(this.instituicaoEnsino, this.cenario, fixacaoExcel.getCampusStr()));
				//newFixacao.setUnidade(Unidade.findBy(this.instituicaoEnsino, this.cenario, fixacaoExcel.getUnidadeStr()));
				//newFixacao.setSala(Sala.findByCodigo(this.instituicaoEnsino, this.cenario, fixacaoExcel.getSalaStr()));
				newFixacao.setHorarios(this.getHorarios(fixacaoExcel));

				Fixacao.entityManager().refresh(newFixacao);
			}
		}
	}

	private Set<HorarioDisponivelCenario> getHorarios(FixacoesImportExcelBean fixacaoExcel)
	{
		DateTimeFormat dateTimeFormat = DateTimeFormat.getFormat(PredefinedFormat.HOUR24_MINUTE);
		Set<HorarioDisponivelCenario> horariosSet = new HashSet<HorarioDisponivelCenario>();
		
		for (String horarioString : fixacaoExcel.getHorariosStr().split(";"))
		{
			String[] horarioArray = horarioString.split(" ");

			Semanas diaSemana = Semanas.get(horarioArray[0]);
			SemanaLetiva semanaLetiva = SemanaLetiva.getBy(this.instituicaoEnsino, this.cenario, horarioArray[1]);
			Turno turno = Turno.find(Long.parseLong(horarioArray[2]), this.instituicaoEnsino);
			Date horarioDate = dateTimeFormat.parse(horarioArray[3]);
			
			List<HorarioDisponivelCenario> horariosList = HorarioDisponivelCenario.findBy(this.instituicaoEnsino, this.cenario, semanaLetiva, diaSemana);
		
			for (HorarioDisponivelCenario horario : horariosList) {
				if (horario.getHorarioAula().getTurno().equals(turno) &&
					horario.getHorarioAula().getHorario().equals(horarioDate)) {
					horariosSet.add(horario);
				}
			}
		}

		return horariosSet;
	}

	private Map<String, Fixacao> montarHash(List<Fixacao> fixacoes)
	{
		Map<String, Fixacao> fixacoesHash = new HashMap<String, Fixacao>();
		
		for(Fixacao fixacao : fixacoes) {
			fixacoesHash.put(fixacao.getId().toString(), fixacao);
		}

		return fixacoesHash;
	}

	private void resolveHeaderColumnNames()
	{
		if (CODIGO_COLUMN_NAME == null)
		{
			CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigo());
			DESCRICAO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().descricao());
			CPF_PROFESSOR_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().cpfProfessor());
			CODIGO_DISCIPLINA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigoDisciplina());
			CODIGO_CAMPUS_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigoCampus());
			CODIGO_UNIDADE_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigoUnidade());
			CODIGO_SALA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigoSala());
			HORARIOS_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().diasHorariosAula());
		}
	}

}
