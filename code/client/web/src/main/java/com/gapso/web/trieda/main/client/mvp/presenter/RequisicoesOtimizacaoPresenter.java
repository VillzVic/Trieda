package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO;
import com.gapso.web.trieda.shared.services.OtimizarServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class RequisicoesOtimizacaoPresenter {
	
	private OtimizarServiceAsync service;
	
	public RequisicoesOtimizacaoPresenter() {
		service = Services.otimizar();
	}

	public void consultaRequisicoesDeOtimizacao(AsyncCallback<List<RequisicaoOtimizacaoDTO>> callback) {
		service.consultaRequisicoesDeOtimizacao(callback);
	}
}