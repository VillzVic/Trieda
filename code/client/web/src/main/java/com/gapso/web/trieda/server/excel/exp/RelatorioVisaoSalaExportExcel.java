package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFComment;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class RelatorioVisaoSalaExportExcel extends AbstractExportExcel {
	
	enum ExcelCellStyleReference {
		HEADER_LEFT_TEXT(5,3),
		HEADER_CENTER_VALUE(5,4),
		HEADER_CENTER_TEXT(7,2),
		TEXT(8,2);
		private int row;
		private int col;
		private ExcelCellStyleReference(int row, int col) {
			this.row = row;
			this.col = col;
		}
		public int getRow() {
			return row;
		}
		public int getCol() {
			return col;
		}
	}
	private HSSFCellStyle[] cellStyles;
	
	private boolean removeUnusedSheets;
	private String sheetName;
	private int initialRow;
	
	public RelatorioVisaoSalaExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		super(cenario,i18nConstants,i18nMessages);
		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = true;
		this.sheetName = ExcelInformationType.RELATORIO_VISAO_SALA.getSheetName();
		this.initialRow = 5;
	}
	
	public RelatorioVisaoSalaExportExcel(boolean removeUnusedSheets, Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		super(cenario,i18nConstants,i18nMessages);
		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.RELATORIO_VISAO_SALA.getSheetName();
		this.initialRow = 5;
	}
	
	@Override
	public String getFileName() {
		return getI18nConstants().relatorioVisaoSala();
	}
	
	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().relatorioVisaoSala();
	}

	private List<AtendimentoRelatorioDTO> getAtendimentoRelatorioDTOList(Cenario cenario) {
		List<AtendimentoTatico> atdTaticoList = AtendimentoTatico.findByCenario(cenario);
		List<AtendimentoOperacional> atdOperacionalList = AtendimentoOperacional.findByCenario(getCenario());
		List<AtendimentoRelatorioDTO> atdRelatorioList = new ArrayList<AtendimentoRelatorioDTO>(atdTaticoList.size() + atdOperacionalList.size());
		for(AtendimentoTatico atdTatico : atdTaticoList) {
			atdRelatorioList.add(ConvertBeans.toAtendimentoTaticoDTO(atdTatico));
		}
		for(AtendimentoOperacional atdOperacional : atdOperacionalList) {
			atdRelatorioList.add(ConvertBeans.toAtendimentoOperacionalDTO(atdOperacional));
		}
		return atdRelatorioList;
	}
	
	@Override
	protected boolean fillInExcel(HSSFWorkbook workbook) {
		List<AtendimentoRelatorioDTO> atdRelatorioList = getAtendimentoRelatorioDTOList(getCenario());
		
		if (!atdRelatorioList.isEmpty()) {
			if (this.removeUnusedSheets) {
				removeUnusedSheets(this.sheetName,workbook);
			}
			
			HSSFSheet sheet = workbook.getSheet(this.sheetName);
			fillInCellStyles(sheet);
			
			List<HSSFComment> excelCommentsPool = buildExcelCommentsPool(workbook);
			Iterator<HSSFComment> itExcelCommentsPool = excelCommentsPool.iterator();
			
			List<HSSFCellStyle> excelColorsPool = buildColorPaletteCellStyles(workbook);
			Map<String,HSSFCellStyle> codigoDisciplinaToColorMap = new HashMap<String,HSSFCellStyle>();
			Map<Sala,Map<Turno,List<AtendimentoRelatorioDTO>>> mapNivel1 = new TreeMap<Sala,Map<Turno,List<AtendimentoRelatorioDTO>>>();
			for (AtendimentoRelatorioDTO atendimento : atdRelatorioList) {
				Sala sala = Sala.find(atendimento.getSalaId());
				Turno turno = Turno.find(atendimento.getTurnoId());
				
				Map<Turno,List<AtendimentoRelatorioDTO>> mapNivel2 = mapNivel1.get(sala);
				if (mapNivel2 == null) {
					mapNivel2 = new HashMap<Turno,List<AtendimentoRelatorioDTO>>();
					mapNivel1.put(sala,mapNivel2);
				}
				
				List<AtendimentoRelatorioDTO> list = mapNivel2.get(turno);
				if (list == null) {
					list = new ArrayList<AtendimentoRelatorioDTO>();
					mapNivel2.put(turno,list);
				}
				
				list.add(atendimento);
				
				HSSFCellStyle style = codigoDisciplinaToColorMap.get(atendimento.getDisciplinaString());
				if (style == null) {
					int index = codigoDisciplinaToColorMap.size() % excelColorsPool.size();
					codigoDisciplinaToColorMap.put(atendimento.getDisciplinaString(),excelColorsPool.get(index));
				}
			}
			
			int nextRow = this.initialRow;
			for (Sala sala : mapNivel1.keySet()) {
				Map<Turno,List<AtendimentoRelatorioDTO>> mapNivel2 = mapNivel1.get(sala);
				for (Turno turno : mapNivel2.keySet()) {
					nextRow = writeSala(sala,turno,mapNivel2.get(turno),nextRow,sheet,itExcelCommentsPool,codigoDisciplinaToColorMap);
				}				
			}
			
			//autoSizeColumns((short)1,(short)6,sheet); TODO: rever autoSize pois atualmente o algoritmo do poi interfere na largura do logo
			
			return true;
		}
		
		return false;
	}
	
	@SuppressWarnings("unused")
	private int writeSala(Sala sala, Turno turno, List<AtendimentoRelatorioDTO> atendimentos, int row, HSSFSheet sheet, Iterator<HSSFComment> itExcelCommentsPool, Map<String,HSSFCellStyle> codigoDisciplinaToColorMap) {
		row = writeHeader(sala,turno,row,sheet);
		
		int initialRow = row;
		int col = 2;
		
		// preenche grade com créditos e células vazias
		int maxCreditos = turno.calculaMaxCreditos();
		for (int indexCredito = 1; indexCredito <= maxCreditos; indexCredito++) {
			// Créditos
			setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],indexCredito);
			// Dias Semana
			for (Semanas semanas : Semanas.values()) {
				setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],"");
			}
			
			row++;
			col = 2;
		}
		
		// processa os atendimentos lidos do BD para que os mesmos sejam visualizados na visão sala
		AtendimentosServiceImpl atendimentosService = new AtendimentosServiceImpl();
		List<AtendimentoRelatorioDTO> atendimentosParaVisaoSala = atendimentosService.montaListaParaVisaoSala(atendimentos);
		
		// agrupa os atendimentos por dia da semana
		Map<Integer,List<AtendimentoRelatorioDTO>> diaSemanaToAtendimentosMap = new HashMap<Integer,List<AtendimentoRelatorioDTO>>();
		for (AtendimentoRelatorioDTO atendimento : atendimentosParaVisaoSala) {
			List<AtendimentoRelatorioDTO> list = diaSemanaToAtendimentosMap.get(atendimento.getSemana());
			if (list == null) {
				list = new ArrayList<AtendimentoRelatorioDTO>();
				diaSemanaToAtendimentosMap.put(atendimento.getSemana(),list);
			}
			list.add(atendimento);
		}		
		
		// para cada dia da semana, escreve os atendimentos no excel
		for (Integer diaSemanaInt : diaSemanaToAtendimentosMap.keySet()) {
			row = initialRow;
			
			Semanas diaSemana = Semanas.get(diaSemanaInt);
			switch (diaSemana) {
				case SEG: col = 3; break;
				case TER: col = 4; break;
				case QUA: col = 5; break;
				case QUI: col = 6; break;
				case SEX: col = 7; break;
				case SAB: col = 8; break;
				case DOM: col = 9; break;
			}

			for (AtendimentoRelatorioDTO atendimento : diaSemanaToAtendimentosMap.get(diaSemanaInt)) {
				HSSFCellStyle style = codigoDisciplinaToColorMap.get(atendimento.getDisciplinaString());
				// escreve célula principal
				setCell(row,col,sheet,style,itExcelCommentsPool,atendimento.getExcelContentVisaoSala(),atendimento.getExcelCommentVisaoSala());
				// une células de acordo com a quantidade de créditos
				mergeCells(row,(row+atendimento.getTotalCreditos()-1),col,col,sheet,style);
				
				row += atendimento.getTotalCreditos();
			}
		}
				
		return initialRow + maxCreditos + 1;
	}

	private int writeHeader(Sala sala, Turno turno, int row, HSSFSheet sheet) {
		int col = 3;
		
		// Campus
		setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal()],"Campus");
		setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal()],sala.getUnidade().getCampus().getCodigo());
		// Sala
		setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal()],"Sala");
		setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal()],sala.getCodigo());
		// Capacidade
		setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal()],"Capacidade");
		setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal()],sala.getCapacidade());
		
		row++;
		col = 3;
		
		// Unidade
		setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal()],"Unidade");
		setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal()],sala.getUnidade().getCodigo());
		// Turno
		setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal()],"Turno");
		setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal()],turno.getNome());
		// Tipo
		setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal()],"Tipo");
		setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal()],sala.getTipoSala().getNome());
		
		row++;
		col = 2;
		
		// Créditos
		setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal()],"Créditos");
		// Dias Semana
		for (Semanas semanas : Semanas.values()) {
			setCell(row,col++,sheet,cellStyles[ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal()],semanas.name());
		}
		
		row++;
		return row;
	}
	
	private void fillInCellStyles(HSSFSheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
	
	private List<HSSFCellStyle> buildColorPaletteCellStyles(HSSFWorkbook workbook) {
		List<HSSFCellStyle> colorPalleteCellStylesList = new ArrayList<HSSFCellStyle>();
		
		HSSFSheet sheet = workbook.getSheet(ExcelInformationType.PALETA_CORES.getSheetName());
		if (sheet != null) {
            for (int rowIndex = sheet.getFirstRowNum(); rowIndex <= sheet.getLastRowNum(); rowIndex++) {
            	HSSFRow row = sheet.getRow(rowIndex);
            	if (row != null) {
            		HSSFCell cell = row.getCell((int)row.getFirstCellNum());
            		if (cell != null) {
            			colorPalleteCellStylesList.add(cell.getCellStyle());
            		}
            	}
            }
		}
		
		removeUnusedSheet(ExcelInformationType.PALETA_CORES.getSheetName(),workbook);
		
		return colorPalleteCellStylesList;
	}
	
	private List<HSSFComment> buildExcelCommentsPool(HSSFWorkbook workbook) {
		List<HSSFComment> excelCommentsPool = new ArrayList<HSSFComment>();
		
		HSSFSheet sheet = workbook.getSheet(ExcelInformationType.RELATORIO_VISAO_SALA.getSheetName());
		if (sheet != null) {
            for (int rowIndex = sheet.getFirstRowNum(); rowIndex <= sheet.getLastRowNum(); rowIndex++) {
            	HSSFRow row = sheet.getRow(rowIndex);
            	if (row != null) {
            		HSSFCell cell = row.getCell(25);
            		if (cell != null && cell.getCellComment() != null) {
            			excelCommentsPool.add(cell.getCellComment());
            		}
            	}
            }
		}
		
		return excelCommentsPool;
	}
}