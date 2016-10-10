package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.EquivalenciaDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface EquivalenciasServiceAsync
{
	void getEquivalencia( Long id, AsyncCallback< EquivalenciaDTO > callback );
	void getBuscaList( CenarioDTO cenarioDTO, DisciplinaDTO disciplinaDTO, CursoDTO cursoDTO, PagingLoadConfig config,
		AsyncCallback< PagingLoadResult< EquivalenciaDTO > > callback );
	void save( EquivalenciaDTO equivalenciaDTO, List<CursoDTO> cursosSelecionados, AsyncCallback< Void > callback )  throws TriedaException;
	void remove( List< EquivalenciaDTO > equivalenciaDTOList, AsyncCallback< Void > callback );
	void removeAll( CenarioDTO cenario, AsyncCallback< Void > callback );
	void getCursosEquivalencia(EquivalenciaDTO equivalenciaDTO,
			AsyncCallback<List<CursoDTO>> callback);
}
