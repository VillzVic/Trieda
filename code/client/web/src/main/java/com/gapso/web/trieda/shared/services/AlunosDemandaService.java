package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "alunosDemanda" )
public interface AlunosDemandaService
	extends RemoteService
{
	AlunoDemandaDTO getAlunoDemanda( Long id );
	ListLoadResult< AlunoDemandaDTO > getAlunosDemandaList( DemandaDTO demandaDTO );
	void saveAlunoDemanda( DemandaDTO demandaDTO, AlunoDemandaDTO alunoDemandaDTO );
	void removeAlunosDemanda( List< AlunoDemandaDTO > list );
}
