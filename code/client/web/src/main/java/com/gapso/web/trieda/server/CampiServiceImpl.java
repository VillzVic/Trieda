package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Estados;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.DeslocamentoCampusDTO;
import com.gapso.web.trieda.client.mvp.model.FileModel;
import com.gapso.web.trieda.client.mvp.model.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.client.services.CampiService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
@Transactional
public class CampiServiceImpl extends RemoteServiceServlet implements CampiService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public CampusDTO getCampus(Long id) {
		if(id == null) return null;
		return ConvertBeans.toCampusDTO(Campus.find(id));
	}
	
	@Override
	public PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(CampusDTO campusDTO) {
		List<HorarioDisponivelCenario> list = new ArrayList<HorarioDisponivelCenario>(Campus.find(campusDTO.getId()).getHorarios());
		List<HorarioDisponivelCenarioDTO> listDTO = ConvertBeans.toHorarioDisponivelCenarioDTO(list);
		
		Map<String, List<HorarioDisponivelCenarioDTO>> horariosTurnos = new HashMap<String, List<HorarioDisponivelCenarioDTO>>();
		for(HorarioDisponivelCenarioDTO o : listDTO) {
			List<HorarioDisponivelCenarioDTO> horarios = horariosTurnos.get(o.getTurnoString());
			if(horarios == null) {
				horarios = new ArrayList<HorarioDisponivelCenarioDTO>();
				horariosTurnos.put(o.getTurnoString(), horarios);
			}
			horarios.add(o);
		}
		
		for(Entry<String, List<HorarioDisponivelCenarioDTO>> entry : horariosTurnos.entrySet()) {
			Collections.sort(entry.getValue());
		}
		
		Map<Date, List<String>> horariosFinalTurnos = new TreeMap<Date, List<String>>();
		for(Entry<String, List<HorarioDisponivelCenarioDTO>> entry : horariosTurnos.entrySet()) {
			Date ultimoHorario = entry.getValue().get(entry.getValue().size()-1).getHorario();
			List<String> turnos = horariosFinalTurnos.get(ultimoHorario);
			if (turnos == null) {
				turnos = new ArrayList<String>();
				horariosFinalTurnos.put(ultimoHorario,turnos);
			}
			turnos.add(entry.getKey());
		}
		
		listDTO.clear();
		for(Entry<Date, List<String>> entry : horariosFinalTurnos.entrySet()) {
			for (String turno : entry.getValue()) {
				listDTO.addAll(horariosTurnos.get(turno));
			}
		}
		
		return new BasePagingLoadResult<HorarioDisponivelCenarioDTO>(listDTO);
	}
	
	@Override
	public void saveHorariosDisponiveis(CampusDTO campusDTO, List<HorarioDisponivelCenarioDTO> listDTO) {
		List<HorarioDisponivelCenario> listSelecionados = ConvertBeans.toHorarioDisponivelCenario(listDTO);
		Campus campus = Campus.find(campusDTO.getId());
		List<Unidade> unidades = Unidade.findByCampus(campus);
		List<Sala> salas = new ArrayList<Sala>();
		for(Unidade unidade : unidades) {
			salas.addAll(Sala.findByUnidade(unidade)); 
		}
		
		List<HorarioDisponivelCenario> removerList = new ArrayList<HorarioDisponivelCenario> (campus.getHorarios());
		removerList.removeAll(listSelecionados);
		for(HorarioDisponivelCenario o : removerList) {
			o.getCampi().remove(campus);
			o.getUnidades().removeAll(unidades);
			o.getSalas().removeAll(salas);
			o.merge();
		}
		
		List<HorarioDisponivelCenario> adicionarList = new ArrayList<HorarioDisponivelCenario> (listSelecionados);
		adicionarList.removeAll(campus.getHorarios());
		for(HorarioDisponivelCenario o : adicionarList) {
			o.getCampi().add(campus);
			o.getUnidades().addAll(unidades);
			o.getSalas().addAll(salas);
			o.merge();
		}
	}
	
	@Override
	public ListLoadResult<CampusDTO> getListAll() {
		List<CampusDTO> campiDTO = new ArrayList<CampusDTO>();
		List<Campus> campi = Campus.findAll();
		for(Campus c : campi) {
			campiDTO.add(ConvertBeans.toCampusDTO(c));
		}
		return new BaseListLoadResult<CampusDTO>(campiDTO);
	}	
	
	@Override
	public PagingLoadResult<CampusDTO> getList(PagingLoadConfig config) {
		List<CampusDTO> list = new ArrayList<CampusDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(Campus campus : Campus.find(config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toCampusDTO(campus));
		}
		BasePagingLoadResult<CampusDTO> result = new BasePagingLoadResult<CampusDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Campus.count());
		return result;
	}

	@Override
	public ListLoadResult<CampusDTO> getList(BasePagingLoadConfig loadConfig) {
		// TODO
		CenarioDTO cenarioDTO = ConvertBeans.toCenarioDTO(Cenario.findMasterData());
		return getBuscaList(cenarioDTO, null, loadConfig.get("query").toString(), null, null, null, loadConfig);
	}

	@Override
	public PagingLoadResult<CampusDTO> getBuscaList(CenarioDTO cenarioDTO, String nome, String codigo, String estadoString, String municipio, String bairro, PagingLoadConfig config) {
		Cenario cenario = Cenario.find(cenarioDTO.getId());
		
		List<CampusDTO> list = new ArrayList<CampusDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		Estados estadoDomain = null;
		if(estadoString != null) {
			for(Estados estado : Estados.values()) {
				if(estado.name().equals(estadoString)) {
					estadoDomain = estado;
					break;
				}
			}
		}
		List<Campus> campi = Campus.findByNomeLikeAndCodigoLikeAndEstadoAndMunicipioLikeAndBairroLike(cenario, nome, codigo, estadoDomain, municipio, bairro, config.getOffset(), config.getLimit(), orderBy);
		for(Campus campus : campi) {
			list.add(ConvertBeans.toCampusDTO(campus));
		}
		BasePagingLoadResult<CampusDTO> result = new BasePagingLoadResult<CampusDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Turno.count());
		return result;
	}
	
	@Override
	public ListLoadResult<CampusDTO> getList() {
		List<CampusDTO> list = new ArrayList<CampusDTO>();
		for(Campus campus : Campus.findAll()) {
			list.add(ConvertBeans.toCampusDTO(campus));
		}
		return new BaseListLoadResult<CampusDTO>(list);
	}
	
	@Override
	public void save(CampusDTO campusDTO) {
		Campus campus = ConvertBeans.toCampus(campusDTO);
		if(campus.getId() != null && campus.getId() > 0) {
			campus.merge();
		} else {
			campus.persist();
		}
	}
	
	@Override
	public void remove(List<CampusDTO> campusDTOList) {
		for(CampusDTO campusDTO : campusDTOList) {
			ConvertBeans.toCampus(campusDTO).remove();
		}
	}
	
	@Override
	public List<DeslocamentoCampusDTO> getDeslocamentos() {
		List<DeslocamentoCampusDTO> list = new ArrayList<DeslocamentoCampusDTO>();
		List<Campus> listCampi = Campus.findAll();
		for(Campus unidade : listCampi) {
			list.add(ConvertBeans.toDeslocamentoCampusDTO(unidade, listCampi));
		}
		Collections.sort(list, new Comparator<DeslocamentoCampusDTO>() {
			@Override
			public int compare(DeslocamentoCampusDTO o1, DeslocamentoCampusDTO o2) {
				return o1.get("origemString").toString().compareToIgnoreCase(o2.get("origemString").toString());
			}
		});
		return list;
	}
	
	@Override
	public void saveDeslocamento(List<DeslocamentoCampusDTO> list) {
		for(DeslocamentoCampusDTO deslocamentoCampusDTO : list) {
			Campus campus = ConvertBeans.toDeslocamentoCampus(deslocamentoCampusDTO);
			campus.merge();
		}
	}
	
	@Override
	public List<FileModel> getResumos(CenarioDTO cenarioDTO, FileModel fileModel) {
		List<FileModel> list = new ArrayList<FileModel>();
		if(!(fileModel instanceof CampusDTO)) {
			Cenario cenario = Cenario.find(cenarioDTO.getId());
			for(Campus campus : cenario.getCampi()) {
				CampusDTO campusDTO = ConvertBeans.toCampusDTO(campus);
				campusDTO.setName(campus.getCodigo() + " (" + campus.getNome() + ")");
				campusDTO.setPath(campus.getCodigo());
				campusDTO.setFolha(true);
				list.add(campusDTO);
			}
		} else {
			Campus campus = Campus.find(((CampusDTO)fileModel).getId());
			Double custoCredito = campus.getValorCredito();
			if(custoCredito == null) custoCredito = 0.0;
			Integer qtdTurma = AtendimentoTatico.countTurma(campus);
			Integer qtdCreditos = AtendimentoTatico.countCreditos(campus);
			Double mediaCreditoTurma = (qtdTurma == 0)? 0.0 : qtdCreditos/qtdTurma;
			Double custoDocenteSemestral = qtdCreditos * custoCredito * 4.5 * 6.0;
			Integer qSalasCenario = Sala.countSalaDeAula(campus);
			Integer qLaboratoriosCenario = Sala.countLaboratorio(campus);
			Integer qSalas = AtendimentoTatico.countSalasDeAula(campus);
			Integer qLaboratorios = AtendimentoTatico.countLaboratorios(campus);
			Double mediaSalaDeAula = qSalas == 0 ? 0 : (qSalasCenario/qSalas) * 100.0;
			Double mediaLaboratorio = qLaboratorios == 0 ? 0 : (qLaboratoriosCenario/qLaboratorios) * 100.0;
			
			List<Demanda> demandas = Demanda.findAllByCampus(campus);
			Integer qtdAlunosAtendidos = 0;
			Integer qtdAlunosNaoAtendidos = 0;
			for(Demanda demanda : demandas) {
				Set<String> turmas = new HashSet<String>();
				List<AtendimentoTatico> atendimentos = AtendimentoTatico.findAllByDemanda(demanda);
				for(AtendimentoTatico atendimento : atendimentos) {
					if(turmas.add(atendimento.getTurma())) {
						qtdAlunosAtendidos += atendimento.getQuantidadeAlunos();
						if(demanda.getQuantidade() >= atendimento.getQuantidadeAlunos()) {
							qtdAlunosNaoAtendidos += (demanda.getQuantidade() - atendimento.getQuantidadeAlunos());
						}
					}
				}
			}
			
			list.add(new FileModel("Turmas abertas: <b>"+qtdTurma+"</b>"));
			list.add(new FileModel("Total de Cr&eacute;ditos semanais: <b>"+qtdCreditos+"</b>"));
			list.add(new FileModel("M&eacute;dia de cr&eacute;ditos por turma: <b>"+mediaCreditoTurma+"</b>"));
			list.add(new FileModel("Custo m&eacute;dio do cr&eacute;dito: <b>R$ "+custoCredito+"</b>"));
			list.add(new FileModel("Custo docente semestral estimado: <b>R$ "+custoDocenteSemestral+"</b>"));
			list.add(new FileModel("Utiliza&ccedil;&atilde;o m&eacute;dia das salas de aula: <b>"+mediaSalaDeAula+"%</b>"));
			list.add(new FileModel("Utiliza&ccedil;&atilde;o m&eacute;dia dos laborat&oacute;rios: <b>"+mediaLaboratorio+"%</b>"));
			list.add(new FileModel("Custo docente por curso"));
			list.add(new FileModel("Custo docente por disciplina"));
			list.add(new FileModel("Total de alunos atendidos: <b>"+qtdAlunosAtendidos+"</b>"));
			list.add(new FileModel("Total de alunos n&atilde;o atendidos: <b>"+qtdAlunosNaoAtendidos+"</b>"));
			list.add(new FileModel("Exportar resultados para excel"));
		}
		return list;
	}
	
}
