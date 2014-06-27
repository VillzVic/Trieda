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
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.TriedaPar;
import com.gapso.web.trieda.server.CampiServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.ResumoDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class ResumoCampiExportExcel
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

	private CellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;
	private int initialColumn;
	private Set<Integer> rowsEspaco = new HashSet<Integer>();
	private Map<Integer, TriedaPar<CellStyle, CellStyle>> rowToStylesmap = new HashMap<Integer, TriedaPar<CellStyle, CellStyle>>();
	

	public ResumoCampiExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public ResumoCampiExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( false, ExcelInformationType.RESUMO_CAMPI.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 5;
		initialColumn = 0;
		rowsEspaco.add(26);
		rowsEspaco.add(35);
		rowsEspaco.add(44);
		rowsEspaco.add(57);
	}

	@Override
	public String getFileName() {
		return getI18nConstants().resumoCampi();
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
		return getI18nConstants().resumoCampi();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook ) {
		List<Campus> campi = Campus.findAll(instituicaoEnsino);
		
		CampiServiceImpl campiServiceImpl = new CampiServiceImpl();
		
		if (this.removeUnusedSheets) {
			removeUnusedSheets(this.getSheetName(),workbook);
		}
		if (!campi.isEmpty()) {
			Sheet sheet = workbook.getSheet(this.getSheetName());
			Sheet templateSheet = null;
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			int nextColumn = this.initialColumn + 4;
			List<TreeNodeDTO> resumo = campiServiceImpl.getResumos(ConvertBeans.toCenarioDTO(getCenario()), null);
			for (TreeNodeDTO node : resumo) {
				List<TreeNodeDTO> campus = campiServiceImpl.getResumos(ConvertBeans.toCenarioDTO(getCenario()), node);
				nextRow = writeHeader(node,nextRow,nextColumn,sheet);
				for (TreeNodeDTO node2 : campus)
				{
					if (rowsEspaco.contains(nextRow))
					{
						nextRow++;
					}
					nextRow = writeData(node2,nextRow, nextColumn, sheet, templateSheet);
				}
				nextColumn += 1;
			}
			autoSizeColumns((short)3, (short)20, sheet);
			return true;
		}

		return false;
	}

	private int writeData(TreeNodeDTO node, int row, int column, Sheet sheet, Sheet templateSheet) {
		if (rowToStylesmap.get(row) == null)
		{
			CellStyle newStyle;
			CellStyle newStyle2;
			newStyle = getCell(row,column,sheet).getCellStyle();
			newStyle2 = getCell(row,column+1,sheet).getCellStyle();

			rowToStylesmap.put(row, TriedaPar.create(newStyle, newStyle2));
		}
					
		
		if (column == this.initialColumn + 4)
			setCell(row,column,sheet,rowToStylesmap.get(row).getPrimeiro(),((ResumoDTO) node.getContent()).getLabel());
		
		setCell(row,column+1,sheet,rowToStylesmap.get(row).getSegundo(),((ResumoDTO) node.getContent()).getValor().replace("<b>", "").replace("</b>", ""));

		row++;
		return row;
	}
	
	private int writeHeader(TreeNodeDTO campus, int row, int column, Sheet sheet) {
		// Node
		setCell(this.initialRow,column+1,sheet,cellStyles[ExcelCellStyleReference.HEADER.ordinal()],getCenario().getNome() + "\n" + campus.getText());

		return this.initialRow + 1;
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
}