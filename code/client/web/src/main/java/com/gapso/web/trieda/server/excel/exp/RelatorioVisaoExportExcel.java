package com.gapso.web.trieda.server.excel.exp;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.poi.hssf.util.HSSFColor;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Font;
import org.apache.poi.ss.usermodel.IndexedColors;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
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
	protected Map<Long,CellStyle> codigoDisciplinaToColorMap;
	protected List<CellStyle> excelColorsPool;
	protected boolean removeUnusedSheets;
	protected int initialRow;
	
	public RelatorioVisaoExportExcel(boolean removeUnusedSheets, Cenario cenario, 
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		ExportExcelFilter filter, InstituicaoEnsino instituicaoEnsino,
		String fileExtension)
	{
		super(false, null, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
		this.setSheetName(getReportSheetName());
		
		this.cellStyles = new CellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.codigoDisciplinaToColorMap = new HashMap<Long,CellStyle>();
		this.initialRow = 5;

		this.setFilter(filter);
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
			fillInCellStyles();
			buildColorPaletteCellStyles(workbook);
			
			processStructureReportControl(structureControl);
			
			result = true;
			
			autoSizeColumns((short)1,(short)1,sheet);
		}

		if(this.removeUnusedSheets) removeUnusedSheets(this.getSheetName(), workbook);

		return result;
	}
	
	protected int writeAulas(List<AtendimentoRelatorioDTO> aulas, int row, int mdcTemposAula, boolean temInfoDeHorarios, 
				List<String> horariosDaGradeHoraria, List<String> horariosDeInicioDeAula, List<String> horariosDeFimDeAula)
	{
		List<String> labelsDasLinhasDaGradeHoraria;
		List<String> hiDasLinhasDaGradeHoraria = new ArrayList<String>();
		if (temInfoDeHorarios) { 
			ParDTO<List<String>,List<String>> parDTO = GradeHoraria.processaLabelsDasLinhasDaGradeHoraria(horariosDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula);
			labelsDasLinhasDaGradeHoraria = parDTO.getPrimeiro();
			hiDasLinhasDaGradeHoraria = parDTO.getSegundo();
		} else {
			labelsDasLinhasDaGradeHoraria = horariosDaGradeHoraria;
		}
		
		// TODO: Utilizar ideia abaixo para generalizar impressão de mais de 65536 linhas em extensão XLS
		// verifica se o max de linhas será extrapolado
		Sheet newSheet = restructuringWorkbookIfRowLimitIsViolated(row,(labelsDasLinhasDaGradeHoraria.size()+12),sheet);
		if (newSheet != null) {
			row = this.initialRow;
			sheet = newSheet;
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

		// preenche grade vazia
		for (int i = 0; i < labelsDasLinhasDaGradeHoraria.size(); i++) {
			// coluna de carga horária
			setCell((row + i), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], labelsDasLinhasDaGradeHoraria.get(i));
			// colunas dos dias da semana
			for(int j = 0; j < Semanas.values().length; j++) {
				setCell((row + i), col++, sheet, this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()], "");
			}
			col = 2;
		}

		// agrupa as aulas por dia da semana e coleta disciplinas
		Map<Integer,List<AtendimentoRelatorioDTO>> colunaGradeHorariaToAulasMap = new HashMap<Integer,List<AtendimentoRelatorioDTO>>();
		for(AtendimentoRelatorioDTO aula : aulas){
			List<AtendimentoRelatorioDTO> aulasDoDia = colunaGradeHorariaToAulasMap.get(aula.getSemana());
			if(aulasDoDia == null){
				aulasDoDia = new ArrayList<AtendimentoRelatorioDTO>();
				colunaGradeHorariaToAulasMap.put(aula.getSemana(), aulasDoDia);
			}
			aulasDoDia.add(aula);
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
					int index = hiDasLinhasDaGradeHoraria.indexOf(aula.getHorarioAulaString());//int index = horariosDeInicioDeAula.indexOf(aula.getHorarioAulaString());
					if (index != -1) {
						row = initialRow + index;
					}
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
				int rowF = row + aula.getTotalCreditos() * linhasDeExcelPorCreditoDaAula - 1;
				mergeCells(row, rowF, col, col, sheet, style);
				
				// fornece a oportunidade das classes concretas executarem algum processamento relacionado com aula durante a escrita da aula
				onWriteAula(row,col,aula);

				if (!temInfoDeHorarios) {
					row += aula.getTotalCreditos() * linhasDeExcelPorCreditoDaAula;
				}
			}
		}
		
		return (initialRow + labelsDasLinhasDaGradeHoraria.size() + 1);
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
	
	protected void fillInCellStyles(){
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
}
