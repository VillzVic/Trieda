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

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFComment;
import org.apache.poi.hssf.usermodel.HSSFRichTextString;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.ClientAnchor;
import org.apache.poi.ss.usermodel.Comment;
import org.apache.poi.ss.usermodel.CreationHelper;
import org.apache.poi.ss.usermodel.Drawing;
import org.apache.poi.ss.usermodel.Font;
import org.apache.poi.ss.usermodel.Hyperlink;
import org.apache.poi.ss.usermodel.IndexedColors;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.ss.util.CellRangeAddress;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.google.gwt.dev.util.collect.HashSet;

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
	private Drawing drawing;
	private Map<String,Map<String,Map<String,String>>> hyperlinksMap;
	private CellStyle hlinkStyle;

	protected AbstractExportExcel(boolean autoSizeColumns, String sheetName, Cenario cenario,
		TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
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
	}

	protected abstract String getPathExcelTemplate();
	protected abstract String getReportName();
	protected abstract boolean fillInExcel( HSSFWorkbook workbook );

	@Override
	public HSSFWorkbook export()
	{
		this.errors.clear();
		this.warnings.clear();

		HSSFWorkbook workbook = null;
		try
		{
			workbook = getExcelTemplate( getPathExcelTemplate() );
		}
		catch ( Exception e )
		{
			e.printStackTrace();
			String msg = ( e.getMessage() + ( e.getCause() != null ?
				e.getCause().getMessage() : "" ) );

			this.errors.add( getI18nMessages().excelErroObterExcelTemplate(
				getPathExcelTemplate(), getReportName(), msg ) );
		}

		export( workbook );
		return workbook;
	}

	@Override
	public boolean export( HSSFWorkbook workbook )
	{
		if ( workbook != null )
		{
			this.factory = workbook.getCreationHelper();
			this.drawing = null;
			this.errors.clear();
			this.warnings.clear();
			
			if (fillInExcel( workbook )) {
				if (this.autoSizeColumns) {
					autoSizeColumns(workbook);
				}
				return true;
			}
		}

		return false;
	}

	private void autoSizeColumns(HSSFWorkbook workbook) {
		HSSFSheet sheet = workbook.getSheet(getSheetName());
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
	public void resolveHyperlinks(Map<String,Map<String,Map<String,String>>> hyperlinksMap, HSSFWorkbook workbook) {}
	
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
		short lastColumn, HSSFSheet sheet )
	{
		for ( short col = firstColumn; col <= lastColumn; col++ )
		{
			sheet.autoSizeColumn( col );
		}
	}

	protected void removeUnusedSheets(
		String usedSheetName, HSSFWorkbook workbook )
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
		List< String > usedSheetsName, HSSFWorkbook workbook )
	{
		Set< HSSFSheet > usedSheets = new HashSet< HSSFSheet >();
		for ( String usedSheetName : usedSheetsName )
		{
			HSSFSheet sheet = workbook.getSheet( usedSheetName );
			if ( sheet != null )
			{
				usedSheets.add( sheet );
			}
		}

		int sheetIx = 0;
		while ( workbook.getNumberOfSheets() > usedSheets.size() )
		{
			HSSFSheet sheet = workbook.getSheetAt( sheetIx++ );
			if ( !usedSheets.contains( sheet ) )
			{
				workbook.removeSheetAt( sheetIx );
				sheetIx = 0;
			}
		}
	}

	protected void mergeCells( int rowI, int rowF, int colI, int colF, HSSFSheet sheet )
	{
		sheet.addMergedRegion( new CellRangeAddress( rowI - 1, rowF - 1, colI - 1, colF - 1 ) );
	}

	protected void mergeCells( int rowI, int rowF, int colI,
		int colF, HSSFSheet sheet, HSSFCellStyle style )
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

	protected void setCell( int row, int col, HSSFSheet sheet, Calendar date )
	{
		final HSSFCell cell = getCell( row, col, sheet );
		cell.setCellValue( date );
	}

	protected void setCell( int row, int col, HSSFSheet sheet,
		HSSFCellStyle style, int mes, int ano )
	{
		final HSSFCell cell = getCell( row, col, sheet );
		Calendar date = Calendar.getInstance();

		// Utilizado para limpar todas as
		// informações e não escrever no excel hh:mm:ss
		date.clear();

		date.set( ano, mes - 1, 1 );
		cell.setCellValue( date );
		cell.setCellStyle( style );
	}

	protected void setCell( int row, int col, HSSFSheet sheet,
		HSSFCellStyle style, Calendar date )
	{
		final HSSFCell cell = getCell( row ,col, sheet );
		cell.setCellValue( date );
		cell.setCellStyle( style );
	}

	protected void setCell(int row, int col, HSSFSheet sheet,
		HSSFCellStyle style, Date date )
	{ 
		final HSSFCell cell = getCell( row, col, sheet );
		cell.setCellValue( date );
		cell.setCellStyle( style );
	}

	protected void setCell( int row, int col, HSSFSheet sheet, String value )
	{
		HSSFCell cell = getCell( row, col, sheet );
		cell.setCellValue( new HSSFRichTextString( value ) );
	}

	protected void setCell( int row, int col, HSSFSheet sheet,
		HSSFCellStyle style, String value )
	{
		HSSFCell cell = getCell( row, col, sheet );
		cell.setCellValue( new HSSFRichTextString( value ) );
		cell.setCellStyle( style );
	}

	protected void setCell(int row, int col, HSSFSheet sheet, HSSFCellStyle style, String value, String comment) {
		HSSFCell cell = getCell( row, col, sheet );
		cell.setCellValue( new HSSFRichTextString( value ) );

		Comment cellComment = cell.getCellComment();
		if (cellComment == null) {
			ClientAnchor anchor = factory.createClientAnchor();
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

		cellComment.setString(factory.createRichTextString(comment));
		cell.setCellStyle(style);
	}
	
	protected void setCellWithHyperlink(int row, int col, HSSFSheet sheet, String value, String hyperlink, boolean withHyperlinkStyle) {
		HSSFCell cell = getCell(row,col,sheet);
		if (value != null) {
			cell.setCellValue(new HSSFRichTextString(value));
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
		
		Hyperlink link = factory.createHyperlink(Hyperlink.LINK_DOCUMENT);
		link.setAddress(hyperlink);
		cell.setHyperlink(link);
	}
	
	protected void setCellWithHyperlink(int row, int col, HSSFSheet sheet, String hyperlink, boolean withHyperlinkStyle) {
		setCellWithHyperlink(row,col,sheet,null,hyperlink,withHyperlinkStyle);
	}

	protected void setCell( int row, int col, HSSFSheet sheet, HSSFCellStyle style,
		Iterator< HSSFComment > itExcelCommentsPool, String value, String comment )
	{
		HSSFCell cell = getCell( row, col, sheet );
		cell.setCellValue( new HSSFRichTextString( value ) );

		HSSFComment cellComment = cell.getCellComment();
		if ( cellComment == null )
		{
			if ( itExcelCommentsPool.hasNext() )
			{
				cellComment = itExcelCommentsPool.next();
				cell.setCellComment( cellComment );
				cellComment.setString( new HSSFRichTextString( comment ) );
			}
		}
		else
		{
			cellComment.setString( new HSSFRichTextString( comment ) );
		}

		cell.setCellStyle( style );
	}

	protected void setCell( int row, int col, HSSFSheet sheet, double value )
	{
		HSSFCell cell = getCell( row, col, sheet );
		cell.setCellValue( value );
	}

	protected void setCell( int row, int col, HSSFSheet sheet,
		HSSFCellStyle style, double value )
	{
		HSSFCell cell = getCell(row, col, sheet );
		cell.setCellValue( value );
		cell.setCellStyle( style );
	}

	protected HSSFCell getCell( int row, int col, HSSFSheet sheet )
	{
		final HSSFRow hssfRow = getRow( row - 1, sheet );
		final HSSFCell cell = getCell( hssfRow, ( col - 1 ) );
		return cell;
	}

	private HSSFRow getRow( int index, HSSFSheet sheet )
	{
		HSSFRow row = sheet.getRow( index );
		if ( row == null )
		{
			row = sheet.createRow( index );
		}

		return row;
	}

	private HSSFCell getCell( HSSFRow row, int col )
	{
		HSSFCell cell = row.getCell( col );
		if ( cell == null )
		{
			cell = row.createCell( col );
		}

		return cell;
	}

	private HSSFWorkbook getExcelTemplate( String pathExcelTemplate )
		throws IOException
	{
		final InputStream inTemplate
			= ExportExcelServlet.class.getResourceAsStream( pathExcelTemplate );

		HSSFWorkbook workBook = null;
		try
		{
			workBook = new HSSFWorkbook( inTemplate );
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
