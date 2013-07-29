package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorCampusDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface ProfessoresServiceAsync
{
	void getProfessor( Long id, AsyncCallback< ProfessorDTO > callback );
	void getList( AsyncCallback< ListLoadResult< ProfessorDTO > > callback );
	void getBuscaList( String cpf, TipoContratoDTO tipoContratoDTO, TitulacaoDTO titulacaoDTO,
		AreaTitulacaoDTO areaTitulacaoDTO, PagingLoadConfig config,
		AsyncCallback< PagingLoadResult< ProfessorDTO > > callback );
	void getTipoContrato( Long id, AsyncCallback< TipoContratoDTO > callback );
	void getTiposContratoAll( AsyncCallback< ListLoadResult< TipoContratoDTO > > callback );
	void getTitulacao( Long id, AsyncCallback< TitulacaoDTO > callback );
	void getTitulacoesAll( AsyncCallback< ListLoadResult< TitulacaoDTO > > callback );
	void save( ProfessorDTO professorDTO, AsyncCallback< Void > callback );
	void remove( List< ProfessorDTO > professorDTOList, AsyncCallback< Void > callback );
	void getHorariosDisponiveis( ProfessorDTO professorDTO, AsyncCallback< List< HorarioDisponivelCenarioDTO > > callback );
	void saveHorariosDisponiveis( ProfessorDTO professorDTO,
		List< HorarioDisponivelCenarioDTO > listDTO, AsyncCallback< Void > callback );
	void getProfessorCampusList( CampusDTO campusDTO, ProfessorDTO professorDTO,
		AsyncCallback< PagingLoadResult< ProfessorCampusDTO > > callback );
	void removeProfessorCampus( List< ProfessorCampusDTO > professorCampusDTOList, AsyncCallback< Void > callback );
	void getProfessoresEmCampus( CampusDTO campusDTO, AsyncCallback< ListLoadResult< ProfessorDTO > > callback );
	void getProfessoresNaoEmCampus( CampusDTO campusDTO, AsyncCallback< List< ProfessorDTO > > callback );
	void salvarProfessorCampus( CampusDTO campusDTO, List< ProfessorDTO > professorDTOList, AsyncCallback< Void > callback );
	void getProfessorCampusByCurrentProfessor( AsyncCallback< PagingLoadResult< ProfessorCampusDTO > > callback );
	
	void geraHabilitacaoParaProfessoresVirtuaisCadastrados(AsyncCallback< Void > callback );
	void getAutoCompleteList(BasePagingLoadConfig loadConfig, String tipoComboBox,
			AsyncCallback<ListLoadResult<ProfessorDTO>> callback);
}
