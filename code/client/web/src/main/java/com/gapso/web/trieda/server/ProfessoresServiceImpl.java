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
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDTO;
import com.gapso.web.trieda.client.services.ProfessoresService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorCampusDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class ProfessoresServiceImpl extends RemoteServiceServlet implements ProfessoresService {

	private static final long serialVersionUID = -1972558331232685995L;

	@Override
	public ProfessorDTO getProfessor(Long id) {
		return ConvertBeans.toProfessorDTO(Professor.find(id));
	}

	@Override
	public List<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(ProfessorDTO professorDTO) {
		List<HorarioDisponivelCenario> list = new ArrayList<HorarioDisponivelCenario>(Professor.find(professorDTO.getId()).getHorarios());
		List<HorarioDisponivelCenarioDTO> listDTO = ConvertBeans.toHorarioDisponivelCenarioDTO(list);
		
		return listDTO;
	}
	
	@Override
	public void saveHorariosDisponiveis(ProfessorDTO professorDTO, List<HorarioDisponivelCenarioDTO> listDTO) {
		List<HorarioDisponivelCenario> listSelecionados = ConvertBeans.toHorarioDisponivelCenario(listDTO);
		Professor professor = Professor.find(professorDTO.getId());
		List<HorarioDisponivelCenario> adicionarList = new ArrayList<HorarioDisponivelCenario> (listSelecionados);
		adicionarList.removeAll(professor.getHorarios());
		List<HorarioDisponivelCenario> removerList = new ArrayList<HorarioDisponivelCenario> (professor.getHorarios());
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
		for(ProfessorDTO professorDTO : professorDTOList) {
//			ConvertBeans.toProfessor(professorDTO).remove();
			Professor.find(professorDTO.getId()).remove();
		}
	}

	@Override
	public List<ProfessorDTO> getProfessoresEmCampus(CampusDTO campusDTO) {
		Campus campus = Campus.find(campusDTO.getId());
		Set<Professor> list = campus.getProfessores();
		List<ProfessorDTO> listDTO = new ArrayList<ProfessorDTO>(list.size());
		for(Professor professor : list) {
			listDTO.add(ConvertBeans.toProfessorDTO(professor));
		}
		return listDTO;
	}
	
	@Override
	public List<ProfessorDTO> getProfessoresNaoEmCampus(CampusDTO campusDTO) {
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
	public PagingLoadResult<ProfessorCampusDTO> getProfessorCampusList(CampusDTO campusDTO, ProfessorDTO professorDTO) {
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
		for(ProfessorCampusDTO pcDTO : professorCampusDTOList) {
			Professor professor = Professor.find(pcDTO.getProfessorId());
			professor.getCampi().remove(Campus.find(pcDTO.getCampusId()));
			professor.merge();
		}
	}
	
}
