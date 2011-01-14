package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Fixacao;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Sala;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.FixacaoDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.services.FixacoesService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class FixacoesServiceImpl extends RemoteServiceServlet implements FixacoesService {

	private static final long serialVersionUID = -594991176048559553L;

	@Override
	public FixacaoDTO getFixacao(Long id) {
		return ConvertBeans.toFixacaoDTO(Fixacao.find(id));
	}
	
	@Override
	public PagingLoadResult<FixacaoDTO> getBuscaList(String codigo, PagingLoadConfig config) {
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		List<FixacaoDTO> list = new ArrayList<FixacaoDTO>();
		for(Fixacao fixacao : Fixacao.findBy(codigo, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toFixacaoDTO(fixacao));
		}
		BasePagingLoadResult<FixacaoDTO> result = new BasePagingLoadResult<FixacaoDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Fixacao.count());
		return result;
	}
	
	@Override
	public void save(FixacaoDTO fixacaoDTO, List<HorarioDisponivelCenarioDTO> hdcDTOList) {
		Fixacao.entityManager().clear();
		Fixacao fixacao = ConvertBeans.toFixacao(fixacaoDTO);
		List<HorarioDisponivelCenario> listSelecionados = ConvertBeans.toHorarioDisponivelCenario(hdcDTOList);
		List<HorarioDisponivelCenario> adicionarList = new ArrayList<HorarioDisponivelCenario>(listSelecionados);
		
		if(fixacao.getId() != null && fixacao.getId() > 0) {
			fixacao.merge();
		} else {
			fixacao.persist();
		}
		Long id = fixacao.getId();
		Fixacao.entityManager().clear();
		
		fixacao = Fixacao.find(id);
		
		adicionarList.removeAll(fixacao.getHorarios());
		for(HorarioDisponivelCenario o : adicionarList) {
			System.out.print("teste");
			HorarioDisponivelCenario hdc = HorarioDisponivelCenario.findHorarioDisponivelCenario(o.getId());
			hdc.getFixacoes().add(fixacao);
			hdc.merge();
		}
		
		List<HorarioDisponivelCenario> removerList = new ArrayList<HorarioDisponivelCenario>(fixacao.getHorarios());
		removerList.removeAll(listSelecionados);
		for(HorarioDisponivelCenario o : removerList) {
			o.getFixacoes().remove(fixacao);
			o.merge();
		}
	}

	@Override
	public void remove(List<FixacaoDTO> fixacaoDTOList) {
		for(FixacaoDTO fixacaoDTO : fixacaoDTOList) {
			Fixacao.find(fixacaoDTO.getId()).remove();
		}
	}
	
	@Override
	public List<HorarioDisponivelCenarioDTO> getHorariosSelecionados(FixacaoDTO fixacaoDTO) {
		Fixacao fixacao = Fixacao.find(fixacaoDTO.getId());
		return ConvertBeans.toHorarioDisponivelCenarioDTO(new ArrayList<HorarioDisponivelCenario>(fixacao.getHorarios()));
	}

	@Override
	public PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(DisciplinaDTO disciplinaDTO, SalaDTO salaDTO) {
		if(disciplinaDTO == null || salaDTO == null) {
			return new BasePagingLoadResult<HorarioDisponivelCenarioDTO>(new ArrayList<HorarioDisponivelCenarioDTO>());
		}
		Disciplina disciplina = Disciplina.find(disciplinaDTO.getId());
		Sala sala = Sala.find(salaDTO.getId());
		Set<HorarioDisponivelCenario> disciplinaHorarios = disciplina.getHorarios();
		Set<HorarioDisponivelCenario> salaHorarios = sala.getHorarios();
		List<HorarioDisponivelCenario> horarios = intercessaoHorarios(disciplinaHorarios, salaHorarios);
		return new BasePagingLoadResult<HorarioDisponivelCenarioDTO>(ConvertBeans.toHorarioDisponivelCenarioDTO(horarios));
	}

	private List<HorarioDisponivelCenario> intercessaoHorarios(Set<HorarioDisponivelCenario> horario1, Set<HorarioDisponivelCenario> horario2) {
		List<HorarioDisponivelCenario> horarios = new ArrayList<HorarioDisponivelCenario>(horario1);
		horarios.retainAll(horario2);
		return horarios;
	}
	
}
