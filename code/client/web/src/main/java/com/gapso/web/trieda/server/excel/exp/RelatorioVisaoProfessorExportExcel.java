package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;

import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoProfessorFiltro;


public class RelatorioVisaoProfessorExportExcel	extends RelatorioVisaoExportExcel{
	RelatorioVisaoProfessorFiltro relatorioFiltro;
	private Campus campus;
	private boolean isVisaoProfessor;
	private List<TrioDTO<Integer,Integer,String>> hyperlinkInfo;

	public RelatorioVisaoProfessorExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, boolean isVisaoProfessor, InstituicaoEnsino instituicaoEnsino)
	{
		this(true, cenario, i18nConstants, i18nMessages, null, isVisaoProfessor, instituicaoEnsino);
	}

	public RelatorioVisaoProfessorExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, ExportExcelFilter filter, boolean isVisaoProfessor, 
		InstituicaoEnsino instituicaoEnsino)
	{
		this(true, cenario, i18nConstants, i18nMessages, filter, isVisaoProfessor, instituicaoEnsino);
	}

	public RelatorioVisaoProfessorExportExcel(boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, boolean isVisaoProfessor, 
		InstituicaoEnsino instituicaoEnsino)
	{
		this(removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, isVisaoProfessor, instituicaoEnsino);
	}

	public RelatorioVisaoProfessorExportExcel(boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, ExportExcelFilter filter, 
		boolean isVisaoProfessor, InstituicaoEnsino instituicaoEnsino)
	{
		super(removeUnusedSheets, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino);

		this.isVisaoProfessor = isVisaoProfessor;
		this.hyperlinkInfo = new ArrayList<TrioDTO<Integer,Integer,String>>();

		if(this.relatorioFiltro != null)
			this.campus = Campus.find(((RelatorioVisaoProfessorFiltro) this.getFilter()).getCampusDTO().getId(), this.instituicaoEnsino);
	}
	
	protected String getReportSheetName(){
		return ExcelInformationType.RELATORIO_VISAO_PROFESSOR.getSheetName();
	}

	public boolean isVisaoProfessor(){
		return this.isVisaoProfessor;
	}

	public void setVisaoProfessor(boolean isVisaoProfessor){
		this.isVisaoProfessor = isVisaoProfessor;
	}

	protected String getReportEntity(){
		return "Professor";
	}
	
	@Override
	protected RelatorioVisaoProfessorFiltro getFilter(){
		return this.relatorioFiltro;
	}
	
	@Override
	protected void setFilter(ExportExcelFilter filter){
		this.relatorioFiltro = (RelatorioVisaoProfessorFiltro) filter;
	}

	@SuppressWarnings("unchecked")
	protected <T> boolean getAtendimentosRelatorioDTOList(T mapControlT){
		List<AtendimentoOperacionalDTO> atendimentosOperacionalDTO = 
			(List< AtendimentoOperacionalDTO >) mapControlT;

		boolean idAdmin = true;

		List< Professor > professores = null;
		List< ProfessorVirtual > professoresVirtuais = null;
		List< Turno > turnos = null;

		// Recupera os dados preenchidos nos filtros
		if ( this.relatorioFiltro != null )
		{
			RelatorioVisaoProfessorFiltro relatorioFiltro = (RelatorioVisaoProfessorFiltro) this.getFilter();
			Professor professor = (relatorioFiltro.getProfessorDTO() == null ? null :
				Professor.find(relatorioFiltro.getProfessorDTO().getId(), this.instituicaoEnsino ) );

			ProfessorVirtual professorVirtual = (relatorioFiltro.getProfessorVirtualDTO() == null ? null :
				ProfessorVirtual.find(relatorioFiltro.getProfessorVirtualDTO().getId(), this.instituicaoEnsino ) );

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

			Turno turno = (relatorioFiltro.getTurnoDTO() == null ? null :
				Turno.find(relatorioFiltro.getTurnoDTO().getId(), this.instituicaoEnsino ) );

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

		for ( AtendimentoOperacional domain : atendimentosOperacional )
		{
			atendimentosOperacionalDTO.add(
				ConvertBeans.toAtendimentoOperacionalDTO( domain ) );
		}

		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		atendimentosOperacionalDTO = service.ordenaPorHorarioAula( atendimentosOperacionalDTO );

		return !atendimentosOperacionalDTO.isEmpty();
	}
	
	@Override
	protected boolean fillInExcel(HSSFWorkbook workbook){
		return this.<List<AtendimentoOperacionalDTO>>fillInExcelImpl(workbook);
	}
	
	@SuppressWarnings("unchecked")
	protected List<AtendimentoOperacionalDTO> getStructureReportControl(){
		return new ArrayList<AtendimentoOperacionalDTO>();
	}
	
	@SuppressWarnings("unchecked")
	protected <T> void processStructureReportControl(T mapControlT){
		List<AtendimentoOperacionalDTO> atendimentos = 
			(List<AtendimentoOperacionalDTO>) mapControlT;
		// linha a partir da qual a escrita será iniciada no excel
		int nextRow = this.initialRow;

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
		}

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

					Professor professor = null;
					ProfessorVirtual professorVirtual = null;

					if(!b) {
						professorVirtual = ProfessorVirtual.find(profId, this.instituicaoEnsino);
					} else {
						professor = Professor.find(profId, this.instituicaoEnsino);
					}
					
					nextRow = writeProfessor( this.campus, professor, professorVirtual, turno, listAtendimentos,
						nextRow, mdcTemposAula, labelsDasLinhasDaGradeHoraria);
				}
			}
		}
	}
	
	@Override
	public void resolveHyperlinks(Map<String,Map<String,Map<String,String>>> hyperlinksMap, HSSFWorkbook workbook) {
		Map<String,Map<String,String>> mapLevel2 = hyperlinksMap.get(ExcelInformationType.RELATORIO_VISAO_PROFESSOR.getSheetName());
		if (mapLevel2 != null && !mapLevel2.isEmpty()) {
			HSSFSheet sheet = workbook.getSheet(this.getSheetName());
			for (Entry<String,Map<String,String>> entry : mapLevel2.entrySet()) {
				String cellValue = entry.getKey();
				if (cellValue.equals(ExcelInformationType.RELATORIO_VISAO_SALA.getSheetName())) { 
					for (TrioDTO<Integer,Integer,String> trio : hyperlinkInfo) {
						String cellHyperlink = entry.getValue().get(trio.getTerceiro());
						if (cellHyperlink != null) {
							setCellWithHyperlink(trio.getPrimeiro(),trio.getSegundo(),sheet,cellHyperlink,false);
						}
					}
				}
			}
		}
		hyperlinkInfo.clear();
	}

	private int writeProfessor(Campus campus, Professor professor, ProfessorVirtual professorVirtual, Turno turno, 
		List< AtendimentoOperacionalDTO > atendimentos, int row, int mdcTemposAula, List<String> labelsDasLinhasDaGradeHoraria)
	{
		String professorKey;
		if(professor == null){
			professorKey = professorVirtual.getId().toString()+"#";
		} else {
			professorKey = professor.getId().toString();
		}
		registerHyperlink(
			ExcelInformationType.RELATORIO_VISAO_SALA.getSheetName(),
			ExcelInformationType.RELATORIO_VISAO_PROFESSOR.getSheetName(),
			professorKey, 
			"'"+ExcelInformationType.RELATORIO_VISAO_PROFESSOR.getSheetName()+"'!B"+row
		);
		
		row = writeHeader(getRowsHeadersPairs(campus, professor, professorVirtual, turno), row, false);
		
		// Processa os atendimentos lidos do BD para
		// que os mesmos sejam visualizados na visão professor
		AtendimentosServiceImpl atendimentosService = new AtendimentosServiceImpl();
		List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>(atendimentosService.extraiAulas(atendimentos));
		List<AtendimentoRelatorioDTO> arDTOListProcessada = atendimentosService.uneAulasQuePodemSerCompartilhadas(aulas);
		List<AtendimentoOperacionalDTO> atendimentosParaVisaoProfessor = new ArrayList<AtendimentoOperacionalDTO>(arDTOListProcessada.size());
		for(AtendimentoRelatorioDTO dto : arDTOListProcessada){
			atendimentosParaVisaoProfessor.add((AtendimentoOperacionalDTO) dto);
		}
		atendimentosParaVisaoProfessor = this.ordenaHorarioAula(atendimentosParaVisaoProfessor);
		
		List<AtendimentoRelatorioDTO> atendimentosParaEscrita = new ArrayList<AtendimentoRelatorioDTO>();
		for(AtendimentoOperacionalDTO dto : atendimentosParaVisaoProfessor){
			atendimentosParaEscrita.add((AtendimentoRelatorioDTO) dto);
		}
		
		return writeAulas(atendimentosParaEscrita, row, mdcTemposAula, false, labelsDasLinhasDaGradeHoraria);
	}
	
	protected void onWriteAula(int row, int col, AtendimentoRelatorioDTO aula) {
		// informação para hyperlink
		hyperlinkInfo.add(TrioDTO.create(row,col,aula.getSalaString()));
	}
	
	protected List<List<ParDTO<String, ?>>> getRowsHeadersPairs(Campus campus, Professor professor, 
		ProfessorVirtual professorVirtual, Turno turno)
	{
		List<List<ParDTO<String, ?>>> list = new ArrayList<List<ParDTO<String, ?>>>(); 
		
		String professorNome, professorVirtualNome;
		if(professor == null){
			professorNome = "";
			professorVirtualNome = professorVirtual.getNome();
		}
		else{
			professorNome = professor.getNome();
			professorVirtualNome = "";
		}
		
		List<ParDTO<String, ?>> row = new ArrayList<ParDTO<String, ?>>();
		row.add(ParDTO.create(this.getI18nConstants().campus(), campus.getCodigo()));
		row.add(ParDTO.create(this.getI18nConstants().professor(), professorNome));
		
		list.add(row);
		
		row = new ArrayList<ParDTO<String, ?>>();
		row.add(ParDTO.create(this.getI18nConstants().turno(), turno.getNome()));
		row.add(ParDTO.create(this.getI18nConstants().professorVirtual(), professorVirtualNome));
		
		list.add(row);
		
		return list;
	}

	private List<AtendimentoOperacionalDTO> ordenaHorarioAula(List<AtendimentoOperacionalDTO> list){
		if(list == null || list.size() == 0){
			return Collections.<AtendimentoOperacionalDTO>emptyList();
		}
		
		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		return service.ordenaPorHorarioAula(list);
	}
}
