package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TriedaTrio;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class DisponibilidadesDisciplinasImportExcel extends AbstractImportExcel<DisponibilidadesDisciplinasImportExcelBean>
{
	static public String DISCIPLINA_CODIGO_COLUMN_NAME;
	static public String DIA_SEMANA_COLUMN_NAME;
	static public String HORARIO_INICIAL_COLUMN_NAME;
	static public String HORARIO_FINAL_COLUMN_NAME;

	private List<String> headerColumnsNames;

	public DisponibilidadesDisciplinasImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		super(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(DISCIPLINA_CODIGO_COLUMN_NAME);
		this.headerColumnsNames.add(DIA_SEMANA_COLUMN_NAME);
		this.headerColumnsNames.add(HORARIO_INICIAL_COLUMN_NAME);
		this.headerColumnsNames.add(HORARIO_FINAL_COLUMN_NAME);
	}

	@Override
	protected List<String> getHeaderColumnsNames()
	{
		return this.headerColumnsNames;
	}

	@Override
	protected DisponibilidadesDisciplinasImportExcelBean createExcelBean(Row header, Row row)
	{
		DisponibilidadesDisciplinasImportExcelBean bean = new DisponibilidadesDisciplinasImportExcelBean(row.getRowNum() + 1);

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

					if (DISCIPLINA_CODIGO_COLUMN_NAME.equals(columnName))
					{
						bean.setDisciplinaCodigoStr(cellValue);
					}
					else if (DIA_SEMANA_COLUMN_NAME.equals(columnName))
					{
						bean.setDiaStr(cellValue);
					}
					else if (HORARIO_INICIAL_COLUMN_NAME.equals(columnName))
					{
						bean.setHorarioInicialStr(cellValue);
					}
					else if (HORARIO_FINAL_COLUMN_NAME.equals(columnName))
					{
						bean.setHorarioFinalStr(cellValue);
					}
				}
			}
		}

		return bean;
	}

	@Override
	public String getSheetName()
	{
		return ExcelInformationType.DISPONIBILIDADES_DISCIPLINAS.getSheetName();
	}

	@Override
	protected String getHeaderToString()
	{
		return this.headerColumnsNames.toString();
	}

	@Override
	protected boolean doSyntacticValidation(List<DisponibilidadesDisciplinasImportExcelBean> sheetContent)
	{
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map<ImportExcelError, List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError, List<Integer>>();

		for (DisponibilidadesDisciplinasImportExcelBean bean : sheetContent)
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
	protected boolean doLogicValidation(List<DisponibilidadesDisciplinasImportExcelBean> sheetContent)
	{
		checkNonRegisteredDisciplina(sheetContent);

		return getErrors().isEmpty();
	}

	private void checkNonRegisteredDisciplina(List<DisponibilidadesDisciplinasImportExcelBean> sheetContent)
	{
		// [ CódigoDisciplina -> Disciplina ]
		Map<String, Disciplina> disciplinasBDMap = Disciplina.buildDisciplinaCodigoToDisciplinaMap(Disciplina.findByCenario(this.instituicaoEnsino, getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (DisponibilidadesDisciplinasImportExcelBean bean : sheetContent)
		{
			Disciplina disciplina = disciplinasBDMap.get(bean.getDisciplinaCodigoStr());
			if (disciplina != null)
			{
				bean.setDisciplina(disciplina);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(DISCIPLINA_CODIGO_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	@Transactional
	@Override
	protected void updateDataBase(List<DisponibilidadesDisciplinasImportExcelBean> sheetContent)
	{
		int count = 0, total = sheetContent.size();
		System.out.print(" " + total);

		Map<Disciplina, List<TriedaTrio<Semanas, Calendar, Calendar>>> disciplinaToDisponibilidadesMap = new HashMap<Disciplina, List<TriedaTrio<Semanas, Calendar, Calendar>>>();
		for (DisponibilidadesDisciplinasImportExcelBean bean : sheetContent)
		{
			TriedaTrio<Semanas, Calendar, Calendar> disponibilidade = TriedaTrio.create(bean.getDiaSemana(), bean.getHorarioInicial(), bean.getHorarioFinal());

			if (disciplinaToDisponibilidadesMap.containsKey(bean.getDisciplina()))
			{
				Boolean concatenouHorario = false;
				for (TriedaTrio<Semanas, Calendar, Calendar> trio : disciplinaToDisponibilidadesMap.get(bean.getDisciplina()))
				{
					if (trio.getTerceiro().equals(disponibilidade.getSegundo()))
					{
						trio.setTerceiro(disponibilidade.getTerceiro());
						concatenouHorario = true;
					}
				}
				if (!concatenouHorario)
					disciplinaToDisponibilidadesMap.get(bean.getDisciplina()).add(disponibilidade);
			}
			else
			{
				List<TriedaTrio<Semanas, Calendar, Calendar>> trios = new ArrayList<TriedaTrio<Semanas, Calendar, Calendar>>();
				trios.add(disponibilidade);
				disciplinaToDisponibilidadesMap.put(bean.getDisciplina(), trios);
			}

			count++;
			total--;
			if (count == 100)
			{
				System.out.println("\t   Faltam " + total + " disponibilidades das disciplinas");
				count = 0;
			}
		}

		if (!disciplinaToDisponibilidadesMap.isEmpty())
		{
			List<SemanaLetiva> semanasLetivas = SemanaLetiva.findByCenario(instituicaoEnsino, getCenario());
			Disciplina.atualizaDisponibilidadesDisciplinas(disciplinaToDisponibilidadesMap, semanasLetivas);
		}
	}

	private void resolveHeaderColumnNames()
	{
		if (DISCIPLINA_CODIGO_COLUMN_NAME == null)
		{
			DISCIPLINA_CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigoDisciplina());
			DIA_SEMANA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().dia());
			HORARIO_INICIAL_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().horarioInicial());
			HORARIO_FINAL_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().horarioFinal());
		}
	}
}
