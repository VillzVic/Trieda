package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.ArrayList;
import java.util.List;

import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO.StatusRequisicaoOtimizacao;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.services.OtimizarServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class RequisicoesOtimizacaoPresenter {
	
	private OtimizarServiceAsync service;
	private List<RequisicaoOtimizacaoDTO> requisicoesDeOtimizacao;
	private ITriedaI18nGateway i18nGateway;
	
	public RequisicoesOtimizacaoPresenter() {
		service = Services.otimizar();
	}
	
	public void setI18NGateway(ITriedaI18nGateway i18nGateway) {
		this.i18nGateway = i18nGateway;
	}

	public void consultaRequisicoesDeOtimizacao(AsyncCallback<List<RequisicaoOtimizacaoDTO>> callback) throws TriedaException {
		service.consultaRequisicoesDeOtimizacao(callback);
	}
	
	public void setRequisicoesDeOtimizacao(List<RequisicaoOtimizacaoDTO> requisicoesDeOtimizacao) {
		this.requisicoesDeOtimizacao = requisicoesDeOtimizacao;
	}
	
	public List<RequisicaoOtimizacaoDTO> getRequisicoesDeOtimizacaoPorStatus(StatusRequisicaoOtimizacao status) {
		List<RequisicaoOtimizacaoDTO> result = new ArrayList<RequisicaoOtimizacaoDTO>();
		
		if (requisicoesDeOtimizacao != null) {
			for (RequisicaoOtimizacaoDTO requisicaoOtimizacao : requisicoesDeOtimizacao) {
				if (requisicaoOtimizacao.getStatusIndex().equals(status.ordinal())) {
					result.add(requisicaoOtimizacao);
				}
			}
		}
		
		return result;
	}
	
	public void removeRequisicoesDeOtimizacaoPorStatus(StatusRequisicaoOtimizacao status) {
		List<RequisicaoOtimizacaoDTO> requisicoesASeremRemovidas = getRequisicoesDeOtimizacaoPorStatus(status);
		if (!requisicoesASeremRemovidas.isEmpty()) {
			service.removeRequisicoesDeOtimizacao(requisicoesASeremRemovidas,new AbstractAsyncCallbackWithDefaultOnFailure<Void>("Erro ao remover registro de requisição de otimização.",i18nGateway) {
				@Override
				public void onSuccess(Void result) {}
			});
		}
	}
}