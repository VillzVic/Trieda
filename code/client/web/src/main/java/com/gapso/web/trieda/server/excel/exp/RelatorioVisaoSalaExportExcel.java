package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class RelatorioVisaoSalaExportExcel
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
	private RelatorioVisaoSalaFiltroExcel relatorioFiltro;

	public RelatorioVisaoSalaExportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino )
	{
		this(true, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino );
	}

	public RelatorioVisaoSalaExportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
		InstituicaoEnsino instituicaoEnsino )
	{
		this(true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino );
	}

	public RelatorioVisaoSalaExportExcel(
		boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		this(removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino );
	}

	public RelatorioVisaoSalaExportExcel(
		boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		ExportExcelFilter filter, InstituicaoEnsino instituicaoEnsino )
	{
		super(false, ExcelInformationType.RELATORIO_VISAO_SALA.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 5;

		this.setFilter( filter );
	}

	@Override
	public String getFileName()
	{
		return this.getI18nConstants().relatorioVisaoSala();
	}

	@Override
	protected String getPathExcelTemplate()
	{
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return this.getI18nConstants().relatorioVisaoSala();
	}

	public RelatorioVisaoSalaFiltroExcel getFilter()
	{
		return this.relatorioFiltro;
	}

	public void setFilter( ExportExcelFilter filter )
	{
		if ( filter instanceof RelatorioVisaoSalaFiltroExcel )
		{
			this.relatorioFiltro = (RelatorioVisaoSalaFiltroExcel)filter;
		}
		else
		{
			this.relatorioFiltro = null;
		}
	}

	private List< AtendimentoRelatorioDTO > getAtendimentoRelatorioDTOList( Cenario cenario )
	{
		List< AtendimentoTatico > atdTaticoList = null;
		List< AtendimentoOperacional > atdOperacionalList =  null;

		if ( this.getFilter() == null )
		{
			atdTaticoList = AtendimentoTatico.findByCenario(
				this.instituicaoEnsino, cenario );

			atdOperacionalList = AtendimentoOperacional.findByCenario(
				this.instituicaoEnsino, cenario );
		}
		else
		{
			Campus campus = Campus.find(
				this.getFilter().getCampusDTO().getId(), this.instituicaoEnsino );

			Unidade unidade = Unidade.find(
				this.getFilter().getUnidadeDTO().getId(), this.instituicaoEnsino );

			Sala sala = Sala.find(
				this.getFilter().getSalaDTO().getId(), this.instituicaoEnsino );

			Turno turno = Turno.find(
				this.getFilter().getTurnoDTO().getId(), this.instituicaoEnsino );

			SemanaLetiva semanaLetiva = SemanaLetiva.find(
					this.getFilter().getSemanaLetivaDTO().getId(), this.instituicaoEnsino );

			atdTaticoList = AtendimentoTatico.findByCenario(
				this.instituicaoEnsino, cenario, campus, unidade, sala, turno, semanaLetiva );

			atdOperacionalList = AtendimentoOperacional.findByCenario(
				this.instituicaoEnsino, cenario, campus, unidade, sala, turno, semanaLetiva );
		}

		List< AtendimentoRelatorioDTO > atdRelatorioList
			= new ArrayList< AtendimentoRelatorioDTO >(
				atdTaticoList.size() + atdOperacionalList.size() );

		List< AtendimentoTaticoDTO > listAtendTaticoDTO
			= ConvertBeans.toListAtendimentoTaticoDTO( atdTaticoList );

		List< AtendimentoOperacionalDTO > listAtendOpDTO
			= ConvertBeans.toListAtendimentoOperacionalDTO( atdOperacionalList );

		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		listAtendOpDTO = service.ordenaPorHorarioAula( listAtendOpDTO );

		atdRelatorioList.addAll( listAtendTaticoDTO );
		atdRelatorioList.addAll( listAtendOpDTO );

		return atdRelatorioList;
	}
	
	@Override
	protected boolean fillInExcel(HSSFWorkbook workbook) {
		boolean result = false;
		
		// busca os atendimentos que deverão ser escritos no excel 
		List<AtendimentoRelatorioDTO> atendimentosDTO = getAtendimentoRelatorioDTOList(getCenario());
		
		if (!atendimentosDTO.isEmpty()) {
			// identifica se os atendimentos se referem ao modo tático ou ao modo operacional
			boolean ehTatico = atendimentosDTO.get(0) instanceof AtendimentoTaticoDTO;
			
			HSSFSheet sheet = workbook.getSheet(this.getSheetName());
			
			// monta estruturas de estilos
			fillInCellStyles(sheet);
			List<HSSFCellStyle> excelColorsPool = buildColorPaletteCellStyles(workbook);

			// Estruturas auxiliares
			// [DisciplinaCodigo -> Estilo para Cor]
			Map<String,HSSFCellStyle> codigoDisciplinaToColorMap = new HashMap<String,HSSFCellStyle>();
			// [SalaId -> Sala]
			Map<Long,Sala> salaIdToSalaMap = new HashMap<Long,Sala>();
			// [TurnoId -> Turno]
			Map<Long,Turno> turnoIdToTurnoMap = new HashMap<Long,Turno>();
			// [SemanaLetivaId -> SemanaLetiva]
			Map<Long,SemanaLetiva> semanaLetivaIdTosemanaLetivaMap = new HashMap<Long,SemanaLetiva>();
			// [SalaId -> [TurnoId -> [SemanaLetivaId -> List<AtendimentoRelatorioDTO>]]]
			Map<Long,Map<Long,Map<Long,List<AtendimentoRelatorioDTO>>>> atendimentosPorSalaTurnoSemanaLetivaMap = new HashMap<Long,Map<Long,Map<Long,List<AtendimentoRelatorioDTO>>>>();
			
			// preenche estruturas auxiliares
			for (AtendimentoRelatorioDTO atendimento : atendimentosDTO) {
				// preenche os maps salaIdToSalaMap, turnoIdToTurnoMap e semanaLetivaIdTosemanaLetivaMap
				if (!salaIdToSalaMap.containsKey(atendimento.getSalaId())) {
					salaIdToSalaMap.put(atendimento.getSalaId(),Sala.find(atendimento.getSalaId(),this.instituicaoEnsino));
				}
				if (!turnoIdToTurnoMap.containsKey(atendimento.getTurnoId())) {
					turnoIdToTurnoMap.put(atendimento.getTurnoId(),Turno.find(atendimento.getTurnoId(),this.instituicaoEnsino));
				}
				Long semanaLetivaASerConsideradaId = (atendimento.getDisciplinaSubstitutaSemanaLetivaId() != null) ? atendimento.getDisciplinaSubstitutaSemanaLetivaId() : atendimento.getSemanaLetivaId(); 
				if (!semanaLetivaIdTosemanaLetivaMap.containsKey(semanaLetivaASerConsideradaId)) {
					semanaLetivaIdTosemanaLetivaMap.put(semanaLetivaASerConsideradaId,SemanaLetiva.find(semanaLetivaASerConsideradaId,instituicaoEnsino));
				}

				// [TurnoId -> [SemanaLetivaId -> List<AtendimentoRelatorioDTO>]]
				Map<Long,Map<Long,List<AtendimentoRelatorioDTO>>> salaAtendimentosPorTurnoSemanaLetivaMap = atendimentosPorSalaTurnoSemanaLetivaMap.get(atendimento.getSalaId());
				if (salaAtendimentosPorTurnoSemanaLetivaMap == null) {
					salaAtendimentosPorTurnoSemanaLetivaMap = new HashMap<Long,Map<Long,List<AtendimentoRelatorioDTO>>>();
					atendimentosPorSalaTurnoSemanaLetivaMap.put(atendimento.getSalaId(),salaAtendimentosPorTurnoSemanaLetivaMap);
				}

				// [SemanaLetivaId -> List<AtendimentoRelatorioDTO>]
				Map<Long,List<AtendimentoRelatorioDTO>> turnoAtendimentosPorSemanaLetivaMap = salaAtendimentosPorTurnoSemanaLetivaMap.get(atendimento.getTurnoId());
				if (turnoAtendimentosPorSemanaLetivaMap == null) {
					turnoAtendimentosPorSemanaLetivaMap = new HashMap<Long,List<AtendimentoRelatorioDTO>>();
					salaAtendimentosPorTurnoSemanaLetivaMap.put(atendimento.getTurnoId(),turnoAtendimentosPorSemanaLetivaMap);
				}

				// armazena os atendimentos associados a uma dada sala, um dado turno e uma dada semana letiva
				List<AtendimentoRelatorioDTO> atendimentos = turnoAtendimentosPorSemanaLetivaMap.get(semanaLetivaASerConsideradaId);
				if (atendimentos == null) {
					atendimentos = new ArrayList<AtendimentoRelatorioDTO>();
					turnoAtendimentosPorSemanaLetivaMap.put(semanaLetivaASerConsideradaId,atendimentos);
				}
				atendimentos.add(atendimento);

				// preenche o mapa de estilos de cor por disciplina
				HSSFCellStyle style = codigoDisciplinaToColorMap.get(atendimento.getDisciplinaString());
				if (style == null) {
					int index = (codigoDisciplinaToColorMap.size()%excelColorsPool.size());
					codigoDisciplinaToColorMap.put(atendimento.getDisciplinaString(),excelColorsPool.get(index));
				}
			}

			// linha a partir da qual a escrita será iniciada no excel
			int nextRow = this.initialRow;

			// ordena as salas por campus e depois pela capacidade
			List<Sala> salasOrdenadasPorCampusEPelaCapacidade = new ArrayList<Sala>(salaIdToSalaMap.values());
			Collections.<Sala>sort(salasOrdenadasPorCampusEPelaCapacidade,new Comparator<Sala>() {
				@Override
				public int compare(Sala o1, Sala o2) {
					Unidade u1 = o1.getUnidade();
					Unidade u2 = o2.getUnidade();
					Campus c1 = u1.getCampus();
					Campus c2 = u2.getCampus();
					int result = c1.getCodigo().compareTo(c2.getCodigo());
					if (result == 0) {
						result = -(o1.getCapacidade().compareTo(o2.getCapacidade())); 
					}
					return result;
				}
			});
			
			// imprime uma grade de horários para cada sala
			for (Sala sala : salasOrdenadasPorCampusEPelaCapacidade) {
				// [TurnoId -> [SemanaLetivaId -> List<AtendimentoRelatorioDTO>]]
				Map<Long,Map<Long,List<AtendimentoRelatorioDTO>>> salaAtendimentosPorTurnoSemanaLetivaMap = atendimentosPorSalaTurnoSemanaLetivaMap.get(sala.getId());
				for (Long turnoId : salaAtendimentosPorTurnoSemanaLetivaMap.keySet()) {
					// [SemanaLetivaId -> List<AtendimentoRelatorioDTO>]
					Map<Long,List<AtendimentoRelatorioDTO>> turnoAtendimentosPorSemanaLetivaMap = salaAtendimentosPorTurnoSemanaLetivaMap.get(turnoId);
					Turno turno = turnoIdToTurnoMap.get(turnoId);
					// verifica qual é o modo de otimização (tático ou operacional)
					if (ehTatico) {
						int mdcTemposAula = 1;
						SemanaLetiva semanaLetivaComMaiorCargaHoraria = null;
						List<AtendimentoRelatorioDTO> atendimentosDeTodasSemanasLetivas = new ArrayList<AtendimentoRelatorioDTO>();
						if (turnoAtendimentosPorSemanaLetivaMap.keySet().size() > 1) {
							List<SemanaLetiva> semanasLetivas = new ArrayList<SemanaLetiva>();
							for (Long semLetId : turnoAtendimentosPorSemanaLetivaMap.keySet()) {
								semanasLetivas.add(semanaLetivaIdTosemanaLetivaMap.get(semLetId));
								// acumula os atendimentos das semanas letivas
								atendimentosDeTodasSemanasLetivas.addAll(turnoAtendimentosPorSemanaLetivaMap.get(semLetId));
							}
							semanaLetivaComMaiorCargaHoraria = SemanaLetiva.getSemanaLetivaComMaiorCargaHoraria(semanasLetivas); 
							mdcTemposAula = SemanaLetiva.caculaMaximoDivisorComumParaTemposDeAulaDasSemanasLetivas(semanasLetivas);
						} else {
							semanaLetivaComMaiorCargaHoraria = semanaLetivaIdTosemanaLetivaMap.get(turnoAtendimentosPorSemanaLetivaMap.keySet().iterator().next());
							atendimentosDeTodasSemanasLetivas.addAll(turnoAtendimentosPorSemanaLetivaMap.get(semanaLetivaComMaiorCargaHoraria.getId()));
							mdcTemposAula = semanaLetivaComMaiorCargaHoraria.getTempo();
						}
						// escreve grade horária no excel
						nextRow = writeSala(sala,turno,semanaLetivaComMaiorCargaHoraria,atendimentosDeTodasSemanasLetivas,nextRow,sheet,codigoDisciplinaToColorMap,mdcTemposAula,ehTatico);
					} else {
						for ( Long semanaLetivaId : turnoAtendimentosPorSemanaLetivaMap.keySet() )
						{
							SemanaLetiva semanaLetiva = semanaLetivaIdTosemanaLetivaMap.get(semanaLetivaId);
							List< AtendimentoRelatorioDTO > listAtendimentos = turnoAtendimentosPorSemanaLetivaMap.get( semanaLetivaId );
	
							nextRow = writeSala( sala, turno, semanaLetiva, listAtendimentos,
								nextRow, sheet, /*itExcelCommentsPool,*/ codigoDisciplinaToColorMap, 1, ehTatico );
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

	private int writeSala(Sala sala, Turno turno, SemanaLetiva semanaLetiva, List<AtendimentoRelatorioDTO> atendimentos, int row, HSSFSheet sheet, Map<String,HSSFCellStyle> codigoDisciplinaToColorMap, int mdcTemposAula, boolean ehTatico) {
		// escreve cabeçalho da grade horária da sala
		row = writeHeader(sala,turno,row,sheet,ehTatico);
		
		List<Long> horariosAulaIdsList = new ArrayList<Long>();
		int initialRow = row;
		int col = 2;

		int maxCreditos = semanaLetiva.calculaMaxCreditos();
		int linhasDeExcelPorCredito = semanaLetiva.getTempo()/mdcTemposAula;

		// preenche grade vazia
		if (ehTatico) {
			int cargaHoraria = mdcTemposAula;
			for (int indexCredito = 1; indexCredito <= maxCreditos; indexCredito++) {
				// coluna de carga horária
				for (int i = 0; i < linhasDeExcelPorCredito; i++) {
					setCell((row+i),col,sheet,this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()],cargaHoraria);
					cargaHoraria += mdcTemposAula;
				}
				col++;
	
				// colunas dos dias da semana
				for (int j = 0; j < Semanas.values().length; j++) {
					for (int i = 0; i < linhasDeExcelPorCredito; i++) {
						setCell((row+i),col,sheet,this.cellStyles[ExcelCellStyleReference.TEXT.ordinal()],"");
					}
					col++;
				}
	
				row += linhasDeExcelPorCredito;
				col = 2;
			}
		} else {
			List<HorarioAula> horariosAulaList = new ArrayList<HorarioAula>(semanaLetiva.getHorariosAula());
			Collections.sort(horariosAulaList);
			for (HorarioAula ha : horariosAulaList) {
				horariosAulaIdsList.add(ha.getId());
				// Horários
				String value = ConvertBeans.dateToString(ha.getHorario(), ha.getSemanaLetiva().getTempo() );
				setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], value );
				
				// Dias Semana
				for ( int i = 0; i < Semanas.values().length; i++ ) {
					setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], "" );
				}
	
				row++;
				col = 2;
			}
		}

		// processa os atendimentos lidos do BD para que os mesmos sejam visualizados na visão sala
		AtendimentosServiceImpl atendimentosService = new AtendimentosServiceImpl();
		List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>();
		if (!ehTatico) {
			List<AtendimentoOperacionalDTO> atendimentosOperacionaisDTO = new ArrayList<AtendimentoOperacionalDTO>();
			for (AtendimentoRelatorioDTO atendimentoDTO : atendimentos) {
				atendimentosOperacionaisDTO.add((AtendimentoOperacionalDTO)atendimentoDTO);
			}
			aulas.addAll(atendimentosService.extraiAulas(atendimentosOperacionaisDTO));
		} else {
			aulas.addAll(atendimentos);
		}
		List<AtendimentoRelatorioDTO> aulasParaVisaoSala = atendimentosService.uneAulasQuePodemSerCompartilhadas(aulas);
		aulasParaVisaoSala = ordenaHorarioAula(aulasParaVisaoSala);

		// agrupa as aulas por dia da semana
		Map<Integer,List<AtendimentoRelatorioDTO>> diaSemanaToAulasMap = new HashMap<Integer,List<AtendimentoRelatorioDTO>>();
		for (AtendimentoRelatorioDTO aula : aulasParaVisaoSala) {
			// TODO -- Considerar apenas os horários da semana letiva
			if (!ehTatico && (aula.getSemanaLetivaId() != semanaLetiva.getId())) {
				continue;
			}

			Integer diaDaSemana = aula.getSemana();
			List<AtendimentoRelatorioDTO> aulasDoDia = diaSemanaToAulasMap.get(diaDaSemana);
			if (aulasDoDia == null) {
				aulasDoDia = new ArrayList<AtendimentoRelatorioDTO>();
				diaSemanaToAulasMap.put(diaDaSemana,aulasDoDia);
			}
			aulasDoDia.add(aula);
		}

		// para cada dia da semana, escreve os atendimentos no excel
		for (Integer diaSemanaInt : diaSemanaToAulasMap.keySet()) {
			Semanas diaSemana = Semanas.get(diaSemanaInt);

			// linha em que a escrita será iniciada
			row = initialRow;
			// coluna em que a escrita será iniciada
			switch (diaSemana) {
				case SEG: { col = 3; break; }
				case TER: { col = 4; break; }
				case QUA: { col = 5; break; }
				case QUI: { col = 6; break; }
				case SEX: { col = 7; break; }
				case SAB: { col = 8; break; }
				case DOM: { col = 9; break; }
			}

			List<AtendimentoRelatorioDTO> aulasDoDia = diaSemanaToAulasMap.get(diaSemanaInt);
			if (aulasDoDia == null || aulasDoDia.isEmpty()) {
				continue;
			}

			// para cada aula
			for (AtendimentoRelatorioDTO aula : aulasDoDia) {
				// obtém o estilo que será aplicado nas células que serão desenhadas
				HSSFCellStyle style = codigoDisciplinaToColorMap.get(aula.getDisciplinaString());
				// obtém a qtd de linhas que devem ser desenhadas para cada crédito da aula em questão
				int linhasDeExcelPorCreditoDaAula = aula.getDuracaoDeUmaAulaEmMinutos()/mdcTemposAula;
				
				if (!ehTatico) {
					AtendimentoOperacionalDTO aulaOp = (AtendimentoOperacionalDTO)aula;
					int index = horariosAulaIdsList.indexOf(aulaOp.getHorarioId());
					if (index != -1) {
						row = initialRow + index;
					}
				}

				// escreve célula principal
				setCell(row,col,sheet,style,HtmlUtils.htmlUnescape(aula.getContentVisaoSala(ReportType.EXCEL)),HtmlUtils.htmlUnescape(aula.getContentToolTipVisaoSala(ReportType.EXCEL)));
				// une células de acordo com a quantidade de créditos da aula
				int rowF = row + (ehTatico ? (aula.getTotalCreditos()*linhasDeExcelPorCreditoDaAula-1) : (aula.getTotalCreditos()-1));
				mergeCells(row,rowF,col,col,sheet,style);

				if (ehTatico) {
					row += aula.getTotalCreditos()*linhasDeExcelPorCreditoDaAula;
				}
			}
		}
		
		return (initialRow + (ehTatico ? (maxCreditos*linhasDeExcelPorCredito) : maxCreditos) + 1);
	}

	private int writeHeader( Sala sala, Turno turno, int row, HSSFSheet sheet, boolean ehTatico )
	{
		int col = 3;

		// Campus
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().campus() ) );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			sala.getUnidade().getCampus().getCodigo() );

		// Sala
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().sala() ) );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			sala.getCodigo() );

		// Capacidade
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().capacidade() ) );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			sala.getCapacidade() );

		row++;
		col = 3;

		// Unidade
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().unidade() ) );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			sala.getUnidade().getCodigo() );

		// Turno
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().turno() ) );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			turno.getNome() );

		// Tipo
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().tipo() ) );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			sala.getTipoSala().getNome() );

		row++;
		col = 2;

		// Créditos ou Horários
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( ehTatico ? this.getI18nConstants().cargaHorariaMinutos() : this.getI18nConstants().horarios() ) );

		// Dias Semana
		for ( Semanas semanas : Semanas.values() )
		{
			setCell( row, col++, sheet,
				this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ],
				semanas.name() );
		}

		row++;
		return row;
	}

	private void fillInCellStyles( HSSFSheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values() )
		{
			this.cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
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

//	private List< HSSFComment > buildExcelCommentsPool( HSSFWorkbook workbook )
//	{
//		List< HSSFComment > excelCommentsPool
//			= new ArrayList< HSSFComment >();
//
//		HSSFSheet sheet = workbook.getSheet(
//			ExcelInformationType.RELATORIO_VISAO_SALA.getSheetName() );
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

	private List< AtendimentoRelatorioDTO > ordenaHorarioAula(
		List< AtendimentoRelatorioDTO > list )
	{
		if ( list == null || list.size() == 0 )
		{
			return Collections.< AtendimentoRelatorioDTO > emptyList();
		}
			
		if ( list.get( 0 ) instanceof AtendimentoTaticoDTO )
		{
			return list;
		}

		List< AtendimentoOperacionalDTO > opList
			= new ArrayList< AtendimentoOperacionalDTO >();

		for ( AtendimentoRelatorioDTO ar : list )
		{
			opList.add( (AtendimentoOperacionalDTO) ar );
		}

		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		opList = service.ordenaPorHorarioAula( opList );

		List< AtendimentoRelatorioDTO > result
			= new ArrayList< AtendimentoRelatorioDTO >( opList );

		return result;
	}
}