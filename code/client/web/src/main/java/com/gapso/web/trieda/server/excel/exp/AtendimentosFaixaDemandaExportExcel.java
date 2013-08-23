package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.AlunosDemandaServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaDemandaDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;

public class AtendimentosFaixaDemandaExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT( 6, 2 ),
		PERCENTE( 6, 11 ),
		INTEGER( 6, 5 ),
		DOUBLE( 6, 9 );

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
	
	public AtendimentosFaixaDemandaExportExcel( Cenario cenario, TriedaI18nConstants i18nConstants,
			TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension );
	}
	
	public AtendimentosFaixaDemandaExportExcel( boolean removeUnusedSheets, Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, 
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino, fileExtension );
	}

	public AtendimentosFaixaDemandaExportExcel( boolean removeUnusedSheets,
			Cenario cenario, TriedaI18nConstants i18nConstants,
			TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.ATENDIMENTOS_FAIXA_DEMANDA.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
		this.setFilter( filter );
	}
	
	@Override
	public String getFileName()
	{
		return getI18nConstants().atendimentosFaixaDemanda();
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
		return getI18nConstants().atendimentosFaixaDemanda();
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

		List< AtendimentoFaixaDemandaDTO > resumoFaixaDemandaDTO
			= new ArrayList< AtendimentoFaixaDemandaDTO >();
		AlunosDemandaServiceImpl alunosDemandaServiceImpl = new AlunosDemandaServiceImpl();
		for ( CampusDTO campusDTO : campusDTOList )
		{
			resumoFaixaDemandaDTO.addAll(
					alunosDemandaServiceImpl.getResumoFaixaDemandaList(ConvertBeans.toCenarioDTO(getCenario()), campusDTO, null) );
		}

		if ( !resumoFaixaDemandaDTO.isEmpty() )
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
			for ( AtendimentoFaixaDemandaDTO resumoFaixaDemandaDTO1 : resumoFaixaDemandaDTO )
			{
				nextRow = writeData( resumoFaixaDemandaDTO1, nextRow, sheet );
			}

			result = true;
		}

		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}

		return result;
	}

	private int writeData( AtendimentoFaixaDemandaDTO resumoFaixaDemandaDTO, int row, Sheet sheet )
	{
		int i = 2;
		
		// Campus
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				resumoFaixaDemandaDTO.getCampusNome() );
		
		// Faixa de Demanda
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			resumoFaixaDemandaDTO.getDemandaDisc() );

		// Demanda P1
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			resumoFaixaDemandaDTO.getDemandaP1() );

		// Atendimento P1
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			resumoFaixaDemandaDTO.getAtendimentoP1() );

		// % Atendimento P1 
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			resumoFaixaDemandaDTO.getAtendimentoPercentP1() );

		// Atendimento P1+P2
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			resumoFaixaDemandaDTO.getAtendimentoSoma() );

		// Atendimento P1+P2 %
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			resumoFaixaDemandaDTO.getAtendimentoSomaPercent() );
		
		// Demanda P1 Acum
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			resumoFaixaDemandaDTO.getDemandaAcumP1() );
		
		// Atendimento P1+P2 Acum
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			resumoFaixaDemandaDTO.getAtendimentoSomaAcum() );
		
		// Atendimento P1+P2 Acum %
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			resumoFaixaDemandaDTO.getAtendimentoSomaAcumPercent() );
		
		// Trumas Abertas
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			resumoFaixaDemandaDTO.getTurmasAbertas() );

		// Media alunos por turma
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.DOUBLE.ordinal() ],
			resumoFaixaDemandaDTO.getMediaTurma() );
		
		// Creditos Pagos
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			resumoFaixaDemandaDTO.getCreditosPagos() );
		
		// Receita Semanal
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.DOUBLE.ordinal() ],
			resumoFaixaDemandaDTO.getReceitaSemanal() );
		
		// Custo Docente Semanal
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.DOUBLE.ordinal() ],
			resumoFaixaDemandaDTO.getCustoDocenteSemanal() );
		
		// Custo Docente por Receita %
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			resumoFaixaDemandaDTO.getCustoDocentePorReceitaPercent() );
		
		// Receita Acumulada
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.DOUBLE.ordinal() ],
			resumoFaixaDemandaDTO.getReceitaAcumulada() );
		
		// Custo Docente Acumulado
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.DOUBLE.ordinal() ],
			resumoFaixaDemandaDTO.getCustoDocenteAcumulado() );
		
		// Custo Docente Por Receita Acumulado %
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			resumoFaixaDemandaDTO.getCustoDocentePorReceitaAcumuladoPercent() );

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
