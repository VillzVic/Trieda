package com.gapso.web.trieda.server.excel.exp;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.poi.hssf.util.HSSFColor;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Font;
import org.apache.poi.ss.usermodel.IndexedColors;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.DeslocamentoUnidade;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.TriedaServerUtil;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHoraria;

@ProgressDeclarationAnnotation
public abstract class RelatorioVisaoExportExcel extends AbstractExportExcel{
	enum ExcelCellStyleReference{
		HEADER_LEFT_TEXT(5, 3),
		HEADER_CENTER_VALUE(5, 4),
		HEADER_CENTER_TEXT(7, 2),
		TEXT(8, 2);

		private int row;
		private int col;

		private ExcelCellStyleReference(int row, int col){
			this.row = row;
			this.col = col;
		}

		public int getRow(){
			return row;
		}

		public int getCol(){
			return col;
		}
		
	}
	
	protected CellStyle [] cellStyles;
	protected Sheet sheet;
	protected Sheet templateSheet;
	protected Map<Long,CellStyle> codigoDisciplinaToColorMap;
	protected List<CellStyle> excelColorsPool;
	protected boolean removeUnusedSheets;
	protected int initialRow;
	protected Set<String> horarioInicioAula = new HashSet<String>();
	protected Set<String> horarioFimAula = new HashSet<String>();
	protected List<String> labelsDasLinhasDaGradeHoraria;
	protected List<Boolean> horarioEhIntervalo = new ArrayList<Boolean>();
	protected Map<String, Turno> horarioToTurnoMap = new HashMap<String, Turno>();
	protected Map<String, Double> origemDestinoCampusToTempoDeslocamentoMap = null;
	protected Map<String, Double> origemDestinoUnidadeToTempoDeslocamentoMap = null;
	
	public RelatorioVisaoExportExcel(boolean removeUnusedSheets, Cenario cenario, 
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		ExportExcelFilter filter, InstituicaoEnsino instituicaoEnsino,
		Map<String, Double> origemDestinoCampusToTempoDeslocamentoMap,
		Map<String, Double> origemDestinoUnidadeToTempoDeslocamentoMap,
		String fileExtension)
	{
		super(false, null, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
		this.setSheetName(getReportSheetName());
		
		this.cellStyles = new CellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.codigoDisciplinaToColorMap = new HashMap<Long,CellStyle>();
		this.initialRow = 5;
		this.origemDestinoCampusToTempoDeslocamentoMap = origemDestinoCampusToTempoDeslocamentoMap;
		this.origemDestinoUnidadeToTempoDeslocamentoMap = origemDestinoUnidadeToTempoDeslocamentoMap;
		
		this.setFilter(filter);
		this.preencheMapHorariosTurnos(cenario);
	}
	
	protected void preencheMapHorariosTurnos(Cenario cenario) {
		Map<String, Map<Turno, Integer>> horarioToTurnoToQtdMap = new HashMap<String, Map<Turno, Integer>>();
		for (SemanaLetiva sl : cenario.getSemanasLetivas()) {
			for (HorarioAula ha : sl.getHorariosAula()) {				
				Calendar hi = TriedaServerUtil.dateToCalendar(ha.getHorario());
				Calendar hf = Calendar.getInstance();
				hf.clear();
				hf.setTime(hi.getTime());
				hf.add(Calendar.MINUTE,sl.getTempo());												
				
				String hiStr = TriedaUtil.shortTimeString(hi.getTime());
				String hfStr = TriedaUtil.shortTimeString(hf.getTime());
				
				Turno turno = ha.getTurno();
				
				Map<Turno, Integer> turnoToQtdMap = horarioToTurnoToQtdMap.get(hiStr);
				if (turnoToQtdMap == null) {
					turnoToQtdMap = new HashMap<Turno, Integer>();
					horarioToTurnoToQtdMap.put(hiStr, turnoToQtdMap);
				}
				Integer qtd = turnoToQtdMap.get(turno);
				if (qtd == null) {
					qtd = 0;
				}
				turnoToQtdMap.put(turno, qtd+1);
				
				turnoToQtdMap = horarioToTurnoToQtdMap.get(hfStr);
				if (turnoToQtdMap == null) {
					turnoToQtdMap = new HashMap<Turno, Integer>();
					horarioToTurnoToQtdMap.put(hfStr, turnoToQtdMap);
				}
				qtd = turnoToQtdMap.get(turno);
				if (qtd == null) {
					qtd = 0;
				}
				turnoToQtdMap.put(turno, qtd+1);
			}
		}
		
		for (Entry<String, Map<Turno, Integer>> entry1 : horarioToTurnoToQtdMap.entrySet()) {
			String horario = entry1.getKey();
			Turno turnoSelecionado = null;
			int qtdOcorrenciasTurnoSelecionado = 0;
			for (Entry<Turno, Integer> entry2 : entry1.getValue().entrySet()) {
				Turno turno = entry2.getKey();
				int qtdOcorrenciasTurno = entry2.getValue();
				if (qtdOcorrenciasTurno > qtdOcorrenciasTurnoSelecionado) {
					turnoSelecionado = turno;
					qtdOcorrenciasTurnoSelecionado = qtdOcorrenciasTurno;
				}
			}
			this.horarioToTurnoMap.put(horario, turnoSelecionado);
		}
	}
	
	protected abstract String getReportSheetName();
	protected abstract String getReportEntity();
	
	@Override
	protected String getPathExcelTemplate(){
		if ( fileExtension.equals("xlsx") )
		{
			return "/templateExport.xlsx";
		}
		else
			return "/templateExport.xls";
	}
	
	private String getRelatorioVisaoName(){
		String str = "";
		try {
			Method m = this.getI18nConstants().getClass().getMethod("relatorioVisao" + getReportEntity());
			str = (String) m.invoke(this.getI18nConstants());
		}
		catch(Exception e){
			e.printStackTrace();
		}
		
		return str;
	}
	
	@Override
	public String getFileName(){
		return getRelatorioVisaoName();
	}
	
	@Override
	protected String getReportName(){
		return getRelatorioVisaoName();
	}
	
	protected abstract ExportExcelFilter getFilter();
	protected abstract void setFilter(ExportExcelFilter filter);
	protected abstract <T> T getStructureReportControl();
	protected abstract <T> boolean getAtendimentosRelatorioDTOList(T structureControl);
	protected abstract <T> void processStructureReportControl(T structureControl);
	
	protected void onWriteAula(int row, int col, AtendimentoRelatorioDTO aula) {
		
	}

	protected <T> boolean fillInExcelImpl(Workbook workbook){
		boolean result = false;
		
		T structureControl = this.<T>getStructureReportControl();
		
		if(getAtendimentosRelatorioDTOList(structureControl)){
			sheet = workbook.getSheet(this.getSheetName());
			
			// monta estruturas de estilos
			fillInCellStyles(sheet);
			buildColorPaletteCellStyles(workbook);
			
			processStructureReportControl(structureControl);
			
			result = true;
			
			/*if (isXls())
				autoSizeColumns((short)1,(short)1,sheet);*/
		}
		
		if(this.removeUnusedSheets) removeUnusedSheets(this.getSheetName(), workbook);

		return result;
	}
	
	protected String getHorarioFinalAula(AtendimentoRelatorioDTO aula)
	{
		String[] horarioInicialArray = aula.getHorarioAulaString().split(":");
		int horarioInicialHoras = Integer.parseInt(horarioInicialArray[0]);
		int horarioInicialMinutos = Integer.parseInt(horarioInicialArray[1]);
		
		
		int horarioFinalHoras = horarioInicialHoras;
		int horarioFinalMinutos = horarioInicialMinutos + (aula.getDuracaoDeUmaAulaEmMinutos() * aula.getTotalCreditos());
		while (horarioFinalMinutos >= 60)
		{
			horarioFinalHoras++;
			horarioFinalMinutos -= 60;
		}
		//System.out.println("HorarioFInal: " + String.format("%02d", horarioFinalHoras) + ":" + String.format("%02d", horarioFinalMinutos));
		return (String.format("%02d", horarioFinalHoras) + ":" + String.format("%02d", horarioFinalMinutos));
	}
	
	protected boolean isHorarioIntervalo(String horario)
	{
		for (int i = 0; i<labelsDasLinhasDaGradeHoraria.size(); i++)
		{
			if (labelsDasLinhasDaGradeHoraria.get(i).contains(horario))
				return horarioEhIntervalo.get(i);
		}
		
		return false;
	}
	
	protected int getIndexHorario(String horario, List<String> labels)
	{
		for (String label : labels)
		{
			//System.out.println("substring: " + label.substring(0, 5) + "-" + horario);
			if (label.substring(0, 5).equals(horario))
			{
			//	System.out.println("returning: " + labels.indexOf(label));
				return labels.indexOf(label);
			}
		}
		//System.out.println("nao achou returning: " + (labels.size() -1));
		return labels.size();
	}
	
	protected int writeAulas(List<AtendimentoRelatorioDTO> aulas, int row, int mdcTemposAula, boolean temInfoDeHorarios, List<String> horariosDaGradeHoraria, List<String> horariosDeInicioDeAula, List<String> horariosDeFimDeAula) {
		if (this.origemDestinoUnidadeToTempoDeslocamentoMap != null) {
			return writeAulasComBuracos(aulas, row, mdcTemposAula, temInfoDeHorarios, horariosDaGradeHoraria, horariosDeInicioDeAula, horariosDeFimDeAula);
		}
		return writeAulasSemBuracos(aulas, row, mdcTemposAula, temInfoDeHorarios, horariosDaGradeHoraria, horariosDeInicioDeAula, horariosDeFimDeAula);
	}
	
	protected int writeAulasSemBuracos(List<AtendimentoRelatorioDTO> aulas, int row, int mdcTemposAula, boolean temInfoDeHorarios, List<String> horariosDaGradeHoraria, List<String> horariosDeInicioDeAula, List<String> horariosDeFimDeAula) {
		if (temInfoDeHorarios) { 
			TrioDTO<List<String>,List<String>, List<Boolean>> parDTO = GradeHoraria.processaLabelsDasLinhasDaGradeHoraria(horariosDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula);
			labelsDasLinhasDaGradeHoraria = parDTO.getPrimeiro();
			horarioEhIntervalo = parDTO.getTerceiro();
		} else {
			labelsDasLinhasDaGradeHoraria = horariosDaGradeHoraria;
		}
		
		// TODO: Utilizar ideia abaixo para generalizar impressão de mais de 65536 linhas em extensão XLS
		// verifica se o max de linhas será extrapolado
		if (isXls()){
			Sheet newSheet = restructuringWorkbookIfRowLimitIsViolated(row,(labelsDasLinhasDaGradeHoraria.size()+12),sheet);
			if (newSheet != null) {
				System.out.println("RestructuringWorkbook");
				row = this.initialRow;
				sheet = newSheet;
			}
		}
//		if ((row + labelsDasLinhasDaGradeHoraria.size() + 12) >= 65536) {
//			//autoSizeColumns((short)1,(short)1,sheet);
//			
//			Workbook workbook = sheet.getWorkbook();
//			Sheet newSheet = workbook.createSheet(sheet.getSheetName()+"1");
//			sheet = newSheet;
//			drawing = newSheet.createDrawingPatriarch();
//			
//			row = this.initialRow;
//		}
		
		int initialRow = row;
		int col = 2;

		// agrupa as aulas por dia da semana e coleta disciplinas
		horarioInicioAula.clear();
		horarioFimAula.clear();
		Map<Integer,List<AtendimentoRelatorioDTO>> colunaGradeHorariaToAulasMap = new HashMap<Integer,List<AtendimentoRelatorioDTO>>();
		for(AtendimentoRelatorioDTO aula : aulas){
			List<AtendimentoRelatorioDTO> aulasDoDia = colunaGradeHorariaToAulasMap.get(aula.getSemana());
			if(aulasDoDia == null){
				aulasDoDia = new ArrayList<AtendimentoRelatorioDTO>();
				colunaGradeHorariaToAulasMap.put(aula.getSemana(), aulasDoDia);
			}
			aulasDoDia.add(aula);
			
			String[] horarioInicialArray = aula.getHorarioAulaString().split(":");
			int horarioInicialHoras = Integer.parseInt(horarioInicialArray[0]);
			int horarioInicialMinutos = Integer.parseInt(horarioInicialArray[1]);
			
			
			horarioInicioAula.add(aula.getHorarioAulaString());
			for (int i = 1; i <= aula.getTotalCreditos() ; i++)
			{
				int horarioFinalHoras = horarioInicialHoras;
				int horarioFinalMinutos = horarioInicialMinutos + (aula.getDuracaoDeUmaAulaEmMinutos() * i);
				while (horarioFinalMinutos >= 60)
				{
					horarioFinalHoras++;
					horarioFinalMinutos -= 60;
				}
				horarioFimAula.add(String.format("%02d", horarioFinalHoras) + ":" + String.format("%02d", horarioFinalMinutos));
			}
		}

		// preenche grade vazia
		int rowAdd = 0;
		List<String> horariosEscritos = new ArrayList<String>();
		if (!labelsDasLinhasDaGradeHoraria.isEmpty())
		{
			String horarioASerEscrito = labelsDasLinhasDaGradeHoraria.get(0).substring(0, 5);
			for (int i = 1; i < labelsDasLinhasDaGradeHoraria.size(); i++) {
				if (horarioInicioAula.contains(labelsDasLinhasDaGradeHoraria.get(i).substring(0, 5)) || horarioFimAula.contains(labelsDasLinhasDaGradeHoraria.get(i).substring(0, 5)))
				{
					horarioASerEscrito += " / " + labelsDasLinhasDaGradeHoraria.get(i).substring(0, 5);
					// coluna de carga horária
					if (!isHorarioIntervalo(horarioASerEscrito))
						setCell((row + rowAdd), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], horarioASerEscrito);
					else
					{
						setCell((row + rowAdd), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], "");
						getRow((row + rowAdd -1), sheet).setHeight((short)200);
					}
					horariosEscritos.add(horarioASerEscrito);
					// colunas dos dias da semana
					for(int j = 0; j < Semanas.values().length; j++) {
						setCell((row + rowAdd), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], "");
					}
					col = 2;
					rowAdd++;
					horarioASerEscrito = labelsDasLinhasDaGradeHoraria.get(i).substring(0, 5);
				}
			}
			horarioASerEscrito += " / " + labelsDasLinhasDaGradeHoraria.get(labelsDasLinhasDaGradeHoraria.size() -1).substring(8, 13);
			setCell((row + rowAdd), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], horarioASerEscrito);
			horariosEscritos.add(horarioASerEscrito);
			for(int j = 0; j < Semanas.values().length; j++) {
				setCell((row + rowAdd), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], "");
			}
		}
		

		// para cada dia da semana, escreve as aulas no excel
		for (Integer colunaGradeHoraria : colunaGradeHorariaToAulasMap.keySet()) {
			Semanas diaSemana = Semanas.get(colunaGradeHoraria);

			// linha em que a escrita será iniciada
			row = initialRow;
			// coluna em que a escrita será iniciada
			col = (diaSemana == Semanas.DOM) ? 9 : colunaGradeHoraria + 1;
			
			List<AtendimentoRelatorioDTO> aulasDoDia = colunaGradeHorariaToAulasMap.get(colunaGradeHoraria);
			if(aulasDoDia == null || aulasDoDia.isEmpty()) continue;

			// para cada aula
			for (AtendimentoRelatorioDTO aula : aulasDoDia) {
				// obtém o estilo que será aplicado nas células que serão desenhadas
				CellStyle style = getCellStyle(aula);
				// obtém a qtd de linhas que devem ser desenhadas para cada crédito da aula em questão
				int linhasDeExcelPorCreditoDaAula = aula.getDuracaoDeUmaAulaEmMinutos() / mdcTemposAula;
				
				if (temInfoDeHorarios) {
					int index = getIndexHorario(aula.getHorarioAulaString(), horariosEscritos);//int index = horariosDeInicioDeAula.indexOf(aula.getHorarioAulaString());
					if (index != -1) {
						row = initialRow + index;
					}
					linhasDeExcelPorCreditoDaAula = getIndexHorario(getHorarioFinalAula(aula), horariosEscritos) - index - 1;
				}

				String contentString = "", contentToolTipString = "";
				try{
					Method m = aula.getClass().getMethod("getContentVisao" + getReportEntity(), ReportType.class);
					contentString = (String) m.invoke(aula, ReportType.EXCEL);
					
					m = aula.getClass().getMethod("getContentToolTipVisao" + getReportEntity(), ReportType.class);
					contentToolTipString = (String) m.invoke(aula, ReportType.EXCEL);
				}
				catch(Exception e){
					e.printStackTrace();
				}
				// escreve célula principal
				setCell(row, col, sheet, style, HtmlUtils.htmlUnescape(contentString), HtmlUtils.htmlUnescape(contentToolTipString));
				// une células de acordo com a quantidade de créditos da aula
				int rowF = row + linhasDeExcelPorCreditoDaAula;
				if ( (row != rowF)){
					if (rowF < row)
						rowF = row;
					//System.out.println("merging: " + row + "-" + rowF);
					mergeCells(row, rowF, col, col, sheet, style);
				}
				
				// fornece a oportunidade das classes concretas executarem algum processamento relacionado com aula durante a escrita da aula
				onWriteAula(row,col,aula);

				if (!temInfoDeHorarios) {
					row += aula.getTotalCreditos() * linhasDeExcelPorCreditoDaAula;
				}
			}
		}
		return (initialRow + horariosEscritos.size() + 1);
	}
	
	protected int writeAulasComBuracos(List<AtendimentoRelatorioDTO> aulas, int row, int mdcTemposAula, boolean temInfoDeHorarios, List<String> horariosDaGradeHoraria, List<String> horariosDeInicioDeAula, List<String> horariosDeFimDeAula) {		
		if (temInfoDeHorarios) { 
			TrioDTO<List<String>,List<String>, List<Boolean>> parDTO = GradeHoraria.processaLabelsDasLinhasDaGradeHoraria(horariosDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula);
			labelsDasLinhasDaGradeHoraria = parDTO.getPrimeiro();
			horarioEhIntervalo = parDTO.getTerceiro();
		} else {
			labelsDasLinhasDaGradeHoraria = horariosDaGradeHoraria;
		}
		
		// TODO: Utilizar ideia abaixo para generalizar impressão de mais de 65536 linhas em extensão XLS
		// verifica se o max de linhas será extrapolado
		if (isXls()){
			Sheet newSheet = restructuringWorkbookIfRowLimitIsViolated(row,(labelsDasLinhasDaGradeHoraria.size()+12),sheet);
			if (newSheet != null) {
				System.out.println("RestructuringWorkbook");
				row = this.initialRow;
				sheet = newSheet;
			}
		}
		
		int initialRow = row;
		int col = 2;
	
		horarioInicioAula.clear();
		horarioFimAula.clear();
		Map<Integer,List<AtendimentoRelatorioDTO>> colunaGradeHorariaToAulasMap = new HashMap<Integer,List<AtendimentoRelatorioDTO>>();
		for(AtendimentoRelatorioDTO aula : aulas){
			// agrupa as aulas por dia da semana e coleta disciplinas
			List<AtendimentoRelatorioDTO> aulasDoDia = colunaGradeHorariaToAulasMap.get(aula.getSemana());
			if(aulasDoDia == null){
				aulasDoDia = new ArrayList<AtendimentoRelatorioDTO>();
				colunaGradeHorariaToAulasMap.put(aula.getSemana(), aulasDoDia);
			}
			aulasDoDia.add(aula);
			
			// coleta horário inicial e final da aula
			String[] horarioInicialArray = aula.getHorarioAulaString().split(":");
			int horarioInicialHoras = Integer.parseInt(horarioInicialArray[0]);
			int horarioInicialMinutos = Integer.parseInt(horarioInicialArray[1]);
			horarioInicioAula.add(aula.getHorarioAulaString());
			for (int i = 1; i <= aula.getTotalCreditos() ; i++) {
				int horarioFinalHoras = horarioInicialHoras;
				int horarioFinalMinutos = horarioInicialMinutos + (aula.getDuracaoDeUmaAulaEmMinutos() * i);
				while (horarioFinalMinutos >= 60) {
					horarioFinalHoras++;
					horarioFinalMinutos -= 60;
				}
				horarioFimAula.add(String.format("%02d", horarioFinalHoras) + ":" + String.format("%02d", horarioFinalMinutos));
			}
		}

		// preenche grade vazia
		int rowAdd = 0;
		List<String> horariosEscritos = new ArrayList<String>();
		Map<Integer, Boolean> matrizIntervalos = new HashMap<Integer, Boolean>();
		if (!labelsDasLinhasDaGradeHoraria.isEmpty()) {
			String horarioASerEscrito = labelsDasLinhasDaGradeHoraria.get(0).substring(0, 5);
			// para cada linha da grade horária
			for (int linhaAtualGradeHor = 1; linhaAtualGradeHor < labelsDasLinhasDaGradeHoraria.size(); linhaAtualGradeHor++) {
				String hiLinhaAtualGradeHor = labelsDasLinhasDaGradeHoraria.get(linhaAtualGradeHor).substring(0,5);
				boolean haAlgumaAulaQueIniciaEmHIdaLinhaAtual = horarioInicioAula.contains(hiLinhaAtualGradeHor);
				boolean haAlgumaAulaQueTerminaEmHIdaLinhaAtual = horarioFimAula.contains(hiLinhaAtualGradeHor);
				if (haAlgumaAulaQueIniciaEmHIdaLinhaAtual || haAlgumaAulaQueTerminaEmHIdaLinhaAtual) {
					horarioASerEscrito += " / " + hiLinhaAtualGradeHor;
					// escreve células da coluna de carga horária
					{
						if (!isHorarioIntervalo(horarioASerEscrito))
							setCell((row + rowAdd), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], horarioASerEscrito);
						else {
							int rowEscrita = (row + rowAdd);
							int colEscrita = col++;
							setCell(rowEscrita, colEscrita, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], "");
							getRow((row + rowAdd -1), sheet).setHeight((short)200);
							matrizIntervalos.put(rowEscrita, true);
						}
						horariosEscritos.add(horarioASerEscrito);
					}
					// escreve células das colunas dos dias da semana
					{
						for (int j = 0; j < Semanas.values().length; j++) {
							setCell((row + rowAdd), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], "");
						}
					}
					col = 2;
					rowAdd++;
					horarioASerEscrito = hiLinhaAtualGradeHor;
				}
			}
			
			horarioASerEscrito += " / " + labelsDasLinhasDaGradeHoraria.get(labelsDasLinhasDaGradeHoraria.size() -1).substring(8, 13);
			setCell((row + rowAdd), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], horarioASerEscrito);
			horariosEscritos.add(horarioASerEscrito);
			for(int j = 0; j < Semanas.values().length; j++) {
				setCell((row + rowAdd), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], "");
			}
		}
		
		// para cada dia da semana, escreve as aulas no excel
		Map<Integer, Map<Integer, AtendimentoRelatorioDTO>> matrizAulas = new HashMap<Integer, Map<Integer, AtendimentoRelatorioDTO>>();
		Map<Integer, Integer> colToLastRowMap = new HashMap<Integer, Integer>();
		for (Integer colunaGradeHoraria : colunaGradeHorariaToAulasMap.keySet()) {
			Semanas diaSemana = Semanas.get(colunaGradeHoraria);
	
			// linha em que a escrita será iniciada
			row = initialRow;
			// coluna em que a escrita será iniciada
			col = (diaSemana == Semanas.DOM) ? 9 : colunaGradeHoraria + 1;
			
			List<AtendimentoRelatorioDTO> aulasDoDia = colunaGradeHorariaToAulasMap.get(colunaGradeHoraria);
			if(aulasDoDia == null || aulasDoDia.isEmpty()) continue;
	
			// para cada aula
			for (AtendimentoRelatorioDTO aula : aulasDoDia) {
				// obtém o estilo que será aplicado nas células que serão desenhadas
				CellStyle style = getCellStyle(aula);
				// obtém a qtd de linhas que devem ser desenhadas para cada crédito da aula em questão
				int linhasDeExcelPorCreditoDaAula = aula.getDuracaoDeUmaAulaEmMinutos() / mdcTemposAula;
				
				if (temInfoDeHorarios) {
					int index = getIndexHorario(aula.getHorarioAulaString(), horariosEscritos);//int index = horariosDeInicioDeAula.indexOf(aula.getHorarioAulaString());
					if (index != -1) {
						row = initialRow + index;
					}
					linhasDeExcelPorCreditoDaAula = getIndexHorario(getHorarioFinalAula(aula), horariosEscritos) - index - 1;
				}
	
				String contentString = "", contentToolTipString = "";
				try{
					Method m = aula.getClass().getMethod("getContentVisao" + getReportEntity(), ReportType.class);
					contentString = (String) m.invoke(aula, ReportType.EXCEL);
					
					m = aula.getClass().getMethod("getContentToolTipVisao" + getReportEntity(), ReportType.class);
					contentToolTipString = (String) m.invoke(aula, ReportType.EXCEL);
				}
				catch(Exception e){
					e.printStackTrace();
				}
				// escreve célula principal
				setCell(row, col, sheet, style, HtmlUtils.htmlUnescape(contentString), HtmlUtils.htmlUnescape(contentToolTipString));
				// une células de acordo com a quantidade de créditos da aula
				int rowF = row + linhasDeExcelPorCreditoDaAula;
				if ((row != rowF)) {
					if (rowF < row)
						rowF = row;
					//System.out.println("merging: " + row + "-" + rowF);
					mergeCells(row, rowF, col, col, sheet, style);
				}
				insereAulaNaMatriz(row, rowF, col, aula, matrizAulas, colToLastRowMap);
				
				// fornece a oportunidade das classes concretas executarem algum processamento relacionado com aula durante a escrita da aula
				onWriteAula(row,col,aula);
	
				if (!temInfoDeHorarios) {
					row += aula.getTotalCreditos() * linhasDeExcelPorCreditoDaAula;
				}
			}
		}
		
		// desenha buracos
		Map<Integer, Map<Integer, TipoBuraco>> matrizBuracos = new HashMap<Integer, Map<Integer, TipoBuraco>>();
		rowAdd = 0;
		row = initialRow;
		col = 2;
		String horarioASerEscrito = labelsDasLinhasDaGradeHoraria.get(0).substring(0,5);
		for (int linhaAtualGradeHor = 1; linhaAtualGradeHor < labelsDasLinhasDaGradeHoraria.size(); linhaAtualGradeHor++) {
			String hiLinhaAtualGradeHor = labelsDasLinhasDaGradeHoraria.get(linhaAtualGradeHor).substring(0,5);
			boolean haAlgumaAulaQueIniciaEmHIdaLinhaAtual = horarioInicioAula.contains(hiLinhaAtualGradeHor);
			boolean haAlgumaAulaQueTerminaEmHIdaLinhaAtual = horarioFimAula.contains(hiLinhaAtualGradeHor);
			if (haAlgumaAulaQueIniciaEmHIdaLinhaAtual || haAlgumaAulaQueTerminaEmHIdaLinhaAtual) {
				horarioASerEscrito += " / " + hiLinhaAtualGradeHor;
				if (!isHorarioIntervalo(horarioASerEscrito)) {
					String hi = horarioASerEscrito.substring(0,5);
					String hf = horarioASerEscrito.substring(8,13);
					for (int j = 0; j < Semanas.values().length; j++) {
						int rowEscrita = (row + rowAdd);
						int colEscrita = col++;
						TipoBuraco tipoBuraco = ehBuraco(rowEscrita, colEscrita, hi, hf, matrizAulas, colToLastRowMap, matrizBuracos, matrizIntervalos);
						if (!TipoBuraco.NAO_EH_BURACO.equals(tipoBuraco)) {
							setCell(rowEscrita, colEscrita, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], tipoBuraco.toString());
						}
					}
				}
				col = 2;
				rowAdd++;
				horarioASerEscrito = hiLinhaAtualGradeHor;
			}
		}
		horarioASerEscrito += " / " + labelsDasLinhasDaGradeHoraria.get(labelsDasLinhasDaGradeHoraria.size()-1).substring(8,13);
		String hi = horarioASerEscrito.substring(0,5);
		String hf = horarioASerEscrito.substring(8,13);
		for(int j = 0; j < Semanas.values().length; j++) {
			int rowEscrita = (row + rowAdd);
			int colEscrita = col++;
			TipoBuraco tipoBuraco = ehBuraco(rowEscrita, colEscrita, hi, hf, matrizAulas, colToLastRowMap, matrizBuracos, matrizIntervalos);
			if (!TipoBuraco.NAO_EH_BURACO.equals(tipoBuraco)) {
				setCell((row + rowAdd), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], tipoBuraco.toString());
			}
		}
		
		return (initialRow + horariosEscritos.size() + 1);
	}
	
	private void insereBuracoNaMatriz(int row, int col, TipoBuraco tipoBuraco, Map<Integer, Map<Integer, TipoBuraco>> matrizBuracos) {
		Map<Integer, TipoBuraco> colToBuracosMap = matrizBuracos.get(row);
		if (colToBuracosMap == null){
			colToBuracosMap = new HashMap<Integer, TipoBuraco>();
			matrizBuracos.put(row, colToBuracosMap);
		}
		colToBuracosMap.put(col, tipoBuraco);
	}
	
	private void insereAulaNaMatriz(int row, int col, AtendimentoRelatorioDTO aula, Map<Integer, Map<Integer, AtendimentoRelatorioDTO>> matrizAulas) {
		Map<Integer, AtendimentoRelatorioDTO> colToAulasMap = matrizAulas.get(row);
		if (colToAulasMap == null){
			colToAulasMap = new HashMap<Integer, AtendimentoRelatorioDTO>();
			matrizAulas.put(row, colToAulasMap);
		}
		colToAulasMap.put(col, aula);
	}
	
	private void insereAulaNaMatriz(int rowI, int rowF, int col, AtendimentoRelatorioDTO aula, Map<Integer, Map<Integer, AtendimentoRelatorioDTO>> matrizAulas, Map<Integer, Integer> colToLastRowMap) {
		for (int row = rowI; row <= rowF; row++) {
			insereAulaNaMatriz(row, col, aula, matrizAulas);
		}
		Integer lastRow = colToLastRowMap.get(col);
		colToLastRowMap.put(col, (lastRow != null) ? Math.max(lastRow, rowF) : rowF);
	}
	
	private AtendimentoRelatorioDTO getAula(int row, int col, Map<Integer, Map<Integer, AtendimentoRelatorioDTO>> matrizAulas) {		
		Map<Integer, AtendimentoRelatorioDTO> colToAulasMap = matrizAulas.get(row);
		if (colToAulasMap != null){
			return colToAulasMap.get(col);
		}
		return null;
	}
	
	private TipoBuraco getBuraco(int row, int col, Map<Integer, Map<Integer, TipoBuraco>> matrizBuracos) {		
		Map<Integer, TipoBuraco> colToAulasMap = matrizBuracos.get(row);
		if (colToAulasMap != null){
			TipoBuraco tipoBuraco = colToAulasMap.get(col);
			if (tipoBuraco != null) {
				return tipoBuraco;
			}
		}
		return TipoBuraco.NAO_EH_BURACO;
	}
	
	enum TipoBuraco {NAO_EH_BURACO, BURACO_DESLOCAMENTO, BURACO};
	private TipoBuraco ehBuraco(int row, int col, String hi, String hf, Map<Integer, Map<Integer, AtendimentoRelatorioDTO>> matrizAulas, Map<Integer, Integer> colToLastRowMap, Map<Integer, Map<Integer, TipoBuraco>> matrizBuracos, Map<Integer, Boolean> matrizIntervalos) {		
		AtendimentoRelatorioDTO eventoEmQuestao = getAula(row, col, matrizAulas);
		if (eventoEmQuestao == null) { // é candidato a buraco?
			Turno turnoHIEventoEmQuestao = this.horarioToTurnoMap.get(hi); // turno do horário de início do possível buraco em questão
			Turno turnoHFEventoEmQuestao = this.horarioToTurnoMap.get(hf); // turno do horário de fim do possíve buraco em questão
			if ((turnoHIEventoEmQuestao != null) && turnoHIEventoEmQuestao.equals(turnoHFEventoEmQuestao)) { // para ser considerado buraco, intervalo tem que pertencer ao mesmo turno
				// coleta o evento anterior ao possível buraco
				AtendimentoRelatorioDTO eventoAnterior = null;
				TipoBuraco buracoAnterior = null;
				if (matrizIntervalos.get(row-1) != null) { // é um intervalo?
					eventoAnterior = getAula(row-2, col, matrizAulas);
					buracoAnterior = getBuraco(row-2, col, matrizBuracos);
				} else {
					eventoAnterior = getAula(row-1, col, matrizAulas);
					buracoAnterior = getBuraco(row-1, col, matrizBuracos);
				}
				if ((eventoAnterior != null) || !TipoBuraco.NAO_EH_BURACO.equals(buracoAnterior)) {
					// se entrou aqui, então:
					//  - ou o evento anterior é uma aula normal
					//  - ou o evento anterior é um buraco
					
					// coleta o evento posterior ao possível buraco
					int lastRow = colToLastRowMap.get(col); // coleta a linha com o último evento existente
					AtendimentoRelatorioDTO eventoPosterior = null;
					int rowBusca = row;
					do {
						rowBusca++;
						eventoPosterior = getAula(rowBusca, col, matrizAulas);
					} while((eventoPosterior == null) && (rowBusca < lastRow));
					/*if (matrizIntervalos.get(row+1) != null) { // é um intervalo?
						eventoPosterior = getAula(row+2, col, matrizAulas);
					} else {
						eventoPosterior = getAula(row+1, col, matrizAulas);
					}*/
					if (eventoPosterior != null) { // evento posterior é uma aula?
						Long turnoEventoAnterior = (eventoAnterior != null) ? this.horarioToTurnoMap.get(eventoAnterior.getHorarioAulaString()).getId() : turnoHIEventoEmQuestao.getId();
						Long turnoEventoEmQuestao = turnoHIEventoEmQuestao.getId();
						Long turnoEventoPosterior = this.horarioToTurnoMap.get(eventoPosterior.getHorarioAulaString()).getId();
						// para ser considerado um buraco, os 3 eventos devem pertencer ao mesmo turno
						if (turnoEventoAnterior.equals(turnoEventoEmQuestao) && turnoEventoEmQuestao.equals(turnoEventoPosterior)) {
							// se entrou aqui o evento em questão é um buraco, agora resta saber se:
							//   - é um buraco por motivo de deslocamento do professor entre unidades
							//   - é um buraco mesmo
							if (eventoAnterior != null) { // evento anterior é uma aula?
								if (ehCasoDeslocamento(eventoAnterior,eventoPosterior)) {
									insereBuracoNaMatriz(row, col, TipoBuraco.BURACO_DESLOCAMENTO, matrizBuracos);
									return TipoBuraco.BURACO_DESLOCAMENTO;
								}
								insereBuracoNaMatriz(row, col, TipoBuraco.BURACO, matrizBuracos);
								return TipoBuraco.BURACO;
							}
							return buracoAnterior;
						}
					}
				}
			}
		}
		return TipoBuraco.NAO_EH_BURACO;
	}
	
	private boolean ehCasoDeslocamento(AtendimentoRelatorioDTO aula1, AtendimentoRelatorioDTO aula2) {
		Double tempoDeslocamento = null;
		if (!aula1.getUnidadeId().equals(aula2.getUnidadeId())) {
			if (aula1.getCampusId().equals(aula2.getCampusId())) {
				String chaveOrigemDestinoUnidade = DeslocamentoUnidade.CriaChaveDeslocamentoUnidade(aula1.getUnidadeId(),aula2.getUnidadeId());
				tempoDeslocamento = this.origemDestinoUnidadeToTempoDeslocamentoMap.get(chaveOrigemDestinoUnidade);
			} else {
				String chaveOrigemDestinoCampus = DeslocamentoCampus.CriaChaveDeslocamentoCampus(aula1.getCampusId(),aula2.getCampusId());
				tempoDeslocamento = this.origemDestinoCampusToTempoDeslocamentoMap.get(chaveOrigemDestinoCampus);
			}
		}
		return tempoDeslocamento != null;
	}
	
	protected void buildCodigoDisciplinaToColorMap(Set<Long> disciplinasIDs) {
		// ordena disciplinas e monta mapa de cores por disciplina
		List<Long> disciplinasOrdenadas = new ArrayList<Long>(disciplinasIDs);
		Collections.sort(disciplinasOrdenadas);
		for (Long disciplinaId : disciplinasOrdenadas) {
			CellStyle style = excelColorsPool.get(codigoDisciplinaToColorMap.size() % excelColorsPool.size());
			codigoDisciplinaToColorMap.put(disciplinaId,style);
		}
	}
	
	protected void buildCodigoDisciplinaToColorMapElevaEducacao(Set<Long> disciplinas) {
		Map<Long, Disciplina> disciplinasMap = new HashMap<Long, Disciplina>();
		for (Disciplina disciplina : getCenario().getDisciplinas()) {
			disciplinasMap.put(disciplina.getId(), disciplina);
		}		
		List<Disciplina> disList = new ArrayList<Disciplina>();
		for (Long disId : disciplinas) {
			disList.add(disciplinasMap.get(disId));
		}
		
		Map<Disciplina, String> disciplinaToSiglaMap = new HashMap<Disciplina, String>();
		Set<String> disciplinasSiglas = new HashSet<String>();
		for (Disciplina disciplina : disList) {
			String disSigla = TriedaUtil.getSiglaDisciplinaParaEscola(disciplina.getCodigo());
//			String disSigla = disciplina.getCodigo();
//			if (disSigla.contains("_")) {
//				disSigla = disSigla.split("_")[0];
//			}
			disciplinasSiglas.add(disSigla);
			disciplinaToSiglaMap.put(disciplina, disSigla);
		}
		
		// ordena disciplinas e monta mapa de cores por disciplina
		List<String> disciplinasOrdenadas = new ArrayList<String>(disciplinasSiglas);
		Collections.sort(disciplinasOrdenadas);
		Map<String, CellStyle> disciplinaSiglaToCorMap = new HashMap<String, CellStyle>();
		for (String disciplinaSigla : disciplinasOrdenadas) {
			CellStyle style = excelColorsPool.get(disciplinaSiglaToCorMap.size() % excelColorsPool.size());
			disciplinaSiglaToCorMap.put(disciplinaSigla,style);
		}
		for (Disciplina dis : disList) {
			String disSigla = disciplinaToSiglaMap.get(dis);
			CellStyle style = disciplinaSiglaToCorMap.get(disSigla);
			codigoDisciplinaToColorMap.put(dis.getId(),style);
		}
	}

	protected CellStyle getCellStyle(AtendimentoRelatorioDTO aula){
		Long disciplinaId = aula.getDisciplinaSubstitutaId() != null ? aula.getDisciplinaSubstitutaId() : aula.getDisciplinaId(); 
		CellStyle style = codigoDisciplinaToColorMap.get(disciplinaId);
//		if(style == null){
//			style = excelColorsPool.get(codigoDisciplinaToColorMap.size() % excelColorsPool.size());
//			codigoDisciplinaToColorMap.put(aula.getDisciplinaString(), style);
//		}
		return style;
	}
	
	protected int writeHeader(List<List<ParDTO<String, ?>>> rowsHeadersPairs, int row, boolean temInfoDeHorarios){
		int col = 3;
		
		for(List<ParDTO<String, ?>> headersPairs : rowsHeadersPairs){
			for(ParDTO<String, ?> headerPair : headersPairs){
				setCell(row, col++, sheet, this.cellStyles[ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal()],
					HtmlUtils.htmlUnescape(headerPair.getPrimeiro())
				);
				if(headerPair.getSegundo() instanceof String)
					setCell(row, col++, sheet, this.cellStyles[ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal()],
						(String) headerPair.getSegundo()
					);
				else if(headerPair.getSegundo() instanceof Integer)
					setCell(row, col++, sheet, this.cellStyles[ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal()],
						(Integer) headerPair.getSegundo()
					);
				else
					setCell(row, col++, sheet, this.cellStyles[ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal()],
						(Double) headerPair.getSegundo()
					);
			}
			row++;
			col = 3;
		}

		col = 2;

		// Créditos ou Horários
		setCell(row, col++, sheet, this.cellStyles[ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal()],
			HtmlUtils.htmlUnescape(temInfoDeHorarios ? this.getI18nConstants().horarios() : this.getI18nConstants().cargaHorariaMinutos()));

		// Dias Semana
		return setSemanasCell(col, row);
	}
	
	protected int setSemanasCell(int col, int row){
		for(Semanas semanas : Semanas.values()){
			setCell(row, col++, sheet, this.cellStyles[ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal()], semanas.name());
		}

		return ++row;
	}
	
	protected void fillInCellStyles(Sheet sheet){
		for(ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()){
			this.cellStyles[cellStyleReference.ordinal()] = 
				getCell(cellStyleReference.getRow(), cellStyleReference.getCol(), sheet).getCellStyle();
		}
	}
	
	protected void buildColorPaletteCellStyles(Workbook workbook){
		excelColorsPool = new ArrayList<CellStyle>();
		
		Font whiteFont = workbook.createFont();
		whiteFont.setColor(IndexedColors.WHITE.index);
		Font blackFont = workbook.createFont();
		blackFont.setColor(IndexedColors.BLACK.index);
		for (HSSFColor color : HSSFColor.getIndexHash().values()) {
			if (calculateColorIndexUsage(color.getTriplet()))
			{
				CellStyle cellStyle = workbook.createCellStyle();
				cellStyle.setFillPattern(CellStyle.SOLID_FOREGROUND);
				cellStyle.setFillForegroundColor(color.getIndex());
				cellStyle.setFont(calculateForegroundColorIndex(color.getTriplet()) == IndexedColors.WHITE.index ? whiteFont : blackFont);
				cellStyle.setAlignment(CellStyle.ALIGN_CENTER);
				cellStyle.setVerticalAlignment(CellStyle.VERTICAL_CENTER);
				cellStyle.setBorderBottom(CellStyle.BORDER_THIN);
				cellStyle.setBorderLeft(CellStyle.BORDER_THIN);
				cellStyle.setBorderRight(CellStyle.BORDER_THIN);
				cellStyle.setBorderTop(CellStyle.BORDER_THIN);
				excelColorsPool.add(cellStyle);
			}
		}
//		Sheet sheet = workbook.getSheet(ExcelInformationType.PALETA_CORES.getSheetName());
//		if(sheet != null){
//            for(int rowIndex = sheet.getFirstRowNum(); rowIndex <= sheet.getLastRowNum(); rowIndex++){
//        	Row row = sheet.getRow(rowIndex);
//            	if(row != null){
//            		Cell cell = row.getCell((int) row.getFirstCellNum());
//            		if(cell != null) excelColorsPool.add(cell.getCellStyle());
//            	}
//            }
//		}
	}
	
	private short calculateForegroundColorIndex(short[] colorRGB) {
		float r = colorRGB[0];
		float g = colorRGB[1];
		float b = colorRGB[2];
		
		if ((0.3 * r + 0.59 * g + 0.11 * b) <= 127) {
			return IndexedColors.WHITE.index;
		}
		
		return IndexedColors.BLACK.index;
	}
	
	private boolean calculateColorIndexUsage(short[] colorRGB) {
		float r = colorRGB[0];
		float g = colorRGB[1];
		float b = colorRGB[2];
		
		return r > 127 && g > 127 && b > 127;

	}
}
