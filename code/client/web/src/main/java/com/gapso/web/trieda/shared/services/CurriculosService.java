package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "curriculos" )
public interface CurriculosService
	extends RemoteService
{
	CurriculoDTO getCurriculo( Long id );
	ListLoadResult< CurriculoDTO > getList( BasePagingLoadConfig loadConfig );
	PagingLoadResult< CurriculoDTO > getBuscaList( CursoDTO cursoDTO,
		String codigo, String descricao, PagingLoadConfig config );
	void save( CurriculoDTO curriculoDTO );
	void remove( List< CurriculoDTO > curriculoDTOList );
	ListLoadResult< CurriculoDisciplinaDTO > getDisciplinasList( CurriculoDTO curriculoDTO );
	void saveDisciplina( CurriculoDTO curriculoDTO, CurriculoDisciplinaDTO curriculoDisciplinaDTO );
	void removeDisciplina( List< CurriculoDisciplinaDTO > curriculoDisciplinaDTOList );
	List< Integer > getPeriodos( CurriculoDTO curriculoDTO );
	ListLoadResult< CurriculoDTO > getListByCurso( CursoDTO cursoDTO );
	ListLoadResult< CurriculoDTO > getListAll();
}
