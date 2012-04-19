package com.gapso.web.trieda.server.excel.exp;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;

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
	
	protected HSSFCellStyle [] cellStyles;
	protected HSSFSheet sheet;
	protected Map<String,HSSFCellStyle> codigoDisciplinaToColorMap;
	protected List<HSSFCellStyle> excelColorsPool;
	protected boolean removeUnusedSheets;
	protected int initialRow;
	
	public RelatorioVisaoExportExcel(boolean removeUnusedSheets, Cenario cenario, 
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		ExportExcelFilter filter, InstituicaoEnsino instituicaoEnsino)
	{
		super(false, null, cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		this.setSheetName(getReportSheetName());
		
		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.codigoDisciplinaToColorMap = new HashMap<String, HSSFCellStyle>();
		this.initialRow = 5;

		this.setFilter(filter);
	}
	
	protected abstract String getReportSheetName();
	protected abstract String getReportEntity();
	
	@Override
	protected String getPathExcelTemplate(){
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
	
	@Override
	protected boolean fillInExcel(HSSFWorkbook workbook){
		return this.fillInExcelImpl(workbook);
	}
	
	protected <T> boolean fillInExcelImpl(HSSFWorkbook workbook){
		boolean result = false;
		
		T structureControl = getStructureReportControl();
		
		if(getAtendimentosRelatorioDTOList(structureControl)){
			sheet = workbook.getSheet(this.getSheetName());
			
			// monta estruturas de estilos
			fillInCellStyles();
			buildColorPaletteCellStyles(workbook);
			
			processStructureReportControl(structureControl);
			
			result = true;
		}

		if(this.removeUnusedSheets) removeUnusedSheets(this.getSheetName(), workbook);

		return result;
	}
	
	protected int writeAtendimento(List<AtendimentoRelatorioDTO> atendimentos, int row, int mdcTemposAula, boolean ehTatico, 
		List<String> labelsDasLinhasDaGradeHoraria)
	{
		int initialRow = row;
		int col = 2;

		// preenche grade vazia
		if(ehTatico){
			for(int i = 0; i < labelsDasLinhasDaGradeHoraria.size(); i++){
				// coluna de carga horária
				setCell((row + i), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], labelsDasLinhasDaGradeHoraria.get(i));
				// colunas dos dias da semana
				for(int j = 0; j < Semanas.values().length; j++)
					setCell((row + i), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], "");
				
				col = 2;
			}
		}
		else{
			for(int i = 0; i < labelsDasLinhasDaGradeHoraria.size() - 1; i++){
				// coluna de carga horária
				String label = labelsDasLinhasDaGradeHoraria.get(i) + " / " + labelsDasLinhasDaGradeHoraria.get(i + 1);
				setCell((row + i), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], label);
				// colunas dos dias da semana
				for(int j = 0; j < Semanas.values().length; j++){
					setCell((row + i), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], "");
				}
				
				col = 2;
			}
		}

		// agrupa as aulas por dia da semana
		Map<Integer,List<AtendimentoRelatorioDTO>> diaSemanaToAtendimentosMap = new HashMap<Integer,List<AtendimentoRelatorioDTO>>();
		for(AtendimentoRelatorioDTO atendimento : atendimentos){
			List<AtendimentoRelatorioDTO> atendimentosDoDia = diaSemanaToAtendimentosMap.get(atendimento.getSemana());
			if(atendimentosDoDia == null){
				atendimentosDoDia = new ArrayList<AtendimentoRelatorioDTO>();
				diaSemanaToAtendimentosMap.put(atendimento.getSemana(), atendimentosDoDia);
			}
			atendimentosDoDia.add(atendimento);
		}

		// para cada dia da semana, escreve as aulas no excel
		for (Integer diaSemanaInt : diaSemanaToAtendimentosMap.keySet()) {
			Semanas diaSemana = Semanas.get(diaSemanaInt);

			// linha em que a escrita será iniciada
			row = initialRow;
			// coluna em que a escrita será iniciada
			col = (diaSemana == Semanas.DOM) ? 9 : diaSemanaInt + 1;
			
			List<AtendimentoRelatorioDTO> atendimentosDoDia = diaSemanaToAtendimentosMap.get(diaSemanaInt);
			if(atendimentosDoDia == null || atendimentosDoDia.isEmpty()) continue;

			// para cada aula
			for (AtendimentoRelatorioDTO atendimento : atendimentosDoDia) {
				// obtém o estilo que será aplicado nas células que serão desenhadas
				HSSFCellStyle style = getCellStyle(atendimento);
				// obtém a qtd de linhas que devem ser desenhadas para cada crédito da aula em questão
				int linhasDeExcelPorCreditoDaAula = atendimento.getDuracaoDeUmaAulaEmMinutos() / mdcTemposAula;
				
				if(!ehTatico){
					AtendimentoOperacionalDTO aulaOp = (AtendimentoOperacionalDTO) atendimento;
					int index = labelsDasLinhasDaGradeHoraria.indexOf(aulaOp.getHorarioString());
					if (index != -1) {
						row = initialRow + index;
					}
				}

				String contentString = "", contentToolTipString = "";
				try{
					Method m = atendimento.getClass().getMethod("getContentVisao" + getReportEntity(), ReportType.class);
					contentString = (String) m.invoke(atendimento, ReportType.EXCEL);
					
					m = atendimento.getClass().getMethod("getContentToolTipVisao" + getReportEntity(), ReportType.class);
					contentToolTipString = (String) m.invoke(atendimento, ReportType.EXCEL);
				}
				catch(Exception e){
					e.printStackTrace();
				}
				
				// escreve célula principal
				setCell(row, col, sheet, style, HtmlUtils.htmlUnescape(contentString), HtmlUtils.htmlUnescape(contentToolTipString));
				// une células de acordo com a quantidade de créditos da aula
				int rowF = row + atendimento.getTotalCreditos() * linhasDeExcelPorCreditoDaAula - 1;
				mergeCells(row, rowF, col, col, sheet, style);

				if(ehTatico){
					row += atendimento.getTotalCreditos() * linhasDeExcelPorCreditoDaAula;
				}
			}
		}
		
		return (initialRow + (ehTatico ? labelsDasLinhasDaGradeHoraria.size() : (labelsDasLinhasDaGradeHoraria.size() - 1)) + 1);
	}
	
	protected HSSFCellStyle getCellStyle(AtendimentoRelatorioDTO atendimento){
		HSSFCellStyle style = codigoDisciplinaToColorMap.get(atendimento.getDisciplinaString());
		if(style == null){
			style = excelColorsPool.get(codigoDisciplinaToColorMap.size() % excelColorsPool.size());
			codigoDisciplinaToColorMap.put(atendimento.getDisciplinaString(), style);
		}
		
		return style;
	}
	
	protected int writeHeader(List<List<ParDTO<String, ?>>> rowsHeadersPairs, int row, boolean ehTatico){
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
			HtmlUtils.htmlUnescape(ehTatico ? this.getI18nConstants().cargaHorariaMinutos() : this.getI18nConstants().horarios()));

		// Dias Semana
		return setSemanasCell(col, row);
	}
	
	protected int setSemanasCell(int col, int row){
		for(Semanas semanas : Semanas.values()){
			setCell(row, col++, sheet, this.cellStyles[ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal()], semanas.name());
		}

		return ++row;
	}
	
	protected void fillInCellStyles(){
		for(ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()){
			this.cellStyles[cellStyleReference.ordinal()] = 
				getCell(cellStyleReference.getRow(), cellStyleReference.getCol(), sheet).getCellStyle();
		}
	}
	
	protected void buildColorPaletteCellStyles(HSSFWorkbook workbook){
		excelColorsPool = new ArrayList<HSSFCellStyle>();
		
		HSSFSheet sheet = workbook.getSheet(ExcelInformationType.PALETA_CORES.getSheetName());
		if(sheet != null){
            for(int rowIndex = sheet.getFirstRowNum(); rowIndex <= sheet.getLastRowNum(); rowIndex++){
            	HSSFRow row = sheet.getRow(rowIndex);
            	if(row != null){
            		HSSFCell cell = row.getCell((int) row.getFirstCellNum());
            		if(cell != null) excelColorsPool.add(cell.getCellStyle());
            	}
            }
		}
	}
	
}
