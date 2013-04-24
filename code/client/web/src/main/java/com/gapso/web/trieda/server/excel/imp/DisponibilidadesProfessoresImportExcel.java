package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TriedaTrio;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class DisponibilidadesProfessoresImportExcel extends AbstractImportExcel<DisponibilidadesProfessoresImportExcelBean> {
	static public String CPF_COLUMN_NAME;
	static public String DIA_SEMANA_COLUMN_NAME;
	static public String HORARIO_INICIAL_COLUMN_NAME;
	static public String HORARIO_FINAL_COLUMN_NAME;

	private List<String> headerColumnsNames;

	public DisponibilidadesProfessoresImportExcel(Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
			InstituicaoEnsino instituicaoEnsino) {
		super(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(CPF_COLUMN_NAME);
		this.headerColumnsNames.add(DIA_SEMANA_COLUMN_NAME);
		this.headerColumnsNames.add(HORARIO_INICIAL_COLUMN_NAME);
		this.headerColumnsNames.add(HORARIO_FINAL_COLUMN_NAME);
	}

	@Override
	protected boolean sheetMustBeProcessed(int sheetIndex, Sheet sheet, Workbook workbook) {
		String sheetName = workbook.getSheetName(sheetIndex);
		return ExcelInformationType.DISPONIBILIDADES_PROFESSORES.getSheetName().equals(sheetName);
	}

	@Override
	protected List<String> getHeaderColumnsNames(int sheetIndex, Sheet sheet, Workbook workbook) {
		return this.headerColumnsNames;
	}

	@Override
	protected DisponibilidadesProfessoresImportExcelBean createExcelBean(Row header, Row row, int sheetIndex, Sheet sheet, Workbook workbook) {
		DisponibilidadesProfessoresImportExcelBean bean = new DisponibilidadesProfessoresImportExcelBean(row.getRowNum() + 1);

		for (int cellIndex = row.getFirstCellNum(); cellIndex <= row.getLastCellNum(); cellIndex++) {
			Cell cell = row.getCell(cellIndex);

			if (cell != null) {
				Cell headerCell = header.getCell(cell.getColumnIndex());

				if (headerCell != null) {
					String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue(cell);

					if (CPF_COLUMN_NAME.equals(columnName)) {
						bean.setCpfStr(cellValue);
					} else if (DIA_SEMANA_COLUMN_NAME.equals(columnName)) {
						bean.setDiaStr(cellValue);
					} else if (HORARIO_INICIAL_COLUMN_NAME.equals(columnName)) {
						bean.setHorarioInicialStr(cellValue);
					} else if (HORARIO_FINAL_COLUMN_NAME.equals(columnName)) {
						bean.setHorarioFinalStr(cellValue);
					}
				}
			}
		}

		return bean;
	}

	@Override
	public String getSheetName() {
		return ExcelInformationType.DISPONIBILIDADES_PROFESSORES.getSheetName();
	}

	@Override
	protected String getHeaderToString() {
		return this.headerColumnsNames.toString();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected void processSheetContent(String sheetName, List<DisponibilidadesProfessoresImportExcelBean> sheetContent) {
		if (doSyntacticValidation(sheetName, sheetContent) && doLogicValidation(sheetName, sheetContent)) {
			updateDataBase(sheetName, sheetContent);
		}
	}

	private boolean doSyntacticValidation(String sheetName, List<DisponibilidadesProfessoresImportExcelBean> sheetContent) {
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map<ImportExcelError, List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError, List<Integer>>();

		for (DisponibilidadesProfessoresImportExcelBean bean : sheetContent) {
			List<ImportExcelError> errorsBean = bean.checkSyntacticErrors();
			for (ImportExcelError error : errorsBean) {
				List<Integer> rowsWithErrors = syntacticErrorsMap.get(error);
				if (rowsWithErrors == null) {
					rowsWithErrors = new ArrayList<Integer>();
					syntacticErrorsMap.put(error, rowsWithErrors);
				}
				rowsWithErrors.add(bean.getRow());
			}
		}

		// Coleta os erros e adiciona os mesmos na lista de mensagens
		for (ImportExcelError error : syntacticErrorsMap.keySet()) {
			List<Integer> linhasComErro = syntacticErrorsMap.get(error);
			getErrors().add(error.getMessage(linhasComErro.toString(),getI18nMessages()));
		}

		return syntacticErrorsMap.isEmpty();
	}

	private boolean doLogicValidation(String sheetName, List<DisponibilidadesProfessoresImportExcelBean> sheetContent) {
		checkNonRegisteredProfessor(sheetContent);

		return getErrors().isEmpty();
	}
	
	private void checkNonRegisteredProfessor(List<DisponibilidadesProfessoresImportExcelBean> sheetContent) {
		// [ CpfProfessor -> Professor ]
		Map<String,Professor> professoresBDMap = Professor.buildProfessorCpfToProfessorMap(Professor.findByCenario(this.instituicaoEnsino,getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (DisponibilidadesProfessoresImportExcelBean bean : sheetContent) {
			Professor professor = professoresBDMap.get(bean.getCpfStr());
			if (professor != null) {
				bean.setProfessor(professor);
			} else {
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty()) {
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(CPF_COLUMN_NAME,rowsWithErrors.toString()));
		}
	}

	@Transactional
	@ProgressReportMethodScan(texto = "Atualizando banco de dados")
	private void updateDataBase(String sheetName, List<DisponibilidadesProfessoresImportExcelBean> sheetContent) {
		int count = 0, total = sheetContent.size();
		System.out.print(" " + total);

		Map<Professor, List<TriedaTrio<Semanas,Calendar,Calendar>>> professoreToDisponibilidadesMap = new HashMap<Professor, List<TriedaTrio<Semanas,Calendar,Calendar>>>();
		for (DisponibilidadesProfessoresImportExcelBean bean : sheetContent) {
			TriedaTrio<Semanas,Calendar,Calendar> disponibilidade = TriedaTrio.create(bean.getDiaSemana(),bean.getHorarioInicial(),bean.getHorarioFinal());

			if (professoreToDisponibilidadesMap.containsKey(bean.getProfessor())) {
				Boolean concatenouHorario = false;
				for (TriedaTrio<Semanas,Calendar,Calendar> trio : professoreToDisponibilidadesMap.get(bean.getProfessor()) ) {
					if (trio.getTerceiro().equals(disponibilidade.getSegundo())) {
						trio.setTerceiro(disponibilidade.getTerceiro());
						concatenouHorario = true;
					}
				}
				if (!concatenouHorario)
					professoreToDisponibilidadesMap.get(bean.getProfessor()).add(disponibilidade);
			}
			else {
				List<TriedaTrio<Semanas,Calendar,Calendar>> trios = new ArrayList<TriedaTrio<Semanas,Calendar,Calendar>>();
				trios.add(disponibilidade);
				professoreToDisponibilidadesMap.put(bean.getProfessor(), trios);
			}

			count++;
			total--;
			if (count == 100) {
				System.out.println("\t   Faltam " + total + " disponibilidades de professores");
				count = 0;
			}
		}
		
		if (!professoreToDisponibilidadesMap.isEmpty()) {
			List<SemanaLetiva> semanasLetivas = SemanaLetiva.findAll(instituicaoEnsino);
			Professor.atualizaHorariosDosProfessores(professoreToDisponibilidadesMap,semanasLetivas);
		}
	}

	private void resolveHeaderColumnNames() {
		if (CPF_COLUMN_NAME == null) {
			CPF_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().cpf());
			DIA_SEMANA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().dia());
			HORARIO_INICIAL_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().horarioInicial());
			HORARIO_FINAL_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().horarioFinal());
		}
	}
}
