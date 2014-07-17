package com.gapso.web.trieda.server.excel.exp;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class SemanaLetivaHorariosExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT( 6, 2 );

		private int row;
		private int col;

		private ExcelCellStyleReference( int row, int col )
		{
			this.row = row;
			this.col = col;
		}

		public int getRow()
		{
			return row;
		}

		public int getCol()
		{
			return col;
		}
	}

	private CellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;

	public SemanaLetivaHorariosExportExcel(Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public SemanaLetivaHorariosExportExcel( boolean removeUnusedSheets,
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino,
		String fileExtension)
	{
		super( true, ExcelInformationType.SEMANA_LETIVA_HORARIOS.getSheetName(), cenario,i18nConstants,i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().semanaLetiva();
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
		return getI18nConstants().turnos();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook )
	{
		List< SemanaLetiva > semanas = SemanaLetiva.findByCenario(
			 	this.instituicaoEnsino, getCenario() );

		if (this.removeUnusedSheets) {
			removeUnusedSheets(this.getSheetName(),workbook);
		}
		
		if (!semanas.isEmpty()) {
			Sheet sheet = workbook.getSheet(this.getSheetName());
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			for ( SemanaLetiva s : semanas )
			{
				List<HorarioAula> haOrdenados = new ArrayList<HorarioAula>();
				haOrdenados.addAll(s.getHorariosAula());
				Collections.sort(haOrdenados,new Comparator<HorarioAula>() {
					@Override
					public int compare(HorarioAula o1, HorarioAula o2) {
						if ( o1.getTurno().getNome().compareTo(o2.getTurno().getNome()) == 0 )
						{
							return o1.getHorario().compareTo(o2.getHorario());
						}
						return o1.getTurno().getNome().compareTo(o2.getTurno().getNome());
					}
				});
				for ( HorarioAula ha : haOrdenados)
				{
					nextRow = writeData( s, ha, nextRow, sheet );
				}
			}

			return true;
		}
		
		return false;
	}

	private int writeData(SemanaLetiva s, HorarioAula ha, int row, Sheet sheet)
	{
		Set<Semanas> semanaDisponivel = new HashSet<Semanas>();
		for (HorarioDisponivelCenario hdc : ha.getHorariosDisponiveisCenario())
		{
			semanaDisponivel.add(hdc.getDiaSemana());
		}
		SimpleDateFormat sdf = new SimpleDateFormat("HH:mm");
		
		// Nome
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],s.getCodigo());
		// Turno
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],ha.getTurno().getNome());
		// Horario Inicial
		setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sdf.format(ha.getHorario()));
		
		// Segunda
		setCell(row,5,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],semanaDisponivel.contains(Semanas.SEG) ? getI18nConstants().sim() : HtmlUtils.htmlUnescape(getI18nConstants().nao()));
		// Terça
		setCell(row,6,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],semanaDisponivel.contains(Semanas.TER) ? getI18nConstants().sim() : HtmlUtils.htmlUnescape(getI18nConstants().nao()));
		// Quarta
		setCell(row,7,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],semanaDisponivel.contains(Semanas.QUA) ? getI18nConstants().sim() : HtmlUtils.htmlUnescape(getI18nConstants().nao()));
		// Quinta
		setCell(row,8,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],semanaDisponivel.contains(Semanas.QUI) ? getI18nConstants().sim() : HtmlUtils.htmlUnescape(getI18nConstants().nao()));
		// Sexta
		setCell(row,9,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],semanaDisponivel.contains(Semanas.SEX) ? getI18nConstants().sim() : HtmlUtils.htmlUnescape(getI18nConstants().nao()));
		// Sabado
		setCell(row,10,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],semanaDisponivel.contains(Semanas.SAB) ? getI18nConstants().sim() : HtmlUtils.htmlUnescape(getI18nConstants().nao()));
		// Domingo
		setCell(row,11,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],semanaDisponivel.contains(Semanas.DOM) ? getI18nConstants().sim() : HtmlUtils.htmlUnescape(getI18nConstants().nao()));
		
		row++;
		return row;
	}

	private void fillInCellStyles(Sheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}