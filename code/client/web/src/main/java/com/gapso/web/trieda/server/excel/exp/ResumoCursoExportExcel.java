package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.extjs.gxt.ui.client.data.ModelData;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.web.trieda.server.CursosServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ResumoCursoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class ResumoCursoExportExcel extends AbstractExportExcel {
	
	enum ExcelCellStyleReference {
		TEXT(6,2),
		INTEGER(6,6),
		DOUBLE(6,15);
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
	
	public ResumoCursoExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		super(cenario,i18nConstants,i18nMessages);
		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = true;
		this.sheetName = ExcelInformationType.RESUMO_CURSO.getSheetName();
		this.initialRow = 6;
	}
	
	public ResumoCursoExportExcel(boolean removeUnusedSheets, Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		super(cenario,i18nConstants,i18nMessages);
		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.RESUMO_CURSO.getSheetName();
		this.initialRow = 6;
	}
	
	@Override
	public String getFileName() {
		return getI18nConstants().resumoCurso();
	}
	
	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().resumoCurso();
	}

	@Override
	protected boolean fillInExcel(HSSFWorkbook workbook) {
		CenarioDTO cenarioDTO = ConvertBeans.toCenarioDTO(getCenario());
		List<Campus> campi = Campus.findAll();
		List<CampusDTO> campusDTOList = new ArrayList<CampusDTO>(campi.size());
		for(Campus campus : campi) {
			campusDTOList.add(ConvertBeans.toCampusDTO(campus));
		}
		
		List<ResumoCursoDTO> resumoCursoDTOList = new ArrayList<ResumoCursoDTO>();
		
		CursosServiceImpl cursosServiceImpl = new CursosServiceImpl();
		for(CampusDTO campusDTO : campusDTOList) {
			resumoCursoDTOList.addAll(cursosServiceImpl.getResumos(cenarioDTO, campusDTO));
		}
		
		if (!resumoCursoDTOList.isEmpty()) {
			if (this.removeUnusedSheets) {
				removeUnusedSheets(this.sheetName,workbook);
			}
			HSSFSheet sheet = workbook.getSheet(this.sheetName);
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			for(ResumoCursoDTO resumoCursoDTO1 : resumoCursoDTOList) {
				for(ModelData resumoCursoDTO2 : resumoCursoDTO1.getChildren()) {
					for(ModelData resumoCursoDTO3 : ((ResumoCursoDTO)resumoCursoDTO2).getChildren()) {
						nextRow = writeData((ResumoCursoDTO)resumoCursoDTO3 ,nextRow,sheet);
					}
				}
			}

			return true;
		}
		
		return false;
	}
	
	private int writeData(ResumoCursoDTO resumoCursoDTO, int row, HSSFSheet sheet) {
		int i = 2;
		// Campus
		setCell(row,i++,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],resumoCursoDTO.getCampusString());
		// Turno
		setCell(row,i++,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],resumoCursoDTO.getTurnoString());
		// Curso
		setCell(row,i++,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],resumoCursoDTO.getCursoString());
		// Matriz Curricular
		setCell(row,i++,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],resumoCursoDTO.getMatrizCurricularString());
		// Período
		setCell(row,i++,sheet,cellStyles[ExcelCellStyleReference.INTEGER.ordinal()],resumoCursoDTO.getPeriodo());
		// Disciplina
		setCell(row,i++,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],resumoCursoDTO.getDisciplinaString());
		// Turma
		setCell(row,i++,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],resumoCursoDTO.getTurma());
		// Tipo de Crédito
		String tipoDeCredito = resumoCursoDTO.getTipoCreditoTeorico()? getI18nConstants().teorico() : getI18nConstants().pratico();
		setCell(row,i++,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],tipoDeCredito);
		// Créditos
		setCell(row,i++,sheet,cellStyles[ExcelCellStyleReference.INTEGER.ordinal()],resumoCursoDTO.getCreditos());
		// Qtde. Alunos
		setCell(row,i++,sheet,cellStyles[ExcelCellStyleReference.INTEGER.ordinal()],resumoCursoDTO.getQuantidadeAlunos());
		// Rateio
		setCell(row,i++,sheet,cellStyles[ExcelCellStyleReference.DOUBLE.ordinal()],resumoCursoDTO.getRateio());
		// Custo Docente
		setCell(row,i++,sheet,cellStyles[ExcelCellStyleReference.DOUBLE.ordinal()],resumoCursoDTO.getCustoDocente());
		// Receita
		setCell(row,i++,sheet,cellStyles[ExcelCellStyleReference.DOUBLE.ordinal()],resumoCursoDTO.getReceita());
		// Margem
		setCell(row,i++,sheet,cellStyles[ExcelCellStyleReference.DOUBLE.ordinal()],resumoCursoDTO.getMargem());
		// Margem %
		setCell(row,i++,sheet,cellStyles[ExcelCellStyleReference.DOUBLE.ordinal()],resumoCursoDTO.getMargemPercente());
		
		row++;
		return row;
	}
	
	private void fillInCellStyles(HSSFSheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}