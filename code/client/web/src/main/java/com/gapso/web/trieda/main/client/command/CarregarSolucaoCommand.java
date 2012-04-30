package com.gapso.web.trieda.main.client.command;

import java.util.List;
import java.util.Map;

import com.gapso.web.trieda.main.client.mvp.view.gateways.ICarregarSolucaoViewGateway;
import com.gapso.web.trieda.main.client.mvp.view.gateways.IGenericViewGateway;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class CarregarSolucaoCommand implements ICommand {
	
	private Long round;
	private Long cenarioId;
	private CenarioDTO cenarioDTO;
	private ITriedaI18nGateway i18nGateway;
	private ICarregarSolucaoViewGateway roundGateway;
	private IGenericViewGateway genericViewGateway;
	
	public CarregarSolucaoCommand(CenarioDTO cenarioDTO, ICarregarSolucaoViewGateway roundGateway, ITriedaI18nGateway i18nGateway, IGenericViewGateway genericViewGateway) {
		this.cenarioDTO = cenarioDTO;
		this.i18nGateway = i18nGateway;
		this.roundGateway = roundGateway;
		this.genericViewGateway = genericViewGateway;
	}
	
	public CarregarSolucaoCommand(RequisicaoOtimizacaoDTO requisicaoDTO, ITriedaI18nGateway i18nGateway, IGenericViewGateway genericViewGateway) {
		this.round = requisicaoDTO.getRound();
		this.cenarioId = requisicaoDTO.getCenarioId();
		this.i18nGateway = i18nGateway;
		this.genericViewGateway = genericViewGateway;
	}

	@Override
	public void execute() {
		if (cenarioDTO == null) {
			Services.cenarios().getCenario(cenarioId,new AbstractAsyncCallbackWithDefaultOnFailure<CenarioDTO>(i18nGateway) {
				@Override
				public void onSuccess(CenarioDTO result) {
					cenarioDTO = result;
					carregaSolucao();
				}
			});
		} else {
			carregaSolucao();
		}
	}

	private void carregaSolucao() {
		final FutureResult<Map<String,List<String>>> futureBoolean = new FutureResult<Map<String,List<String>>>();
		
		if (round == null) {
			round = roundGateway.getRound();
		}
		
		Services.otimizar().saveContent(cenarioDTO,round,futureBoolean);
		
		FutureSynchronizer synch = new FutureSynchronizer(futureBoolean);
		synch.addCallback(new AbstractAsyncCallbackWithDefaultOnFailure<Boolean>(i18nGateway) {
			@Override
			public void onSuccess(Boolean result) {
				Map<String,List<String>> ret = futureBoolean.result();

				if (ret.get("warning").isEmpty() && ret.get("error").isEmpty()) {
					genericViewGateway.showMessageBoxAlert("OTIMIZADO","Saída salva com sucesso",null);
				}
				else {
					genericViewGateway.showOtimizarMessagesView(ret.get("warning"),ret.get("error"));
				}
			}
		});
	}
}