package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface AlunosDemandaServiceAsync
{
	void getAlunoDemanda( Long id, AsyncCallback< AlunoDemandaDTO > callback );
	void getAlunosDemandaList(DemandaDTO demandaDTO, AsyncCallback< ListLoadResult< AlunoDemandaDTO > > callback );
	void saveAlunoDemanda( DemandaDTO demandaDTO, AlunoDemandaDTO alunoDemandaDTO, AsyncCallback< Void > callback );
	void removeAlunosDemanda( List< AlunoDemandaDTO > list, AsyncCallback< Void > callback );
}
