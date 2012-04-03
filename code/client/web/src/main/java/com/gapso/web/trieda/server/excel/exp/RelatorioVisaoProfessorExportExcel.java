package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;


public class RelatorioVisaoProfessorExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		HEADER_LEFT_TEXT( 5,3 ),
		HEADER_CENTER_VALUE( 5,4 ),
		HEADER_CENTER_TEXT( 7,2 ),
		TEXT( 8,2 );

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

	private HSSFCellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;
	private RelatorioVisaoProfessorFiltroExcel relatorioFiltro;
	private Campus campus;
	private boolean isVisaoProfessor;

	public RelatorioVisaoProfessorExportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, boolean isVisaoProfessor,
		InstituicaoEnsino instituicaoEnsino )
	{
		this( true, cenario, i18nConstants, i18nMessages, null, isVisaoProfessor, instituicaoEnsino );
	}

	public RelatorioVisaoProfessorExportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
		boolean isVisaoProfessor, InstituicaoEnsino instituicaoEnsino )
	{
		this( true, cenario, i18nConstants, i18nMessages, filter, isVisaoProfessor, instituicaoEnsino );
	}

	public RelatorioVisaoProfessorExportExcel(
		boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		boolean isVisaoProfessor, InstituicaoEnsino instituicaoEnsino )
	{
		this( removeUnusedSheets, cenario, i18nConstants,
			  i18nMessages, null, isVisaoProfessor, instituicaoEnsino );
	}

	public RelatorioVisaoProfessorExportExcel(
		boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		ExportExcelFilter filter, boolean isVisaoProfessor,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( false, ExcelInformationType.RELATORIO_VISAO_PROFESSOR.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 5;
		this.isVisaoProfessor = isVisaoProfessor;
		this.setFilter( filter );

		if ( this.relatorioFiltro != null )
		{
			this.campus = Campus.find(
				this.relatorioFiltro.getCampusDTO().getId(),
				this.instituicaoEnsino );
		}
	}

	public boolean isVisaoProfessor()
	{
		return this.isVisaoProfessor;
	}

	public void setVisaoProfessor( boolean isVisaoProfessor )
	{
		this.isVisaoProfessor = isVisaoProfessor;
	}

	@Override
	public String getFileName()
	{
		return this.getI18nConstants().relatorioVisaoProfessor();
	}

	@Override
	protected String getPathExcelTemplate()
	{
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return this.getI18nConstants().relatorioVisaoProfessor();
	}

	public RelatorioVisaoProfessorFiltroExcel getFilter()
	{
		return this.relatorioFiltro;
	}

	public void setFilter( ExportExcelFilter filter )
	{
		if ( filter instanceof RelatorioVisaoProfessorFiltroExcel )
		{
			this.relatorioFiltro = (RelatorioVisaoProfessorFiltroExcel) filter;
		}
		else
		{
			this.relatorioFiltro = null;
		}
	}

	private List< AtendimentoOperacionalDTO > getAtendimentos()
	{
		boolean idAdmin = true;

		List< Professor > professores = null;
		List< ProfessorVirtual > professoresVirtuais = null;
		List< Turno > turnos = null;

		// Recupera os dados preenchidos nos filtros
		if ( this.relatorioFiltro != null )
		{
			Professor professor = ( this.relatorioFiltro.getProfessorDTO() == null ? null :
				Professor.find( this.relatorioFiltro.getProfessorDTO().getId(), this.instituicaoEnsino ) );

			ProfessorVirtual professorVirtual = ( this.relatorioFiltro.getProfessorVirtualDTO() == null ? null :
				ProfessorVirtual.find( this.relatorioFiltro.getProfessorVirtualDTO().getId(), this.instituicaoEnsino ) );

			if ( professor != null )
			{
				professores = new ArrayList< Professor >( 1 );
				professores.add( professor );
			}
			else if ( professorVirtual != null )
			{
				professoresVirtuais = new ArrayList< ProfessorVirtual >( 1 );
				professoresVirtuais.add( professorVirtual );
			}

			Turno turno = ( this.relatorioFiltro.getTurnoDTO() == null ? null :
				Turno.find( this.relatorioFiltro.getTurnoDTO().getId(), this.instituicaoEnsino ) );

			if ( turno == null )
			{
				turnos = Turno.findAll( this.instituicaoEnsino );
			}
			else
			{
				turnos = new ArrayList< Turno >( 1 );
				turnos.add( turno );
			}

		}
		else
		{
			// Relatorio solicitado a partir da opção de 'exportar tudo'
			professores = Professor.findAll( this.instituicaoEnsino );
			professoresVirtuais = ProfessorVirtual.findAll( this.instituicaoEnsino );
			turnos = Turno.findAll( this.instituicaoEnsino );
		}

		Set< AtendimentoOperacional > atendimentosOperacional
			= new HashSet< AtendimentoOperacional >();

		// Atendimentos operacionais dos professores
		if ( professores != null && professores.size() > 0)
		{
			for ( Professor professor : professores )
			{
				for ( Turno turno : turnos )
				{
						List< AtendimentoOperacional > atendimentos
							= AtendimentoOperacional.getAtendimentosOperacional( this.instituicaoEnsino,
								idAdmin, professor, null, turno, this.isVisaoProfessor());

						atendimentosOperacional.addAll( atendimentos );
				}
			}
		}

		// Atendimentos operacionais dos professores virtuais
		if ( professoresVirtuais != null && professoresVirtuais.size() > 0)
		{
			for ( ProfessorVirtual professorVirtual : professoresVirtuais )
			{
				for ( Turno turno : turnos )
				{
						List< AtendimentoOperacional > atendimentos
							= AtendimentoOperacional.getAtendimentosOperacional( this.instituicaoEnsino,
								idAdmin, null, professorVirtual, turno, this.isVisaoProfessor());

						atendimentosOperacional.addAll( atendimentos );
				}
			}
		}

		List< AtendimentoOperacionalDTO > atendimentosOperacionalDTO
			= new ArrayList< AtendimentoOperacionalDTO >();

		for ( AtendimentoOperacional domain : atendimentosOperacional )
		{
			atendimentosOperacionalDTO.add(
				ConvertBeans.toAtendimentoOperacionalDTO( domain ) );
		}

		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		atendimentosOperacionalDTO = service.ordenaPorHorarioAula( atendimentosOperacionalDTO );

		return atendimentosOperacionalDTO;
	}
	
	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		boolean result = false;

		List< AtendimentoOperacionalDTO > atendimentos = this.getAtendimentos();

		if ( !atendimentos.isEmpty() )
		{
			HSSFSheet sheet = workbook.getSheet( this.getSheetName() );
			fillInCellStyles( sheet );

//			List< HSSFComment > excelCommentsPool = buildExcelCommentsPool( workbook );
//			Iterator< HSSFComment > itExcelCommentsPool = excelCommentsPool.iterator();
			List< HSSFCellStyle > excelColorsPool = buildColorPaletteCellStyles( workbook );

			Map< String, HSSFCellStyle > codigoDisciplinaToColorMap
				= new HashMap< String, HSSFCellStyle >();

			// Dado o id de um professor ( ou professor virtual ),
			// temos os seus atendimentos, organizados por turno
			Map<Boolean, Map< Long, Map< Turno, Map< Long, List< AtendimentoOperacionalDTO > > > > >mapNivel0
				= new TreeMap<Boolean, Map< Long, Map< Turno, Map< Long, List< AtendimentoOperacionalDTO > > > > >();

			mapNivel0.put(true, new HashMap< Long, Map< Turno, Map< Long, List< AtendimentoOperacionalDTO > > > >());
			mapNivel0.put(false, new HashMap< Long, Map< Turno, Map< Long, List< AtendimentoOperacionalDTO > > > >());
			
			for ( AtendimentoOperacionalDTO atendimento : atendimentos )
			{
				Professor professor = null;
				ProfessorVirtual professorVirtual = null;
				Long professorId = null;
				Map< Long, Map< Turno, Map< Long, List< AtendimentoOperacionalDTO > > > > mapNivel1;

				if ( this.campus == null )
				{
					this.campus = Campus.find(
						atendimento.getCampusId(), this.instituicaoEnsino );
				}

				if ( atendimento.getProfessorId() != null )
				{
					professor = Professor.find(
						atendimento.getProfessorId(), this.instituicaoEnsino );
					mapNivel1 = mapNivel0.get(true);
					professorId = professor.getId();
				}
				else
				{
					professorVirtual = ProfessorVirtual.find(
						atendimento.getProfessorVirtualId(), this.instituicaoEnsino );
					mapNivel1 = mapNivel0.get(false);
					professorId = professorVirtual.getId();
				}

				Turno turno = Turno.find( atendimento.getTurnoId(), this.instituicaoEnsino );
				SemanaLetiva semanaLetiva = SemanaLetiva.find( atendimento.getSemanaLetivaId(), this.instituicaoEnsino );

				Map< Turno, Map< Long, List< AtendimentoOperacionalDTO > > > mapNivel2 = mapNivel1.get( professorId );

				if ( mapNivel2 == null )
				{
					mapNivel2 = new HashMap< Turno, Map< Long, List< AtendimentoOperacionalDTO > > >();
					mapNivel1.put( professorId, mapNivel2 );
				}

				Map< Long, List< AtendimentoOperacionalDTO > > mapNivel3 = mapNivel2.get( turno );

				if ( mapNivel3 == null )
				{
					mapNivel3 = new HashMap< Long, List< AtendimentoOperacionalDTO > >();
					mapNivel2.put( turno, mapNivel3 );
				}

				List< AtendimentoOperacionalDTO > list = mapNivel3.get( semanaLetiva.getId() );
				
				if ( list == null )
				{
					list = new ArrayList< AtendimentoOperacionalDTO >();
					mapNivel3.put( semanaLetiva.getId(), list );
				}

				list.add( atendimento );

				HSSFCellStyle style = codigoDisciplinaToColorMap.get(
					atendimento.getDisciplinaString() );

				if ( style == null )
				{
					int index = ( codigoDisciplinaToColorMap.size() % excelColorsPool.size() );
					codigoDisciplinaToColorMap.put(
						atendimento.getDisciplinaString(), excelColorsPool.get( index ) );
				}
			}

			int nextRow = this.initialRow;

			for(Boolean b : mapNivel0.keySet()){
				Map< Long, Map< Turno, Map< Long, List< AtendimentoOperacionalDTO > > > > mapNivel1 = mapNivel0.get(b);
				for ( Long profId : mapNivel1.keySet() )
				{
					Map< Turno, Map< Long, List< AtendimentoOperacionalDTO > > > mapNivel2 = mapNivel1.get( profId );
	
					for ( Turno turno : mapNivel2.keySet() )
					{
						Map< Long, List< AtendimentoOperacionalDTO > > mapNivel3 = mapNivel2.get( turno );
	
						Set<Long> semanasLetivasIDs = new HashSet<Long>();
						List<AtendimentoOperacionalDTO> listAtendimentos = new ArrayList<AtendimentoOperacionalDTO>();
						for (Long semLetId : mapNivel3.keySet()) {
							semanasLetivasIDs.add(semLetId);
							// acumula os atendimentos das semanas letivas
							listAtendimentos.addAll(mapNivel3.get(semLetId));
						}
						
						AtendimentosServiceImpl service = new AtendimentosServiceImpl();
						TrioDTO<Integer,SemanaLetiva,List<String>> trio = service.calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(semanasLetivasIDs, false, turno.getId());
						int mdcTemposAula = trio.getPrimeiro();
						List<String> labelsDasLinhasDaGradeHoraria = trio.getTerceiro();
	
						Professor professor = Professor.find( profId, this.instituicaoEnsino );
	
						if(!b){
							ProfessorVirtual professorVirtual = ProfessorVirtual.find(
								profId, this.instituicaoEnsino );
							if ( professorVirtual != null )
							{
								nextRow = writeProfessorVirtual(
									this.campus, professorVirtual, turno, listAtendimentos,
									nextRow, sheet, /*itExcelCommentsPool,*/ codigoDisciplinaToColorMap, mdcTemposAula, labelsDasLinhasDaGradeHoraria);
							}
						}
						else{
							nextRow = writeProfessor( this.campus, professor, turno, listAtendimentos,
								nextRow, sheet, /*itExcelCommentsPool,*/ codigoDisciplinaToColorMap, mdcTemposAula, labelsDasLinhasDaGradeHoraria);
						}
					}
				}
			}

			result = true;
		}

		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}

		return result;
	}

	private int writeProfessor(
		Campus campus, Professor professor,
		Turno turno, List< AtendimentoOperacionalDTO > atendimentos,
		int row, HSSFSheet sheet, /*Iterator< HSSFComment > itExcelCommentsPool,*/
		Map< String, HSSFCellStyle > codigoDisciplinaToColorMap, int mdcTemposAula, List<String> labelsDasLinhasDaGradeHoraria )
	{
		row = writeHeaderProfessor( campus, professor, turno, row, sheet );
		
		int initialRow = row;
		int col = 2;

		// Preenche grade com horários e células vazias
		for (int i = 0; i < labelsDasLinhasDaGradeHoraria.size()-1; i++) {
			// coluna de carga horária
			String label = labelsDasLinhasDaGradeHoraria.get(i) + " / " + labelsDasLinhasDaGradeHoraria.get(i+1);
			setCell((row+i),col++,sheet,this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()],label);
			// colunas dos dias da semana
			for (int j = 0; j < Semanas.values().length; j++) {
				setCell((row+i),col++,sheet,this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()],"");
			}
			
			col = 2;
		}

		// Processa os atendimentos lidos do BD para que
		// os mesmos sejam visualizados na visão professor
		AtendimentosServiceImpl atendimentosService = new AtendimentosServiceImpl();
		List< AtendimentoRelatorioDTO > aulas = new ArrayList< AtendimentoRelatorioDTO >(atendimentosService.extraiAulas(atendimentos));
		List< AtendimentoRelatorioDTO > arDTOListProcessada = atendimentosService.uneAulasQuePodemSerCompartilhadas( aulas );
		List< AtendimentoOperacionalDTO > atendimentosParaVisaoProfessor = new ArrayList<AtendimentoOperacionalDTO>(arDTOListProcessada.size());
		for (AtendimentoRelatorioDTO dto : arDTOListProcessada) {
			atendimentosParaVisaoProfessor.add((AtendimentoOperacionalDTO)dto);
		}
		
		atendimentosParaVisaoProfessor = this.ordenaHorarioAula( atendimentosParaVisaoProfessor );

		// Agrupa os atendimentos por dia da semana
		Map< Integer, List< AtendimentoOperacionalDTO > > diaSemanaToAtendimentosMap = new HashMap< Integer, List< AtendimentoOperacionalDTO > >();
		for ( AtendimentoOperacionalDTO atendimento : atendimentosParaVisaoProfessor ) {
			List< AtendimentoOperacionalDTO > list = diaSemanaToAtendimentosMap.get( atendimento.getSemana() );

			if ( list == null ) {
				list = new ArrayList< AtendimentoOperacionalDTO >();
				diaSemanaToAtendimentosMap.put( atendimento.getSemana(), list );
			}

			list.add( atendimento );
		}

		// Para cada dia da semana, escreve os atendimentos no excel
		for ( Integer diaSemanaInt : diaSemanaToAtendimentosMap.keySet() ) {
			row = initialRow;

			Semanas diaSemana = Semanas.get( diaSemanaInt );

			switch ( diaSemana )
			{
				case SEG: { col = 3; break; }
				case TER: { col = 4; break; }
				case QUA: { col = 5; break; }
				case QUI: { col = 6; break; }
				case SEX: { col = 7; break; }
				case SAB: { col = 8; break; }
				case DOM: { col = 9; break; }
			}

			List< AtendimentoOperacionalDTO > atedimentosDiaSemana = diaSemanaToAtendimentosMap.get( diaSemanaInt );

			for (AtendimentoOperacionalDTO atendimento : atedimentosDiaSemana) {
				HSSFCellStyle style = codigoDisciplinaToColorMap.get(atendimento.getDisciplinaString());
				// obtém a qtd de linhas que devem ser desenhadas para cada crédito da aula em questão
				int linhasDeExcelPorCreditoDaAula = atendimento.getDuracaoDeUmaAulaEmMinutos()/mdcTemposAula;
				
				int index = labelsDasLinhasDaGradeHoraria.indexOf(atendimento.getHorarioString());
				if(index != -1){
					row = initialRow + index;
				}

				// escreve célula principal
				setCell(row,col,sheet,style,HtmlUtils.htmlUnescape(atendimento.getContentVisaoSala(ReportType.EXCEL)),HtmlUtils.htmlUnescape(atendimento.getContentToolTipVisaoSala(ReportType.EXCEL)));
				// une células de acordo com a quantidade de créditos da aula
				int rowF = row + atendimento.getTotalCreditos()*linhasDeExcelPorCreditoDaAula - 1;
				mergeCells(row,rowF,col,col,sheet,style);
			}
		}

		return ( initialRow + labelsDasLinhasDaGradeHoraria.size());
	}

	private int writeProfessorVirtual(
		Campus campus, ProfessorVirtual professorVirtual,
		Turno turno, List< AtendimentoOperacionalDTO > atendimentos, int row,
		HSSFSheet sheet, /*Iterator< HSSFComment > itExcelCommentsPool,*/
		Map< String,HSSFCellStyle > codigoDisciplinaToColorMap , int mdcTemposAula, List<String> labelsDasLinhasDaGradeHoraria)
	{
		row = writeHeaderProfessorVirtual( campus, professorVirtual, turno, row, sheet );
		
		int initialRow = row;
		int col = 2;

		// Preenche grade com horários e células vazias
		for (int i = 0; i < labelsDasLinhasDaGradeHoraria.size()-1; i++) {
			// coluna de carga horária
			String label = labelsDasLinhasDaGradeHoraria.get(i) + " / " + labelsDasLinhasDaGradeHoraria.get(i+1);
			setCell((row+i),col++,sheet,this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()],label);
			// colunas dos dias da semana
			for (int j = 0; j < Semanas.values().length; j++) {
				setCell((row+i),col++,sheet,this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()],"");
			}
			
			col = 2;
		}

		// Processa os atendimentos lidos do BD para
		// que os mesmos sejam visualizados na visão professor
		AtendimentosServiceImpl atendimentosService = new AtendimentosServiceImpl();
		List< AtendimentoRelatorioDTO > aulas = new ArrayList< AtendimentoRelatorioDTO >(atendimentosService.extraiAulas(atendimentos));
		List< AtendimentoRelatorioDTO > arDTOListProcessada = atendimentosService.uneAulasQuePodemSerCompartilhadas( aulas );
		List< AtendimentoOperacionalDTO > atendimentosParaVisaoProfessor = new ArrayList<AtendimentoOperacionalDTO>(arDTOListProcessada.size());
		for (AtendimentoRelatorioDTO dto : arDTOListProcessada) {
			atendimentosParaVisaoProfessor.add((AtendimentoOperacionalDTO)dto);
		}

		atendimentosParaVisaoProfessor
			= this.ordenaHorarioAula( atendimentosParaVisaoProfessor );

		// Agrupa os atendimentos por dia da semana
		Map< Integer, List< AtendimentoOperacionalDTO > > diaSemanaToAtendimentosMap
			= new HashMap< Integer, List< AtendimentoOperacionalDTO > >();

		for ( AtendimentoOperacionalDTO atendimento
			: atendimentosParaVisaoProfessor )
		{
			List< AtendimentoOperacionalDTO > list
				= diaSemanaToAtendimentosMap.get( atendimento.getSemana() );

			if ( list == null )
			{
				list = new ArrayList< AtendimentoOperacionalDTO >();
				diaSemanaToAtendimentosMap.put( atendimento.getSemana(), list );
			}

			list.add( atendimento );
		}

		// Para cada dia da semana, escreve os atendimentos no excel
		for ( Integer diaSemanaInt : diaSemanaToAtendimentosMap.keySet() )
		{
			row = initialRow;
			Semanas diaSemana = Semanas.get( diaSemanaInt );

			switch ( diaSemana )
			{
				case SEG: { col = 3; break; }
				case TER: { col = 4; break; }
				case QUA: { col = 5; break; }
				case QUI: { col = 6; break; }
				case SEX: { col = 7; break; }
				case SAB: { col = 8; break; }
				case DOM: { col = 9; break; }
			}

			List< AtendimentoOperacionalDTO > atedimentosDiaSemana
					= diaSemanaToAtendimentosMap.get( diaSemanaInt );

			for (AtendimentoOperacionalDTO atendimento : atedimentosDiaSemana) {
				HSSFCellStyle style = codigoDisciplinaToColorMap.get(atendimento.getDisciplinaString());
				// obtém a qtd de linhas que devem ser desenhadas para cada crédito da aula em questão
				int linhasDeExcelPorCreditoDaAula = atendimento.getDuracaoDeUmaAulaEmMinutos()/mdcTemposAula;
				
				int index = labelsDasLinhasDaGradeHoraria.indexOf(atendimento.getHorarioString());
				if (index != -1) {
					row = initialRow + index;
				}

				// escreve célula principal
				setCell(row,col,sheet,style,HtmlUtils.htmlUnescape(atendimento.getContentVisaoSala(ReportType.EXCEL)),HtmlUtils.htmlUnescape(atendimento.getContentToolTipVisaoSala(ReportType.EXCEL)));
				// une células de acordo com a quantidade de créditos da aula
				int rowF = row + atendimento.getTotalCreditos()*linhasDeExcelPorCreditoDaAula - 1;
				mergeCells(row,rowF,col,col,sheet,style);
			}
		}

		return ( initialRow + labelsDasLinhasDaGradeHoraria.size());
	}

	private int writeHeaderProfessor(
		Campus campus, Professor professor,
		Turno turno, int row, HSSFSheet sheet )
	{
		int col = 3;

		// Campus
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], this.getI18nConstants().campus() );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], campus.getCodigo() );

		// Professor
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], this.getI18nConstants().professor() );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], professor.getNome() );

		row++;
		col = 3;

		// Turno
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], this.getI18nConstants().turno() );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], turno.getNome() );

		// Professor Virtual
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			this.getI18nConstants().professorVirtual() );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], "" );

		row++;
		col = 2;

		// Horários
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ], HtmlUtils.htmlUnescape(this.getI18nConstants().horarios()) );

		// Dias Semana
		for ( Semanas semanas : Semanas.values() )
		{
			setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ], semanas.name() );
		}

		row++;
		return row;
	}

	private int writeHeaderProfessorVirtual( Campus campus, ProfessorVirtual professorVirtual,
		Turno turno, int row, HSSFSheet sheet )
	{
		int col = 3;

		// Campus
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], this.getI18nConstants().campus() );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], campus.getCodigo() );

		// Professor
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], this.getI18nConstants().professor() );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], "" );

		row++;
		col = 3;

		// Turno
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], this.getI18nConstants().turno() );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], turno.getNome() );

		// Professor Virtual
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], this.getI18nConstants().professorVirtual() );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], professorVirtual.getNome() );

		row++;
		col = 2;

		// Horários
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ], this.getI18nConstants().horarios() );

		// Dias Semana
		for ( Semanas semanas : Semanas.values() )
		{
			setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ], semanas.name() );
		}

		row++;
		return row;
	}

	private List< HSSFCellStyle > buildColorPaletteCellStyles( HSSFWorkbook workbook )
	{
		List< HSSFCellStyle > colorPalleteCellStylesList = new ArrayList< HSSFCellStyle >();

		HSSFSheet sheet = workbook.getSheet(
			ExcelInformationType.PALETA_CORES.getSheetName() );

		if ( sheet != null )
		{
            for ( int rowIndex = sheet.getFirstRowNum(); rowIndex <= sheet.getLastRowNum(); rowIndex++ )
            {
            	HSSFRow row = sheet.getRow( rowIndex );
            	if ( row != null )
            	{
            		HSSFCell cell = row.getCell( (int)row.getFirstCellNum() );
            		if ( cell != null )
            		{
            			colorPalleteCellStylesList.add( cell.getCellStyle() );
            		}
            	}
            }
		}

		return colorPalleteCellStylesList;
	}

	private void fillInCellStyles( HSSFSheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values() )
		{
			this.cellStyles[ cellStyleReference.ordinal() ]
				= getCell( cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}

//	private List< HSSFComment > buildExcelCommentsPool( HSSFWorkbook workbook )
//	{
//		List< HSSFComment > excelCommentsPool = new ArrayList< HSSFComment >();
//		HSSFSheet sheet = workbook.getSheet(
//			ExcelInformationType.RELATORIO_VISAO_PROFESSOR.getSheetName() );
//
//		if ( sheet != null )
//		{
//            for ( int rowIndex = sheet.getFirstRowNum();
//            	rowIndex <= sheet.getLastRowNum(); rowIndex++ )
//            {
//            	HSSFRow row = sheet.getRow( rowIndex );
//
//            	if ( row != null )
//            	{
//            		HSSFCell cell = row.getCell( 25 );
//
//            		if ( cell != null && cell.getCellComment() != null )
//            		{
//            			excelCommentsPool.add( cell.getCellComment() );
//            		}
//            	}
//            }
//		}
//
//		return excelCommentsPool;
//	}

	private List< AtendimentoOperacionalDTO > ordenaHorarioAula( List< AtendimentoOperacionalDTO > list ) {
		if ( list == null || list.size() == 0 ) {
			return Collections.< AtendimentoOperacionalDTO > emptyList();
		}
		
		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		return service.ordenaPorHorarioAula( list );
	}
}
