package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaIncompativelDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.ResumoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TipoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface DisciplinasServiceAsync {
	/**
	 * @see com.gapso.web.trieda.shared.services.DisciplinasService#getHorariosDisponiveis(com.gapso.web.trieda.shared.dtos.DisciplinaDTO)
	 */
	void getHorariosDisponiveis(DisciplinaDTO disciplinaDTO, AsyncCallback<List<HorarioDisponivelCenarioDTO>> callback);
	
	/**
	 * @see com.gapso.web.trieda.shared.services.DisciplinasService#getSemanasLetivas(com.gapso.web.trieda.shared.dtos.DisciplinaDTO)
	 */
	void getSemanasLetivas(DisciplinaDTO disciplinaDTO, AsyncCallback<List<SemanaLetivaDTO>> callback);
	
	/**
	 * @see com.gapso.web.trieda.shared.services.DisciplinasService#associarDisciplinasSemLaboratorioATodosLaboratorios()
	 */
	void associarDisciplinasSemLaboratorioATodosLaboratorios(AsyncCallback<Void> callback);
	
	void getDisciplina( Long id, AsyncCallback< DisciplinaDTO > callback );
	void getList( BasePagingLoadConfig loadConfig, AsyncCallback< ListLoadResult< DisciplinaDTO > > callback );
	void getBuscaList(String nome, String codigo, TipoDisciplinaDTO tipoDisciplinaDTO,
		PagingLoadConfig config, AsyncCallback< PagingLoadResult< DisciplinaDTO > > callback );
	void save( DisciplinaDTO disciplinaDTO, AsyncCallback< Void > callback );
	void remove( List< DisciplinaDTO > disciplinaDTOList, AsyncCallback< Void > callback );
	void getTipoDisciplinaList( AsyncCallback< ListLoadResult< TipoDisciplinaDTO > > callback );
	void getTipoDisciplina( Long id, AsyncCallback< TipoDisciplinaDTO > callback );
	void getListByCursoAndName( List< CursoDTO > cursos, String name, AsyncCallback< ListLoadResult< DisciplinaDTO > > callback );
	void getListByCurriculoIdAndName( Long curriculoId, String name, AsyncCallback< ListLoadResult< DisciplinaDTO > > callback );
	void getListByCursos( List< CursoDTO > cursos, AsyncCallback< ListLoadResult< DisciplinaDTO > > callback );
	void getListByCurriculo( long curriculoId, AsyncCallback< ListLoadResult< DisciplinaDTO > > callback );
	void getFolderChildren( TreeNodeDTO loadConfig, AsyncCallback< List< TreeNodeDTO > > callback );
	void saveDisciplinaToSala( OfertaDTO ofertaDTO, Integer periodo,
		CurriculoDisciplinaDTO cdDTO, SalaDTO salaDTO, AsyncCallback< Void > callback );
	void saveDisciplinaToSala( OfertaDTO ofertaDTO, Integer periodo,
		CurriculoDisciplinaDTO cdDTO, GrupoSalaDTO grupoSalaDTO, AsyncCallback< Void > callback );
	void removeDisciplinaToSala( SalaDTO salaDTO, CurriculoDisciplinaDTO cdDTO, AsyncCallback< Void > callback );
	void removeDisciplinaToSala( GrupoSalaDTO grupoSalaDTO, CurriculoDisciplinaDTO cdDTO, AsyncCallback< Void > callback );
	void saveHorariosDisponiveis( DisciplinaDTO disciplinaDTO,
		List< HorarioDisponivelCenarioDTO > listDTO, AsyncCallback< Void > callback );
	void getDisciplinasByTreeSalas( TreeNodeDTO salaTreeNodeDTO, TreeNodeDTO ofertaTreeNodeDTO,
		TreeNodeDTO curriculoDisciplinaTreeNodeDTO, AsyncCallback< List< TreeNodeDTO > > callback );
	void getDisciplinasByTreeGrupoSalas( TreeNodeDTO grupoSalaTreeNodeDTO, TreeNodeDTO ofertaTreeNodeDTO,
		TreeNodeDTO curriculoDisciplinaTreeNodeDTO, AsyncCallback< List< TreeNodeDTO > > callback );
	void getDisciplinasIncompativeis( CurriculoDTO curriculoDTO, Integer periodo,
		AsyncCallback< List< DisciplinaIncompativelDTO > > callback );
	void saveDisciplinasIncompativeis( List< DisciplinaIncompativelDTO > list, AsyncCallback< Void > callback );
	void salvarDivisaoCredito( DisciplinaDTO disciplinaDTO, DivisaoCreditoDTO divisaoCreditoDTO, AsyncCallback< Void > callback );
	void getDivisaoCredito( DisciplinaDTO disciplinaDTO, AsyncCallback< DivisaoCreditoDTO > callback );
	void getResumos( CenarioDTO cenarioDTO, CampusDTO campusDTO, AsyncCallback< List< ResumoDisciplinaDTO > > callback );
	void removeDivisaoCredito( DisciplinaDTO disciplinaDTO, AsyncCallback< Void > callback );
}
