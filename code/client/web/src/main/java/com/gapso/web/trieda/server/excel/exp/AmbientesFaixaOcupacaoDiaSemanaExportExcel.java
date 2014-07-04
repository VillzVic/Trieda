package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDoubleDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;

@ProgressDeclarationAnnotation
public class AmbientesFaixaOcupacaoDiaSemanaExportExcel
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
	
	private CellStyle[] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;
	private AtendimentosFaixaDemandaFiltroExcel filter;
	
	public AmbientesFaixaOcupacaoDiaSemanaExportExcel( Cenario cenario, TriedaI18nConstants i18nConstants,
			TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension );
	}
	
	public AmbientesFaixaOcupacaoDiaSemanaExportExcel( boolean removeUnusedSheets, Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, 
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino, fileExtension );
	}

	public AmbientesFaixaOcupacaoDiaSemanaExportExcel( boolean removeUnusedSheets,
			Cenario cenario, TriedaI18nConstants i18nConstants,
			TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.AMBIENTES_FAIXA_OCUPACAO_SEMANA.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
		this.setFilter( filter );
	}
	
	@Override
	public String getFileName()
	{
		return getI18nConstants().ambientesFaixaOcupacaoSemana();
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
		return getI18nConstants().ambientesFaixaOcupacaoSemana();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook )
	{
		boolean result = false;
		
		List< CampusDTO > campusDTOList = null;
		if ( this.getFilter() == null
			|| this.getFilter().getCampusDTO() == null )
		{
			List< Campus > campi = Campus.findByCenario( this.instituicaoEnsino, getCenario() );
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

		List< RelatorioQuantidadeDoubleDTO > relatorioDocenteDTO
			= new ArrayList< RelatorioQuantidadeDoubleDTO >();
		AtendimentosServiceImpl atendimentosServiceImpl = new AtendimentosServiceImpl();
		for ( CampusDTO campusDTO : campusDTOList )
		{
			relatorioDocenteDTO.addAll(
					atendimentosServiceImpl.getAmbientesFaixaUtilizacaoPorDiaSemana(ConvertBeans.toCenarioDTO(getCenario()), campusDTO) );
		}

		if ( !relatorioDocenteDTO.isEmpty() )
		{
			Sheet sheet = workbook.getSheet( this.getSheetName() );
			if (isXls()) {
				fillInCellStyles(sheet);
			}
			else {
				Sheet templateSheet = templateWorkbook.getSheet(this.getSheetName());
				fillInCellStyles(templateSheet);
			}
			int nextRow = this.initialRow;
			for ( RelatorioQuantidadeDoubleDTO relatorioDocenteDTO1 : relatorioDocenteDTO )
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

	private int writeData( RelatorioQuantidadeDoubleDTO relatorioDocenteDTO, int row, Sheet sheet )
	{
		int i = 2;
		
		// Campus
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				relatorioDocenteDTO.getCampusNome() );
		
		// Faixa Credito
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				relatorioDocenteDTO.getFaixaCredito() );
		
		// Segunda
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				relatorioDocenteDTO.getQuantidade() + "%" );
	
		// Terça
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				relatorioDocenteDTO.getQuantidade2() + "%" );
		
		// Quarta
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				relatorioDocenteDTO.getQuantidade3() + "%" );
		
		// Quinta
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				relatorioDocenteDTO.getQuantidade4() + "%" );
	
		// Sexta
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				relatorioDocenteDTO.getQuantidade5() + "%" );
		
		// Sabado
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				relatorioDocenteDTO.getQuantidade6() + "%" );

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