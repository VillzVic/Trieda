package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorCampusDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.services.ProfessoresService;

/**
 * The server side implementation of the RPC service.
 */
public class ProfessoresServiceImpl extends RemoteService implements ProfessoresService {

	private static final long serialVersionUID = -1972558331232685995L;

	@Override
	public ProfessorDTO getProfessor(Long id) {
		if(id == null) return null;
		return ConvertBeans.toProfessorDTO(Professor.find(id));
	}

	@Override
	public List<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(ProfessorDTO professorDTO, SemanaLetivaDTO semanaLetivaDTO) {
		Professor professor = Professor.find(professorDTO.getId());
		SemanaLetiva semanaLetiva = SemanaLetiva.find(semanaLetivaDTO.getId());
		List<HorarioDisponivelCenario> list = professor.getHorarios(semanaLetiva);
		List<HorarioDisponivelCenarioDTO> listDTO = ConvertBeans.toHorarioDisponivelCenarioDTO(list);
		
		return listDTO;
	}
	
	@Override
	public void saveHorariosDisponiveis(ProfessorDTO professorDTO, SemanaLetivaDTO semanaLetivaDTO, List<HorarioDisponivelCenarioDTO> listDTO) {
		SemanaLetiva semanaLetiva = SemanaLetiva.find(semanaLetivaDTO.getId());
		List<HorarioDisponivelCenario> listSelecionados = ConvertBeans.toHorarioDisponivelCenario(listDTO);
		Professor professor = Professor.find(professorDTO.getId());
		List<HorarioDisponivelCenario> adicionarList = new ArrayList<HorarioDisponivelCenario> (listSelecionados);
		adicionarList.removeAll(professor.getHorarios(semanaLetiva));
		List<HorarioDisponivelCenario> removerList = new ArrayList<HorarioDisponivelCenario> (professor.getHorarios(semanaLetiva));
		removerList.removeAll(listSelecionados);
		for(HorarioDisponivelCenario o : removerList) {
			o.getProfessores().remove(professor);
			o.merge();
		}
		for(HorarioDisponivelCenario o : adicionarList) {
			o.getProfessores().add(professor);
			o.merge();
		}
	}
	
	@Override
	public ListLoadResult<ProfessorDTO> getList() {
		List<ProfessorDTO> list = new ArrayList<ProfessorDTO>();
		for(Professor professor : Professor.findAll()) {
			list.add(ConvertBeans.toProfessorDTO(professor));
		}
		return new BaseListLoadResult<ProfessorDTO>(list);
	}
	
	@Override
	public PagingLoadResult<ProfessorDTO> getBuscaList(String cpf, TipoContratoDTO tipoContratoDTO, TitulacaoDTO titulacaoDTO, AreaTitulacaoDTO areaTitulacaoDTO, PagingLoadConfig config) {
		TipoContrato tipoContrato	= (tipoContratoDTO == null)?	null : TipoContrato.find(tipoContratoDTO.getId());
		Titulacao titulacao 		= (titulacaoDTO == null)? 		null : Titulacao.find(titulacaoDTO.getId());
		AreaTitulacao areaTitulacao	= (areaTitulacaoDTO == null)?	null : AreaTitulacao.find(areaTitulacaoDTO.getId());
		
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		
		List<ProfessorDTO> list = new ArrayList<ProfessorDTO>();
		for(Professor professor : Professor.findBy(cpf, tipoContrato, titulacao, areaTitulacao, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toProfessorDTO(professor));
		}
		BasePagingLoadResult<ProfessorDTO> result = new BasePagingLoadResult<ProfessorDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Professor.count(cpf, tipoContrato, titulacao, areaTitulacao));
		return result;
	}
	
	@Override
	public TipoContratoDTO getTipoContrato(Long id) {
		return ConvertBeans.toTipoContratoDTO(TipoContrato.find(id));
	}
	
	@Override
	public ListLoadResult<TipoContratoDTO> getTiposContratoAll() {
		List<TipoContratoDTO> listDTO = new ArrayList<TipoContratoDTO>();
		List<TipoContrato> list = TipoContrato.findAll();
		for(TipoContrato tipoContrato : list) {
			listDTO.add(ConvertBeans.toTipoContratoDTO(tipoContrato));
		}
		return new BaseListLoadResult<TipoContratoDTO>(listDTO);
	}
	
	@Override
	public TitulacaoDTO getTitulacao(Long id) {
		return ConvertBeans.toTitulacaoDTO(Titulacao.find(id));
	}
	
	@Override
	public ListLoadResult<TitulacaoDTO> getTitulacoesAll() {
		List<TitulacaoDTO> listDTO = new ArrayList<TitulacaoDTO>();
		List<Titulacao> list = Titulacao.findAll();
		for(Titulacao titulacao : list) {
			listDTO.add(ConvertBeans.toTitulacaoDTO(titulacao));
		}
		return new BaseListLoadResult<TitulacaoDTO>(listDTO);
	}
	
	@Override
	public void save(ProfessorDTO professorDTO) {
		onlyAdministrador();
		
		Professor professor = ConvertBeans.toProfessor(professorDTO);
		if(professor.getId() != null && professor.getId() > 0) {
			professor.merge();
		} else {
			professor.persist();
			// TODO Pegar a semana letiva do cenario do professor
			Set<HorarioAula> horariosAula = SemanaLetiva.findAll().get(0).getHorariosAula();
			for(HorarioAula horarioAula : horariosAula) {
				Set<HorarioDisponivelCenario> horariosDisponiveis = horarioAula.getHorariosDisponiveisCenario();
				for(HorarioDisponivelCenario horarioDisponivel : horariosDisponiveis) {
					horarioDisponivel.getProfessores().add(professor);
					horarioDisponivel.merge();
				}
			}
		}
	}
	
	@Override
	public void remove(List<ProfessorDTO> professorDTOList) {
		onlyAdministrador();
		
		for(ProfessorDTO professorDTO : professorDTOList) {
//			ConvertBeans.toProfessor(professorDTO).remove();
			Professor.find(professorDTO.getId()).remove();
		}
	}

	@Override
	public ListLoadResult<ProfessorDTO> getProfessoresEmCampus(CampusDTO campusDTO) {
		onlyAdministrador();
		
		Campus campus = Campus.find(campusDTO.getId());
		Set<Professor> list = campus.getProfessores();
		List<ProfessorDTO> listDTO = new ArrayList<ProfessorDTO>(list.size());
		for(Professor professor : list) {
			listDTO.add(ConvertBeans.toProfessorDTO(professor));
		}
		return new BaseListLoadResult<ProfessorDTO>(listDTO);
	}
	
	@Override
	public List<ProfessorDTO> getProfessoresNaoEmCampus(CampusDTO campusDTO) {
		onlyAdministrador();
		
		Campus campus = Campus.find(campusDTO.getId());
		Set<Professor> listAssociados = campus.getProfessores();
		List<Professor> list = Professor.findAll();
		list.removeAll(listAssociados);
		List<ProfessorDTO> listDTO = new ArrayList<ProfessorDTO>(list.size());
		for(Professor professor : list) {
			listDTO.add(ConvertBeans.toProfessorDTO(professor));
		}
		return listDTO;
	}
	
	@Override
	public PagingLoadResult<ProfessorCampusDTO> getProfessorCampusByCurrentProfessor() {
		onlyProfessor();
		
		List<ProfessorCampusDTO> list = ConvertBeans.toProfessorCampusDTO(getUsuario().getProfessor());
		return new BasePagingLoadResult<ProfessorCampusDTO>(list);
	}
	@Override
	public PagingLoadResult<ProfessorCampusDTO> getProfessorCampusList(CampusDTO campusDTO, ProfessorDTO professorDTO) {
		onlyAdministrador();
		
		List<ProfessorCampusDTO> list = null;
		if(campusDTO != null && professorDTO == null) list = ConvertBeans.toProfessorCampusDTO(Campus.find(campusDTO.getId()));
		else if(campusDTO == null && professorDTO != null) list = ConvertBeans.toProfessorCampusDTO(Professor.find(professorDTO.getId()));
		else if(campusDTO == null && professorDTO == null) {
			list = new ArrayList<ProfessorCampusDTO>();
			for(Campus campus : Campus.findAll()) {
				list.addAll(ConvertBeans.toProfessorCampusDTO(campus));
			}
		}
		else if(campusDTO != null && professorDTO != null) {
			Campus campus = Campus.find(campusDTO.getId());
			Professor professor = Professor.find(professorDTO.getId());
			for(Campus c : professor.getCampi()) {
				if(campus.equals(c)) {
					list = new ArrayList<ProfessorCampusDTO>(1);
					ProfessorCampusDTO dto = new ProfessorCampusDTO();
					dto.setProfessorId(professor.getId());
					dto.setProfessorString(professor.getNome());
					dto.setProfessorCpf(professor.getCpf());
					dto.setCampusId(campus.getId());
					dto.setCampusString(campus.getCodigo());
					list.add(dto);
					break;
				}
			}
		}
		return new BasePagingLoadResult<ProfessorCampusDTO>(list);
	}
	
	@Override
	public void salvarProfessorCampus(CampusDTO campusDTO, List<ProfessorDTO> professorDTOList) {
		onlyAdministrador();
		
		Campus campus = Campus.find(campusDTO.getId());
		List<Professor> professorList = new ArrayList<Professor>(professorDTOList.size());
		for(ProfessorDTO professorDTO : professorDTOList) {
			professorList.add(Professor.find(professorDTO.getId()));
		}
		// Remove os que não estão na lista
		for(Professor professor : campus.getProfessores()) {
			if(!professorList.contains(professor)) {
				professor.getCampi().remove(campus);
				professor.merge();
			}
		}
		// Adiciona na lista os que não estão
		for(Professor professor : professorList) {
			if(!campus.getProfessores().contains(professor)) {
				professor.getCampi().add(campus);
				professor.merge();
			}
		}
	}
	
	@Override
	public void removeProfessorCampus(List<ProfessorCampusDTO> professorCampusDTOList) {
		onlyAdministrador();
		
		for(ProfessorCampusDTO pcDTO : professorCampusDTOList) {
			Professor professor = Professor.find(pcDTO.getProfessorId());
			Campus campus = Campus.find(pcDTO.getCampusId());
			
			professor.getCampi().remove(campus);
			professor.merge();
		}
	}
	
}
