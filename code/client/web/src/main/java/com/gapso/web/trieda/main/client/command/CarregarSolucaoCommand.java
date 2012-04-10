package com.gapso.web.trieda.main.client.command;

import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.widget.MessageBox;
import com.gapso.web.trieda.main.client.mvp.presenter.OtimizarMessagesPresenter;
import com.gapso.web.trieda.main.client.mvp.view.OtimizarMessagesView;
import com.gapso.web.trieda.main.client.mvp.view.gateways.ICarregarSolucaoViewGateway;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
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
	
	public CarregarSolucaoCommand(CenarioDTO cenarioDTO, ICarregarSolucaoViewGateway roundGateway, ITriedaI18nGateway i18nGateway) {
		this.cenarioDTO = cenarioDTO;
		this.i18nGateway = i18nGateway;
		this.roundGateway = roundGateway;
	}
	
	public CarregarSolucaoCommand(RequisicaoOtimizacaoDTO requisicaoDTO, ITriedaI18nGateway i18nGateway) {
		this.round = requisicaoDTO.getRound();
		this.cenarioId = requisicaoDTO.getCenarioId();
		this.i18nGateway = i18nGateway;
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
					MessageBox.alert("OTIMIZADO","Saída salva com sucesso",null);
				}
				else {
					Presenter presenter = new OtimizarMessagesPresenter(ret.get("warning"),ret.get("error"),new OtimizarMessagesView());
					presenter.go(null);
				}
			}
		});
	}
}
