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
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TriedaTrio;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class DisponibilidadesSalasImportExcel extends AbstractImportExcel<DisponibilidadesSalasImportExcelBean> {
	static public String SALA_CODIGO_COLUMN_NAME;
	static public String DIA_SEMANA_COLUMN_NAME;
	static public String HORARIO_INICIAL_COLUMN_NAME;
	static public String HORARIO_FINAL_COLUMN_NAME;

	private List<String> headerColumnsNames;

	public DisponibilidadesSalasImportExcel(Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
			InstituicaoEnsino instituicaoEnsino) {
		super(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(SALA_CODIGO_COLUMN_NAME);
		this.headerColumnsNames.add(DIA_SEMANA_COLUMN_NAME);
		this.headerColumnsNames.add(HORARIO_INICIAL_COLUMN_NAME);
		this.headerColumnsNames.add(HORARIO_FINAL_COLUMN_NAME);
	}

	@Override
	protected boolean sheetMustBeProcessed(int sheetIndex, Sheet sheet, Workbook workbook) {
		String sheetName = workbook.getSheetName(sheetIndex);
		return ExcelInformationType.DISPONIBILIDADES_SALAS.getSheetName().equals(sheetName);
	}

	@Override
	protected List<String> getHeaderColumnsNames(int sheetIndex, Sheet sheet, Workbook workbook) {
		return this.headerColumnsNames;
	}

	@Override
	protected DisponibilidadesSalasImportExcelBean createExcelBean(Row header, Row row, int sheetIndex, Sheet sheet, Workbook workbook) {
		DisponibilidadesSalasImportExcelBean bean = new DisponibilidadesSalasImportExcelBean(row.getRowNum() + 1);

		for (int cellIndex = row.getFirstCellNum(); cellIndex <= row.getLastCellNum(); cellIndex++) {
			Cell cell = row.getCell(cellIndex);

			if (cell != null) {
				Cell headerCell = header.getCell(cell.getColumnIndex());

				if (headerCell != null) {
					String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue(cell);

					if (SALA_CODIGO_COLUMN_NAME.equals(columnName)) {
						bean.setSalaCodigoStr(cellValue);
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
		return ExcelInformationType.DISPONIBILIDADES_SALAS.getSheetName();
	}

	@Override
	protected String getHeaderToString() {
		return this.headerColumnsNames.toString();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected void processSheetContent(String sheetName, List<DisponibilidadesSalasImportExcelBean> sheetContent) {
		if (doSyntacticValidation(sheetName, sheetContent) && doLogicValidation(sheetName, sheetContent)) {
			updateDataBase(sheetName, sheetContent);
		}
	}

	private boolean doSyntacticValidation(String sheetName, List<DisponibilidadesSalasImportExcelBean> sheetContent) {
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map<ImportExcelError, List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError, List<Integer>>();

		for (DisponibilidadesSalasImportExcelBean bean : sheetContent) {
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

	private boolean doLogicValidation(String sheetName, List<DisponibilidadesSalasImportExcelBean> sheetContent) {
		checkNonRegisteredSala(sheetContent);

		return getErrors().isEmpty();
	}
	
	private void checkNonRegisteredSala(List<DisponibilidadesSalasImportExcelBean> sheetContent) {
		// [ CodigoSala -> Sala ]
		Map<String,Sala> salasBDMap = Sala.buildSalaCodigoToSalaMap(Sala.findByCenario(this.instituicaoEnsino,getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (DisponibilidadesSalasImportExcelBean bean : sheetContent) {
			Sala sala = salasBDMap.get(bean.getSalaCodigoStr());
			if (sala != null) {
				bean.setSala(sala);
			} else {
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty()) {
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(SALA_CODIGO_COLUMN_NAME,rowsWithErrors.toString()));
		}
	}

	@Transactional
	@ProgressReportMethodScan(texto = "Atualizando banco de dados")
	private void updateDataBase(String sheetName, List<DisponibilidadesSalasImportExcelBean> sheetContent) {
		int count = 0, total = sheetContent.size();
		System.out.print(" " + total);

		Map<Sala, List<TriedaTrio<Semanas,Calendar,Calendar>>> salaToDisponibilidadesMap = new HashMap<Sala, List<TriedaTrio<Semanas,Calendar,Calendar>>>();
		for (DisponibilidadesSalasImportExcelBean bean : sheetContent) {
			TriedaTrio<Semanas,Calendar,Calendar> disponibilidade = TriedaTrio.create(bean.getDiaSemana(),bean.getHorarioInicial(),bean.getHorarioFinal());

			if (salaToDisponibilidadesMap.containsKey(bean.getSala())) {
				Boolean concatenouHorario = false;
				for (TriedaTrio<Semanas,Calendar,Calendar> trio : salaToDisponibilidadesMap.get(bean.getSala()) ) {
					if (trio.getTerceiro().equals(disponibilidade.getSegundo())) {
						trio.setTerceiro(disponibilidade.getTerceiro());
						concatenouHorario = true;
					}
				}
				if (!concatenouHorario)
					salaToDisponibilidadesMap.get(bean.getSala()).add(disponibilidade);
			}
			else {
				List<TriedaTrio<Semanas,Calendar,Calendar>> trios = new ArrayList<TriedaTrio<Semanas,Calendar,Calendar>>();
				trios.add(disponibilidade);
				salaToDisponibilidadesMap.put(bean.getSala(), trios);
			}

			count++;
			total--;
			if (count == 100) {
				System.out.println("\t   Faltam " + total + " disponibilidades das salas");
				count = 0;
			}
		}
		
		if (!salaToDisponibilidadesMap.isEmpty()) {
			List<SemanaLetiva> semanasLetivas = SemanaLetiva.findByCenario(instituicaoEnsino, getCenario());
			Sala.atualizaHorariosDasSalas(salaToDisponibilidadesMap,semanasLetivas);
		}
	}

	private void resolveHeaderColumnNames() {
		if (SALA_CODIGO_COLUMN_NAME == null) {
			SALA_CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigoSala());
			DIA_SEMANA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().dia());
			HORARIO_INICIAL_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().horarioInicial());
			HORARIO_FINAL_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().horarioFinal());
		}
	}
}

