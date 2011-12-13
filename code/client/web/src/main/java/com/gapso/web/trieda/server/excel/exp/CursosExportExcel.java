package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class CursosExportExcel extends AbstractExportExcel {
	
	enum ExcelCellStyleReference {
		TEXT(6,2),
		NUMBER_DOUBLE(6,5),
		NUMBER_INT(6,7);
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
	private int initialRow;
	
	public CursosExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino );
	}

	public CursosExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( true, ExcelInformationType.CURSOS.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().cursos();
	}
	
	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().cursos();
	}

	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		List< Curso > cursos = Curso.findByCenario(
			this.instituicaoEnsino, getCenario() );
		
		if (!cursos.isEmpty()) {
			if (this.removeUnusedSheets) {
				removeUnusedSheets(this.getSheetName(),workbook);
			}
			HSSFSheet sheet = workbook.getSheet(this.getSheetName());
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			for (Curso curso : cursos) {
				nextRow = writeData(curso,nextRow,sheet);
			}

			return true;
		}
		
		return false;
	}
	
	private int writeData(Curso curso, int row, HSSFSheet sheet) {
		// Codigo
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],curso.getCodigo());
		// Nome
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],curso.getNome());
		// Tipo
		setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],curso.getTipoCurso().getCodigo());
		// % Min PhD
		setCell(row,5,sheet,cellStyles[ExcelCellStyleReference.NUMBER_DOUBLE.ordinal()],curso.getNumMinDoutores());
		// % Min MSc
		setCell(row,6,sheet,cellStyles[ExcelCellStyleReference.NUMBER_DOUBLE.ordinal()],curso.getNumMinMestres());
		// % min Tempo Integral + Parcial
		setCell(row,7,sheet,cellStyles[ExcelCellStyleReference.NUMBER_DOUBLE.ordinal()],curso.getMinTempoIntegralParcial());
		// % min Tempo Integral
		setCell(row,8,sheet,cellStyles[ExcelCellStyleReference.NUMBER_DOUBLE.ordinal()],curso.getMinTempoIntegral());
		// Max Disciplinas Professor
		setCell(row,9,sheet,cellStyles[ExcelCellStyleReference.NUMBER_INT.ordinal()],curso.getMaxDisciplinasPeloProfessor());
		// Permite mais de uma Disciplina por Professor
		setCell(row,10,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],(curso.getAdmMaisDeUmDisciplina() ? getI18nConstants().sim() : HtmlUtils.htmlUnescape(getI18nConstants().nao())));
		
		row++;
		return row;
	}
	
	private void fillInCellStyles(HSSFSheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}


