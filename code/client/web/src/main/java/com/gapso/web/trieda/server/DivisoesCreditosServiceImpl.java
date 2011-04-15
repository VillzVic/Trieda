package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Set;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.DivisaoCredito;
import com.gapso.web.trieda.main.client.services.DivisoesCreditosService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class DivisoesCreditosServiceImpl extends RemoteServiceServlet implements DivisoesCreditosService {

	private static final long serialVersionUID = 245474536841101290L;

	@Override
	public DivisaoCreditoDTO getDivisaoCredito(Long id) {
		return ConvertBeans.toDivisaoCreditoDTO(DivisaoCredito.find(id));
	}
	
	@Override
	public PagingLoadResult<DivisaoCreditoDTO> getList(CenarioDTO cenarioDTO, PagingLoadConfig config) {
		Cenario cenario = Cenario.find(cenarioDTO.getId());
		List<DivisaoCreditoDTO> list = new ArrayList<DivisaoCreditoDTO>();
		Set<DivisaoCredito> divisoesCreditos = cenario.getDivisoesCredito();
		
		for(DivisaoCredito divisaoCredito : divisoesCreditos) {
			list.add(ConvertBeans.toDivisaoCreditoDTO(divisaoCredito));
		}
		Collections.sort(list, new Comparator<DivisaoCreditoDTO>() {
			@Override
			public int compare(DivisaoCreditoDTO d1, DivisaoCreditoDTO d2) {
				return d1.getTotalCreditos().compareTo(d2.getTotalCreditos());
			}
		});
		
		BasePagingLoadResult<DivisaoCreditoDTO> result = new BasePagingLoadResult<DivisaoCreditoDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(DivisaoCredito.count(cenario));
		return result;
	}
	
	@Override
	public void save(DivisaoCreditoDTO divisaoCreditoDTO) {
		DivisaoCredito divisaoCredito = ConvertBeans.toDivisaoCredito(divisaoCreditoDTO);
		if(divisaoCredito.getId() != null && divisaoCredito.getId() > 0) {
			divisaoCredito.merge();
		} else {
			divisaoCredito.persist();
		}
	}
	
	@Override
	public void remove(List<DivisaoCreditoDTO> divisaoCreditoDTOList) {
		for(DivisaoCreditoDTO divisaoCreditoDTO : divisaoCreditoDTOList) {
			ConvertBeans.toDivisaoCredito(divisaoCreditoDTO).remove();
		}
	}

}
