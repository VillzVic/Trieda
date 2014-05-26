package com.gapso.web.trieda.server.excel.imp;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class SemanaLetivaHorariosImportExcel extends AbstractImportExcel<SemanaLetivaHorariosImportExcelBean> {
	static public String SEMANA_LETIVA_CODIGO_COLUMN_NAME;
	static public String TURNO_COLUMN_NAME;
	static public String HORARIO_COLUMN_NAME;
	static public String SEGUNDA_COLUMN_NAME;
	static public String TERCA_COLUMN_NAME;
	static public String QUARTA_COLUMN_NAME;
	static public String QUINTA_COLUMN_NAME;
	static public String SEXTA_COLUMN_NAME;
	static public String SABADO_COLUMN_NAME;
	static public String DOMINGO_COLUMN_NAME;

	private List<String> headerColumnsNames;
	boolean continuaProcessamento = true;

	public SemanaLetivaHorariosImportExcel(Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
			InstituicaoEnsino instituicaoEnsino) {
		super(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(SEMANA_LETIVA_CODIGO_COLUMN_NAME);
		this.headerColumnsNames.add(TURNO_COLUMN_NAME);
		this.headerColumnsNames.add(HORARIO_COLUMN_NAME);
		this.headerColumnsNames.add(SEGUNDA_COLUMN_NAME);
		this.headerColumnsNames.add(TERCA_COLUMN_NAME);
		this.headerColumnsNames.add(QUARTA_COLUMN_NAME);
		this.headerColumnsNames.add(QUINTA_COLUMN_NAME);
		this.headerColumnsNames.add(SEXTA_COLUMN_NAME);
		this.headerColumnsNames.add(SABADO_COLUMN_NAME);
		this.headerColumnsNames.add(DOMINGO_COLUMN_NAME);
	}

	@Override
	protected boolean sheetMustBeProcessed(int sheetIndex, Sheet sheet, Workbook workbook) {
		String sheetName = workbook.getSheetName(sheetIndex);
		return ExcelInformationType.SEMANA_LETIVA_HORARIOS.getSheetName().equals(sheetName);
	}

	@Override
	protected List<String> getHeaderColumnsNames(int sheetIndex, Sheet sheet, Workbook workbook) {
		return this.headerColumnsNames;
	}

	@Override
	protected SemanaLetivaHorariosImportExcelBean createExcelBean(Row header, Row row, int sheetIndex, Sheet sheet, Workbook workbook) {
		SemanaLetivaHorariosImportExcelBean bean = new SemanaLetivaHorariosImportExcelBean(row.getRowNum() + 1);

		for (int cellIndex = row.getFirstCellNum(); cellIndex <= row.getLastCellNum(); cellIndex++) {
			Cell cell = row.getCell(cellIndex);

			if (cell != null) {
				Cell headerCell = header.getCell(cell.getColumnIndex());

				if (headerCell != null) {
					String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue(cell);

					if (SEMANA_LETIVA_CODIGO_COLUMN_NAME.equals(columnName)) {
						bean.setSemanaLetivaCodigoStr(cellValue);
					} else if (TURNO_COLUMN_NAME.equals(columnName)) {
						bean.setTurnoNomeStr(cellValue);
					} else if (HORARIO_COLUMN_NAME.equals(columnName)) {
						bean.setHorarioStr(cellValue);
					} else if (SEGUNDA_COLUMN_NAME.equals(columnName)) {
						bean.setSegundaStr(cellValue);
					} else if (TERCA_COLUMN_NAME.equals(columnName)) {
						bean.setTercaStr(cellValue);
					} else if (QUARTA_COLUMN_NAME.equals(columnName)) {
						bean.setQuartaStr(cellValue);
					} else if (QUINTA_COLUMN_NAME.equals(columnName)) {
						bean.setQuintaStr(cellValue);
					} else if (SEXTA_COLUMN_NAME.equals(columnName)) {
						bean.setSextaStr(cellValue);
					} else if (SABADO_COLUMN_NAME.equals(columnName)) {
						bean.setSabadoStr(cellValue);
					} else if (DOMINGO_COLUMN_NAME.equals(columnName)) {
						bean.setDomingoStr(cellValue);
					}
				}
			}
		}

		return bean;
	}

	@Override
	public String getSheetName() {
		return ExcelInformationType.SEMANA_LETIVA_HORARIOS.getSheetName();
	}

	@Override
	protected String getHeaderToString() {
		return this.headerColumnsNames.toString();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected void processSheetContent(String sheetName, List<SemanaLetivaHorariosImportExcelBean> sheetContent) {
		if (doSyntacticValidation(sheetName, sheetContent) && doLogicValidation(sheetName, sheetContent)) {
			updateDataBase(sheetName, sheetContent);
		}
	}

	private boolean doSyntacticValidation(String sheetName, List<SemanaLetivaHorariosImportExcelBean> sheetContent) {
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map<ImportExcelError, List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError, List<Integer>>();

		for (SemanaLetivaHorariosImportExcelBean bean : sheetContent) {
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

	private boolean doLogicValidation(String sheetName, List<SemanaLetivaHorariosImportExcelBean> sheetContent) {
		checkNonRegisteredSemanaLetiva(sheetContent);
		checkNonRegisteredTurno(sheetContent);
		checkIntervaloHorarios(sheetContent);

		return getErrors().isEmpty();
	}
	
	private void checkNonRegisteredSemanaLetiva(List<SemanaLetivaHorariosImportExcelBean> sheetContent) {
		// [ CodigoSala -> Sala ]
		Map<String,SemanaLetiva> semanaLetivaBDMap = SemanaLetiva.buildSemanaLetivaCodigoToSemanaLetivaMap(SemanaLetiva.findByCenario(this.instituicaoEnsino,getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (SemanaLetivaHorariosImportExcelBean bean : sheetContent) {
			SemanaLetiva semanaLetiva = semanaLetivaBDMap.get(bean.getSemanaLetivaCodigoStr());
			if (semanaLetiva != null) {
				bean.setSemanaLetiva(semanaLetiva);
			} else {
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty()) {
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(SEMANA_LETIVA_CODIGO_COLUMN_NAME,rowsWithErrors.toString()));
		}
	}
	
	private void checkNonRegisteredTurno(List<SemanaLetivaHorariosImportExcelBean> sheetContent) {
		// [ CodigoSala -> Sala ]
		Map<String,Turno> turnoBDMap = Turno.buildTurnoNomeToTurnoMap(Turno.findByCenario(this.instituicaoEnsino,getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (SemanaLetivaHorariosImportExcelBean bean : sheetContent) {
			Turno turno = turnoBDMap.get(bean.getTurnoNomeStr());
			if (turno != null) {
				bean.setTurno(turno);
			} else {
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty()) {
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(TURNO_COLUMN_NAME,rowsWithErrors.toString()));
		}
	}
	
	private void checkIntervaloHorarios(List<SemanaLetivaHorariosImportExcelBean> sheetContent) 
	{
		Map<SemanaLetiva, Map<Turno, List<Calendar>>> semanaLetivaTurnoMapHorario = new HashMap<SemanaLetiva, Map<Turno, List<Calendar>>>();
		String horariosComErros = "";
		SimpleDateFormat sdf = new SimpleDateFormat("HH:mm");
		for (SemanaLetivaHorariosImportExcelBean bean : sheetContent) 
		{
			if (semanaLetivaTurnoMapHorario.get(bean.getSemanaLetiva()) == null)
			{
				
				Map<Turno, List<Calendar>> turnoMapHorarios = new HashMap<Turno, List<Calendar>>();
				List<Calendar> horarios = new ArrayList<Calendar>();
				horarios.add(bean.getHorario());
				
				turnoMapHorarios.put(bean.getTurno(), horarios);
				
				semanaLetivaTurnoMapHorario.put(bean.getSemanaLetiva(),turnoMapHorarios);
			}
			else
			{
				if(semanaLetivaTurnoMapHorario.get(bean.getSemanaLetiva()).get(bean.getTurno()) == null){
					Map<Turno, List<Calendar>> turnoMapHorarios = new HashMap<Turno, List<Calendar>>();
					List<Calendar> horarios = new ArrayList<Calendar>();
					horarios.add(bean.getHorario());
					turnoMapHorarios.put(bean.getTurno(), horarios);
					semanaLetivaTurnoMapHorario.put(bean.getSemanaLetiva(),turnoMapHorarios);
				} else {
					semanaLetivaTurnoMapHorario.get(bean.getSemanaLetiva()).get(bean.getTurno()).add(bean.getHorario());
				}
			}
		}
		for (SemanaLetiva key : semanaLetivaTurnoMapHorario.keySet())
		{
			
			for(Turno turnoKey : semanaLetivaTurnoMapHorario.get(key).keySet()){
			
				List<Calendar> horariosOrdenados = new ArrayList<Calendar>();
				horariosOrdenados.addAll(semanaLetivaTurnoMapHorario.get(key).get(turnoKey));
				
				Collections.sort(horariosOrdenados);
				
				for (int i = 1; i<horariosOrdenados.size(); i++)
				{
					if ((horariosOrdenados.get(i).getTimeInMillis() - horariosOrdenados.get(i-1).getTimeInMillis()) < (key.getTempo() * 60000))
					{
						horariosComErros += " \"" + sdf.format(horariosOrdenados.get(i-1).getTime()) + "-" + sdf.format(horariosOrdenados.get(i).getTime()) + "\"";
					}
				}
			
			}
		}
		
		if (!horariosComErros.isEmpty())
		{
			getErrors().add("Os horarios" + horariosComErros + " tem a diferença menor que o tempo da semana letiva");
		}
	}

	@Transactional
	@ProgressReportMethodScan(texto = "Atualizando banco de dados")
	private void updateDataBase(String sheetName, List<SemanaLetivaHorariosImportExcelBean> sheetContent) {
		Set<SemanaLetiva> semanasLetivasAtualizadas = new HashSet<SemanaLetiva>();
		for (SemanaLetivaHorariosImportExcelBean bean : sheetContent)
		{
			semanasLetivasAtualizadas.add(bean.getSemanaLetiva());
		}
		for (SemanaLetiva semanaLetiva : semanasLetivasAtualizadas )
		{
			for (HorarioAula horario : semanaLetiva.getHorariosAula())
			{
				horario.remove();
			}
			semanaLetiva.getHorariosAula().clear();
		}
		for (SemanaLetivaHorariosImportExcelBean bean : sheetContent) {
			HorarioAula newHorario = new HorarioAula();
			
			newHorario.setSemanaLetiva(bean.getSemanaLetiva());
			newHorario.setTurno(bean.getTurno());
			newHorario.setHorario(bean.getHorario().getTime());
			newHorario.persist();
			bean.getSemanaLetiva().getHorariosAula().add(newHorario);
			bean.getSemanaLetiva().merge();
			
			List< Campus > campi = Campus.findByCenario( instituicaoEnsino, cenario );
			List< Unidade > unidades = Unidade.findByCenario( instituicaoEnsino, cenario );
			List< Sala > salas = Sala.findByCenario( instituicaoEnsino, cenario );
			List< Disciplina > disciplinas = Disciplina.findByCenario( instituicaoEnsino, cenario );
			List< Professor > professores = Professor.findByCenario( instituicaoEnsino, cenario );

			for ( Semanas semana : Semanas.values() )
			{
				
				if(checkWeekDayEnable(semana,bean))
				{
					HorarioDisponivelCenario hdc = new HorarioDisponivelCenario();
	
					hdc.setDiaSemana( semana );
					hdc.setHorarioAula( newHorario );
	
					hdc.getCampi().addAll( campi );
					hdc.getUnidades().addAll( unidades );
					hdc.getSalas().addAll( salas );
					hdc.getDisciplinas().addAll( disciplinas );
					hdc.getProfessores().addAll( professores );
	
					hdc.persist();
					newHorario.getHorariosDisponiveisCenario().add(hdc);
				}
			}
			newHorario.merge();
		}
	}

	private boolean checkWeekDayEnable(Semanas semana,
			SemanaLetivaHorariosImportExcelBean bean) {
		
		boolean output = false;
		switch (semana) {
		case SEG:
			output = bean.getSegunda();
			break;
		case TER:
			output = bean.getTerca();
			break;
		case QUA:
			output = bean.getQuarta();
			break;
		case QUI:
			output = bean.getQuinta();
			break;
		case SEX:
			output = bean.getSexta();
			break;
		case SAB:
			output = bean.getSabado();
			break;
		case DOM:
			output = bean.getDomingo();
			break;
		}
		return output;
	}

	private void resolveHeaderColumnNames() {
		if (SEMANA_LETIVA_CODIGO_COLUMN_NAME == null) {
			SEMANA_LETIVA_CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape("Código Semana Letiva");
			TURNO_COLUMN_NAME = HtmlUtils.htmlUnescape("Turno");
			HORARIO_COLUMN_NAME = HtmlUtils.htmlUnescape("Horário Inicial");
			SEGUNDA_COLUMN_NAME = HtmlUtils.htmlUnescape("Segunda");
			TERCA_COLUMN_NAME = HtmlUtils.htmlUnescape("Terça");
			QUARTA_COLUMN_NAME = HtmlUtils.htmlUnescape("Quarta");
			QUINTA_COLUMN_NAME = HtmlUtils.htmlUnescape("Quinta");
			SEXTA_COLUMN_NAME = HtmlUtils.htmlUnescape("Sexta");
			SABADO_COLUMN_NAME = HtmlUtils.htmlUnescape("Sábado");
			DOMINGO_COLUMN_NAME = HtmlUtils.htmlUnescape("Domingo");
		}
	}
}
