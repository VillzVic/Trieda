package com.gapso.web.trieda.server.excel.exp;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

import com.extjs.gxt.ui.client.data.BaseTreeModel;
import com.extjs.gxt.ui.client.data.ModelData;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.TriedaPar;
import com.gapso.web.trieda.server.CampiServiceImpl;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;

public class CompararCenariosExportExcel
		extends AbstractExportExcel
{
	enum ExcelCellStyleReference {
		HEADER(5, 5);
		

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

	private CompararCenariosFiltroExcel filter;
	private CellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;
	private int initialColumn;
	private Set<Integer> rowsEspaco = new HashSet<Integer>();
	private Map<Integer, TriedaPar<CellStyle, CellStyle>> rowToStylesmap = new HashMap<Integer, TriedaPar<CellStyle, CellStyle>>();
	private List<CenarioDTO> cenariosDTO;
	

	public CompararCenariosExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		ExportExcelFilter filter, InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension );
	}

	public CompararCenariosExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		ExportExcelFilter filter, InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( false, ExcelInformationType.COMPARAR_CENARIOS.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 4;
		initialColumn = 0;
		getResumoCampiEspacamento(rowsEspaco);
		this.setFilter( filter );
	}

	@Override
	public String getFileName() {
		return getI18nConstants().compararCenarios();
	}

	@Override
	protected String getPathExcelTemplate() {
		if ( fileExtension.equals("xlsx") )
		{
			return "/templateExport.xlsx";
		}
		else
			return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().compararCenarios();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook ) {
		
		cenariosDTO = filter.getCenariosDTO();
		
		CampiServiceImpl campiServiceImpl = new CampiServiceImpl();
		
		if (this.removeUnusedSheets) {
			removeUnusedSheets(this.getSheetName(),workbook);
		}
		if (!cenariosDTO.isEmpty()) {
			Sheet sheet = workbook.getSheet(this.getSheetName());
			Sheet templateSheet = null;
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			int nextColumn = this.initialColumn + 4;
			List<BaseTreeModel> resumo = campiServiceImpl.getCenariosComparados(cenariosDTO);
			for (BaseTreeModel node : resumo) {
				nextRow += 1;
				List<ModelData> models = node.getChildren();
				nextRow = writeHeader(node,nextRow,nextColumn,sheet);
				int rowEspacoIndex = this.initialRow + 1;
				for (ModelData data : models)
				{
					if (rowsEspaco.contains(rowEspacoIndex))
					{
						nextRow++;
						rowEspacoIndex++;
					}
					nextRow = writeData(data,nextRow, nextColumn, sheet, templateSheet);
					rowEspacoIndex++;
					
				}
			}
			removeRemainingRows(sheet, nextRow);
			autoSizeColumns((short)3, (short)20, sheet);
			return true;
		}

		return false;
	}

	private void removeRemainingRows(Sheet sheet, int nextRow) {
		int lastRowNum = sheet.getLastRowNum();
		for (int i = 0; i<lastRowNum - nextRow; i++)
		{
			Row lastRow =  sheet.getRow(sheet.getLastRowNum());
			sheet.removeRow( lastRow );
		}
	}

	private int writeData(ModelData node, int row, int column, Sheet sheet, Sheet templateSheet) {
		if (rowToStylesmap.get(row) == null)
		{
			CellStyle newStyle;
			CellStyle newStyle2;
			newStyle = getCell(row,column,sheet).getCellStyle();
			newStyle2 = getCell(row,column+1,sheet).getCellStyle();

			rowToStylesmap.put(row, TriedaPar.create(newStyle, newStyle2));
		}
					
		
		setCell(row,column,sheet,rowToStylesmap.get(row).getPrimeiro(), node.get("label").toString());
		int nextColumn = 1;
		for(CenarioDTO cenarioDTO : cenariosDTO)
		{
			String value = node.get(cenarioDTO.getNome()) == null ? "" : node.get(cenarioDTO.getNome()).toString().replace("<b>", "").replace("</b>", "");
			setCell(row,column+nextColumn,sheet,rowToStylesmap.get(row).getSegundo(),value);
			nextColumn++;
		}

		row++;
		return row;
	}
	
	private int writeHeader(BaseTreeModel campus, int row, int column, Sheet sheet) {
		// Node
		int nextColumn = 1;
		for(CenarioDTO cenarioDTO : cenariosDTO)
		{
			setCell(row,column+nextColumn,sheet,cellStyles[ExcelCellStyleReference.HEADER.ordinal()],cenarioDTO.getNome() + "\n" + campus.get("label").toString());
			nextColumn++;
		}

		return row + 1;
	}

	private void fillInCellStyles(Sheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
	
	@Override
	protected Workbook getExcelTemplate( String pathExcelTemplate )
		throws IOException
	{
		final InputStream inTemplate
			= ExportExcelServlet.class.getResourceAsStream( pathExcelTemplate );

		Workbook workBook = null;
		try
		{
			if ( fileExtension.equals("xls") )
			{
				workBook = new HSSFWorkbook( inTemplate );
			}
			else if ( fileExtension.equals("xlsx") )
			{
				workBook = new XSSFWorkbook(inTemplate);
			}
		}
		catch ( IOException e )
		{
			throw e;
		}
		finally
		{
			if ( inTemplate != null )
			{
				inTemplate.close();
			}
		}

		return workBook;
	}
	
	public CompararCenariosFiltroExcel getFilter()
	{
		return filter;
	}

	public void setFilter( ExportExcelFilter filter )
	{
		if ( filter != null && filter instanceof CompararCenariosFiltroExcel )
		{
			this.filter = (CompararCenariosFiltroExcel) filter;
		}
		else
		{
			this.filter = null;
		}
	}
}