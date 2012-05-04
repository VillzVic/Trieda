package com.gapso.web.trieda.main.client.command;

import com.gapso.web.trieda.main.client.mvp.view.gateways.IGenericViewGateway;
import com.gapso.web.trieda.main.client.mvp.view.gateways.IRequisicoesOtimizacaoViewGateway;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.services.OtimizarServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;

public class CancelarRequisicaoOtimizacaoCommand implements ICommand {
	private RequisicaoOtimizacaoDTO requisicaoDTO;
	private IRequisicoesOtimizacaoViewGateway reqOtmViewGateway;
	private ITriedaI18nGateway i18nGateway;
	private IGenericViewGateway genericViewGateway;
	
	public CancelarRequisicaoOtimizacaoCommand(RequisicaoOtimizacaoDTO requisicaoDTO, IRequisicoesOtimizacaoViewGateway reqOtmViewGateway,ITriedaI18nGateway i18nGateway, IGenericViewGateway genericViewGateway) {
		this.requisicaoDTO = requisicaoDTO;
		this.reqOtmViewGateway = reqOtmViewGateway;
		this.i18nGateway = i18nGateway;
		this.genericViewGateway = genericViewGateway;
	}
	
	@Override
	public void execute() {
		final OtimizarServiceAsync service = Services.otimizar();
		service.cancelaRequisicaoDeOtimizacao(requisicaoDTO.getRound(),new AbstractAsyncCallbackWithDefaultOnFailure<Boolean>(i18nGateway) {
			@Override
			public void onSuccess(Boolean result) {
				service.removeRequisicaoDeOtimizacao(requisicaoDTO,new AbstractAsyncCallbackWithDefaultOnFailure<Void>(i18nGateway) {
					@Override
					public void onSuccess(Void result) {
						reqOtmViewGateway.removeRequisicaoOtimizacaoFromGrid(requisicaoDTO);
						genericViewGateway.showMessageBoxInfo(i18nGateway.getI18nConstants().informacao(),"Requisição de otimização cancelada com sucesso!",null);
					}
				});
			}
		});
	}
}