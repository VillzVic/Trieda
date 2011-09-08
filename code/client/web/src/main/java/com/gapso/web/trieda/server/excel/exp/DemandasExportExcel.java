package com.gapso.web.trieda.server.excel.exp;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class DemandasExportExcel extends AbstractExportExcel {
	
	enum ExcelCellStyleReference {
		TEXT(6,2),
		NUMBER(6,6);
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

	public DemandasExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino );
	}

	public DemandasExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );

		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.DEMANDAS.getSheetName();
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().ofertasEDemandas();
	}
	
	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().ofertasEDemandas();
	}

	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		List< Oferta > ofertas = Oferta.findByCenario(
				this.instituicaoEnsino, getCenario() );

		if ( !ofertas.isEmpty() )
		{
			if ( this.removeUnusedSheets )
			{
				removeUnusedSheets( this.sheetName, workbook );
			}

			HSSFSheet sheet = workbook.getSheet( this.sheetName );
			fillInCellStyles( sheet );

			int nextRow = this.initialRow;
			for ( Oferta oferta : ofertas )
			{
				nextRow = writeData( oferta, nextRow,sheet );
			}

			return true;
		}

		return false;
	}
	
	private int writeData( Oferta oferta, int row, HSSFSheet sheet )
	{
		if ( oferta.getDemandas().isEmpty() )
		{
			// Campus
			setCell( row, 2, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				oferta.getCampus().getCodigo() );

			// Turno
			setCell( row, 3, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				oferta.getTurno().getNome() );

			row++;
		}
		else
		{
			// [ CodDisciplina -> Demanda ]
			Map< String, Demanda > demandasMap
				= new HashMap< String, Demanda >();

			for ( Demanda demanda : oferta.getDemandas() )
			{
				demandasMap.put( demanda.getDisciplina().getCodigo(), demanda );
			}

			Curriculo curriculo = oferta.getCurriculo();
			List< Integer > listPeriodos = null;

			if ( curriculo != null )
			{
				listPeriodos = curriculo.getPeriodos( this.instituicaoEnsino );
			}

			if ( listPeriodos != null && listPeriodos.size() > 0 )
			{
				for ( Integer periodo : listPeriodos )
				{
					List< CurriculoDisciplina > disciplinasDeUmPeriodo
						= curriculo.getCurriculoDisciplinasByPeriodo(
							this.instituicaoEnsino, periodo );

					if ( disciplinasDeUmPeriodo == null
						|| disciplinasDeUmPeriodo.size() == 0 )
					{
						continue;
					}

					for ( CurriculoDisciplina disciplinaDeUmPeriodo : disciplinasDeUmPeriodo )
					{
						// Campus
						setCell( row, 2, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
							oferta.getCampus().getCodigo() );

						// Turno
						setCell( row, 3, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
							oferta.getTurno().getNome() );

						// Curso
						setCell( row, 4, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
							curriculo.getCurso().getCodigo() );

						// Matriz Curricular
						setCell( row, 5, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
							curriculo.getCodigo() );

						// Período
						setCell( row, 6, sheet, cellStyles[ ExcelCellStyleReference.NUMBER.ordinal()], periodo );

						// Disciplina
						setCell( row, 7, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
							disciplinaDeUmPeriodo.getDisciplina().getCodigo() );

						// Demanda de Alunos
						Demanda demanda = demandasMap.get(
							disciplinaDeUmPeriodo.getDisciplina().getCodigo() );

						setCell( row, 8, sheet, cellStyles[ ExcelCellStyleReference.NUMBER.ordinal() ],
							( demanda != null ? demanda.getQuantidade() : 0 ) );

						row++;
					}
				}
			}
		}

		return row;
	}
	
	private void fillInCellStyles(HSSFSheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}