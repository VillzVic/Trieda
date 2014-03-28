package com.gapso.web.trieda.server.excel.exp;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.ClientAnchor;
import org.apache.poi.ss.usermodel.Comment;
import org.apache.poi.ss.usermodel.CreationHelper;
import org.apache.poi.ss.usermodel.Drawing;
import org.apache.poi.ss.usermodel.Font;
import org.apache.poi.ss.usermodel.Hyperlink;
import org.apache.poi.ss.usermodel.IndexedColors;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.ss.util.CellRangeAddress;
import org.apache.poi.xssf.streaming.SXSSFWorkbook;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.google.gwt.dev.util.collect.HashSet;

@ProgressDeclarationAnnotation
public abstract class AbstractExportExcel implements IExportExcel {
	protected List<String> errors;
	protected List<String> warnings;
	private boolean autoSizeColumns;
	private Cenario cenario;
	private TriedaI18nConstants i18nConstants;
	private TriedaI18nMessages i18nMessages;
	private String sheetName;
	protected InstituicaoEnsino instituicaoEnsino;
	private CreationHelper factory;
	protected Drawing drawing;
	private Map<String,Map<String,Map<String,String>>> hyperlinksMap;
	private CellStyle hlinkStyle;
	protected String fileExtension;

	protected AbstractExportExcel(boolean autoSizeColumns, String sheetName, Cenario cenario,
		TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino,
		String fileExtension)
	{
		this.autoSizeColumns = autoSizeColumns;
		this.instituicaoEnsino = instituicaoEnsino;
		this.cenario = cenario;
		this.i18nConstants = i18nConstants;
		this.i18nMessages = i18nMessages;
		this.sheetName = sheetName;

		this.errors = new ArrayList< String >();
		this.warnings = new ArrayList< String >();
		this.hyperlinksMap = new HashMap<String,Map<String,Map<String,String>>>();
		
		this.fileExtension = fileExtension;
	}

	protected abstract String getPathExcelTemplate();
	protected abstract String getReportName();
	protected abstract boolean fillInExcel( Workbook workbook, Workbook templateWorkbook );

	@Override
	public Workbook export()
	{
		this.errors.clear();
		this.warnings.clear();

		Workbook workbook = null;
		Workbook templateWorkbook = null;
		try
		{
			workbook = getExcelTemplate( getPathExcelTemplate() );
			if ( !isXls() )
				templateWorkbook = getSimpleExcelTemplate( getPathExcelTemplate() );
		}
		catch ( Exception e )
		{
			e.printStackTrace();
			String msg = ( e.getMessage() + ( e.getCause() != null ?
				e.getCause().getMessage() : "" ) );

			this.errors.add( getI18nMessages().excelErroObterExcelTemplate(
				getPathExcelTemplate(), getReportName(), msg ) );
		}

		export( workbook, templateWorkbook );
		return workbook;
	}

	@Override
	public boolean export( Workbook workbook, Workbook templateWorkbook )
	{
		if ( workbook != null )
		{
			this.factory = workbook.getCreationHelper();
			this.drawing = null;
			this.errors.clear();
			this.warnings.clear();
			
			if (fillInExcel( workbook, templateWorkbook )) {
				if (this.autoSizeColumns) {
					if( isXls() )
						autoSizeColumns(workbook);
				}
				return true;
			}
		}

		return false;
	}

	private void autoSizeColumns(Workbook workbook) {
		Sheet sheet = workbook.getSheet(getSheetName());
		if (sheet != null) {
			autoSizeColumns((short)0,(short)20,sheet);			
		}
	}

	@Override
	public List< String > getErrors()
	{
		return this.errors;
	}

	@Override
	public List< String > getWarnings()
	{
		return this.warnings;
	}
	
	@Override
	public Map<String,Map<String,Map<String,String>>> getHyperlinksMap() {
		return this.hyperlinksMap;
	}
	
	@Override
	public void resolveHyperlinks(Map<String,Map<String,Map<String,String>>> hyperlinksMap, Workbook workbook) {}
	
	protected void registerHyperlink(String sheetTarget, String sheetOrigin, String key, String hyperlink) {
		Map<String,Map<String,String>> mapLevel2 = hyperlinksMap.get(sheetTarget);
		if (mapLevel2 == null) {
			mapLevel2 = new HashMap<String,Map<String,String>>();
			hyperlinksMap.put(sheetTarget,mapLevel2);
		}
		
		Map<String,String> mapLevel3 = mapLevel2.get(sheetOrigin);
		if (mapLevel3 == null) {
			mapLevel3 = new HashMap<String,String>();
			mapLevel2.put(sheetOrigin,mapLevel3);
		}
		
		mapLevel3.put(key,hyperlink);
	}
	
	protected void registerHyperlink(String[] sheetsTargets, String sheetOrigin, String key, String hyperlink) {
		for (String sheetTarget : sheetsTargets) {
			registerHyperlink(sheetTarget,sheetOrigin,key,hyperlink);
		}
	}
	
	protected String getSheetName() {
		return this.sheetName;
	}
	
	protected void setSheetName(String sheetName){
		this.sheetName = sheetName;
	}

	protected Cenario getCenario()
	{
		return this.cenario;
	}

	protected TriedaI18nConstants getI18nConstants()
	{
		return this.i18nConstants;
	}

	protected TriedaI18nMessages getI18nMessages()
	{
		return this.i18nMessages;
	}

	protected void autoSizeColumns( short firstColumn,
		short lastColumn, Sheet sheet )
	{
		for ( short col = firstColumn; col <= lastColumn; col++ )
		{
			sheet.autoSizeColumn( col );
		}
	}

	protected void removeUnusedSheets(
		String usedSheetName, Workbook workbook )
	{
		while ( workbook.getNumberOfSheets() > 1 )
		{
			int sheetIx = 0;
			while ( workbook.getSheetName( sheetIx ).equals( usedSheetName ) )
			{
				sheetIx++;
			}

			workbook.removeSheetAt( sheetIx );
		}
	}

	protected void removeUnusedSheets(
		List< String > usedSheetsName, Workbook workbook )
	{
		Set< Sheet > usedSheets = new HashSet< Sheet >();
		
		for (int sheetIndex = 0; sheetIndex < workbook.getNumberOfSheets(); sheetIndex++) {
			Sheet sheet = workbook.getSheetAt(sheetIndex);
			
			boolean isSheetUsed = false;
			for (String usedSheetName : usedSheetsName) {
				// é necessário verificar o caractere # para fazer a comparacao por conta da funcionalidade que utiliza
				// mais de uma aba para exportar relatórios da versão XLS com mais do que 65.535 linhas, pois
				// o nome das demais abas será o da aba original concatenado com "#1", "#2", ..., por exemplo, 
				// "Demandas por Aluno", "Demandas por Aluno#1", "Demandas por Aluno#2", ...
				String realName = (sheet.getSheetName().indexOf("#") != -1) ? sheet.getSheetName().substring(0, sheet.getSheetName().indexOf("#"))
						: sheet.getSheetName();
				if (realName.equals(usedSheetName)) {
					isSheetUsed = true;
					break;
				}
			}
			
			if (isSheetUsed) {
				usedSheets.add( sheet );
			}
		}

		int sheetIx = 0;
		while ( workbook.getNumberOfSheets() > usedSheets.size() )
		{
			Sheet sheet = workbook.getSheetAt( sheetIx );
			if ( !usedSheets.contains( sheet ) )
			{
				workbook.removeSheetAt( sheetIx );
				sheetIx = 0;
			} else
			{
				sheetIx++;
			}
		}
	}

	protected void mergeCells( int rowI, int rowF, int colI, int colF, Sheet sheet )
	{
		sheet.addMergedRegion( new CellRangeAddress( rowI - 1, rowF - 1, colI - 1, colF - 1 ) );
	}

	protected void mergeCells( int rowI, int rowF, int colI,
		int colF, Sheet sheet, CellStyle style )
	{
		for ( int row = rowI; row <= rowF; row++ )
		{
			for ( int col = colI; col <= colF; col++ )
			{
				getCell( row, col, sheet ).setCellStyle( style );
			}
		}

		sheet.addMergedRegion( new CellRangeAddress( rowI - 1, rowF - 1, colI - 1, colF - 1 ) );
	}

	protected void setCell( int row, int col, Sheet sheet, Calendar date )
	{
		final Cell cell = getCell( row, col, sheet );
		cell.setCellValue( date );
	}

	protected void setCell( int row, int col, Sheet sheet,
		CellStyle style, int mes, int ano )
	{
		final Cell cell = getCell( row, col, sheet );
		Calendar date = Calendar.getInstance();

		// Utilizado para limpar todas as
		// informações e não escrever no excel hh:mm:ss
		date.clear();

		date.set( ano, mes - 1, 1 );
		cell.setCellValue( date );
		cell.setCellStyle( style );
	}

	protected void setCell( int row, int col, Sheet sheet,
		CellStyle style, Calendar date )
	{
		final Cell cell = getCell( row ,col, sheet );
		cell.setCellValue( date );
		cell.setCellStyle( style );
	}

	protected void setCell(int row, int col, Sheet sheet,
		CellStyle style, Date date )
	{ 
		final Cell cell = getCell( row, col, sheet );
		cell.setCellValue( date );
		cell.setCellStyle( style );
	}

	protected void setCell( int row, int col, Sheet sheet, String value )
	{
		Cell cell = getCell( row, col, sheet );
		cell.setCellValue( getCreationHelper(sheet.getWorkbook()).createRichTextString( value ) );
	}

	protected void setCell( int row, int col, Sheet sheet,
		CellStyle style, String value )
	{
		Cell cell = getCell( row, col, sheet );
		cell.setCellValue( getCreationHelper(sheet.getWorkbook()).createRichTextString( value ) );
		cell.setCellStyle( style );
	}

	protected void setCell(int row, int col, Sheet sheet, CellStyle style, String value, String comment) {
		Cell cell = getCell( row, col, sheet );
		cell.setCellValue( getCreationHelper(sheet.getWorkbook()).createRichTextString( value ) );

		Comment cellComment = cell.getCellComment();
		if (cellComment == null) {
			ClientAnchor anchor = getCreationHelper(sheet.getWorkbook()).createClientAnchor();
		    anchor.setCol1(cell.getColumnIndex());
		    anchor.setCol2(cell.getColumnIndex()+4);
		    anchor.setRow1(row);
		    anchor.setRow2(row+12);
		    if (drawing == null) {
		    	drawing = sheet.createDrawingPatriarch();
		    }
			cellComment = drawing.createCellComment(anchor);
			cell.setCellComment(cellComment);
		}

		cellComment.setString(getCreationHelper(sheet.getWorkbook()).createRichTextString(comment));
		cell.setCellStyle(style);
	}
	
	protected void setCellWithHyperlink(int row, int col, Sheet sheet, String value, String hyperlink, boolean withHyperlinkStyle) {
		Cell cell = getCell(row,col,sheet);
		if (value != null) {
			cell.setCellValue(getCreationHelper(sheet.getWorkbook()).createRichTextString(value));
		}
		
		if (withHyperlinkStyle) {
			if (hlinkStyle == null) {
				Workbook wb = sheet.getWorkbook();
				hlinkStyle = wb.createCellStyle();
				Font hlinkFont = wb.createFont();
				hlinkFont.setUnderline(Font.U_SINGLE);
				hlinkFont.setColor(IndexedColors.BLUE.getIndex());
				hlinkStyle.setFont(hlinkFont);
				hlinkStyle.setBorderBottom(CellStyle.BORDER_THIN);
				hlinkStyle.setBorderLeft(CellStyle.BORDER_THIN);
				hlinkStyle.setBorderRight(CellStyle.BORDER_THIN);
				hlinkStyle.setBorderTop(CellStyle.BORDER_THIN);
			}
			cell.setCellStyle(hlinkStyle);
		}
		
		Hyperlink link = getCreationHelper(sheet.getWorkbook()).createHyperlink(Hyperlink.LINK_DOCUMENT);
		link.setAddress(hyperlink);
		cell.setHyperlink(link);
	}
	
	protected void setCellWithHyperlink(int row, int col, Sheet sheet, String hyperlink, boolean withHyperlinkStyle) {
		setCellWithHyperlink(row,col,sheet,null,hyperlink,withHyperlinkStyle);
	}

	protected void setCell( int row, int col, Sheet sheet, CellStyle style,
		Iterator< Comment > itExcelCommentsPool, String value, String comment )
	{
		Cell cell = getCell( row, col, sheet );
		cell.setCellValue( getCreationHelper(sheet.getWorkbook()).createRichTextString( value ) );

		Comment cellComment = cell.getCellComment();
		if ( cellComment == null )
		{
			if ( itExcelCommentsPool.hasNext() )
			{
				cellComment = itExcelCommentsPool.next();
				cell.setCellComment( cellComment );
				cellComment.setString( getCreationHelper(sheet.getWorkbook()).createRichTextString( comment ) );
			}
		}
		else
		{
			cellComment.setString( getCreationHelper(sheet.getWorkbook()).createRichTextString( comment ) );
		}

		cell.setCellStyle( style );
	}

	protected void setCell( int row, int col, Sheet sheet, double value )
	{
		Cell cell = getCell( row, col, sheet );
		cell.setCellValue( value );
	}

	protected void setCell( int row, int col, Sheet sheet,
		CellStyle style, double value )
	{
		Cell cell = getCell(row, col, sheet );
		cell.setCellValue( value );
		cell.setCellStyle( style );
	}

	protected Cell getCell( int row, int col, Sheet sheet )
	{
		final Row ssRow = getRow( row - 1, sheet );
		final Cell cell = getCell( ssRow, ( col - 1 ) );
		return cell;
	}
	
	protected Sheet restructuringWorkbookIfRowLimitIsViolated(int actualRow, int numberOfRowsThatWillBeWritten, Sheet sheet) {
		if ((actualRow + numberOfRowsThatWillBeWritten) >= 65536) {
			Workbook workbook = sheet.getWorkbook();
			
			String sheetName = sheet.getSheetName();
			String[] splitSheetName = sheetName.split("#");
			String newSheetNamePart1 = splitSheetName[0];
			String newSheetNamePart2 = "";
			if (splitSheetName.length == 2) {
				String s = splitSheetName[1];
				int number = Integer.parseInt(s);
				number++;
				newSheetNamePart2 = Integer.toString(number);
			} else {
				newSheetNamePart2 = "1";
			}
			
			Sheet newSheet = workbook.createSheet(newSheetNamePart1+"#"+newSheetNamePart2);
			drawing = newSheet.createDrawingPatriarch();
			return newSheet;
		}
		return null;
	}
	
	private CreationHelper getCreationHelper(Workbook workbook) {
		if (this.factory == null) {
			this.factory = workbook.getCreationHelper();
		}
		return this.factory;
	}

	private Row getRow( int index, Sheet sheet )
	{
		Row row = sheet.getRow( index );
		if ( row == null )
		{
			row = sheet.createRow( index );
		}

		return row;
	}

	private Cell getCell( Row row, int col )
	{
		Cell cell = row.getCell( col );
		if ( cell == null )
		{
			cell = row.createCell( col );
		}

		return cell;
	}

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
				workBook = new SXSSFWorkbook(new XSSFWorkbook(inTemplate));
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
	
	private Workbook getSimpleExcelTemplate( String pathExcelTemplate )
			throws IOException
		{
			final InputStream inTemplate
				= ExportExcelServlet.class.getResourceAsStream( pathExcelTemplate );

			Workbook workBook = null;
			try
			{
					workBook = new XSSFWorkbook(inTemplate);
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
	
	protected Boolean isXls()
	{
		if( fileExtension.equals("xls") )
		{
			return true;
		}
		return false;
	}
}
