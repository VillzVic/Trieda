package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.ProfessoresServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;
import com.gapso.web.trieda.shared.util.view.TriedaException;

@ProgressDeclarationAnnotation
public class ProfessoresTitulacoesExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT( 6, 2 ),
		INTEGER( 6, 3 );

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
	
	private CellStyle[] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;
	private AtendimentosFaixaDemandaFiltroExcel filter;
	
	public ProfessoresTitulacoesExportExcel( Cenario cenario, TriedaI18nConstants i18nConstants,
			TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension );
	}
	
	public ProfessoresTitulacoesExportExcel( boolean removeUnusedSheets, Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, 
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino, fileExtension );
	}

	public ProfessoresTitulacoesExportExcel( boolean removeUnusedSheets,
			Cenario cenario, TriedaI18nConstants i18nConstants,
			TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.PROFESSORES_TITULACOES.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
		this.setFilter( filter );
	}
	
	@Override
	public String getFileName()
	{
		return getI18nConstants().professoresTitulacoes();
	}

	@Override
	protected String getPathExcelTemplate()
	{
		if ( fileExtension.equals("xlsx") )
		{
			return "/templateExport.xlsx";
		}
		else
			return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return getI18nConstants().professoresTitulacoes();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook )
	{
		boolean result = false;
		
		List< CampusDTO > campusDTOList = null;
		if ( this.getFilter() == null
			|| this.getFilter().getCampusDTO() == null )
		{
			List< Campus > campi = Campus.findAll( this.instituicaoEnsino );
			campusDTOList = new ArrayList< CampusDTO >( campi.size() );
			for ( Campus campus : campi )
			{
				campusDTOList.add( ConvertBeans.toCampusDTO( campus ) );
			}
		}
		else
		{
			campusDTOList = new ArrayList< CampusDTO >();
			campusDTOList.add( this.getFilter().getCampusDTO() );
		}

		List< RelatorioQuantidadeDTO > relatorioDocenteDTO
			= new ArrayList< RelatorioQuantidadeDTO >();
		ProfessoresServiceImpl professoresServiceImpl = new ProfessoresServiceImpl();
		for ( CampusDTO campusDTO : campusDTOList )
		{
			try {
				relatorioDocenteDTO.addAll(
						professoresServiceImpl.getProfessoresTitulacoes(ConvertBeans.toCenarioDTO(getCenario()), campusDTO) );
			} catch (TriedaException e) {
				e.printStackTrace();
			}
		}

		if ( !relatorioDocenteDTO.isEmpty() )
		{
			Sheet sheet = workbook.getSheet( this.getSheetName() );
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			for ( RelatorioQuantidadeDTO relatorioDocenteDTO1 : relatorioDocenteDTO )
			{
				nextRow = writeData( relatorioDocenteDTO1, nextRow, sheet );
			}

			result = true;
		}

		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}

		return result;
	}

	private int writeData( RelatorioQuantidadeDTO relatorioDocenteDTO, int row, Sheet sheet )
	{
		int i = 2;
		
		// Faixa Credito
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				relatorioDocenteDTO.getFaixaCredito() );
		
		// Professores da Instituicao qtde total
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
				relatorioDocenteDTO.getQuantidade2() );

		// Professores da Instituicao qtde utilizados
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
				relatorioDocenteDTO.getQuantidade3() );
		
		// Professores Virtuais
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
				relatorioDocenteDTO.getQuantidade() );
		row++;
		return row;
	}

	private void fillInCellStyles( Sheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference
			: ExcelCellStyleReference.values() )
		{
			cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}

	public AtendimentosFaixaDemandaFiltroExcel getFilter()
	{
		return filter;
	}

	public void setFilter( ExportExcelFilter filter )
	{
		if ( filter != null && filter instanceof AtendimentosFaixaDemandaFiltroExcel )
		{
			this.filter = (AtendimentosFaixaDemandaFiltroExcel) filter;
		}
		else
		{
			this.filter = null;
		}
	}
}