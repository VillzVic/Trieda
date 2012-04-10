package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.gapso.web.trieda.main.client.mvp.view.OtimizarMessagesView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.OtimizarServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.google.gwt.user.client.Timer;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public abstract class AbstractRequisicaoOtimizacaoPresenter implements Presenter {
	
	private final OtimizarServiceAsync service;
	private ITriedaI18nGateway i18nGateway;
	
	protected AbstractRequisicaoOtimizacaoPresenter(ITriedaI18nGateway i18nGateway) {
		this.i18nGateway = i18nGateway;
		this.service = Services.otimizar();
	}
	
	abstract protected void enviaRequisicaoOtimizacaoOnFailure();
	abstract protected void enviaRequisicaoOtimizacaoOnSuccess();
	abstract protected void otimizacaoFinalizada();
	

	protected void enviaRequisicaoDeOtimizacao(final ParametroDTO parametroDTO, final CenarioDTO cenarioDTO) {
		service.enviaRequisicaoDeOtimizacao(parametroDTO,new AbstractAsyncCallbackWithDefaultOnFailure<Long>("Não foi possível gerar a grade de horários.",i18nGateway) {
			@Override
			public void onFailure(Throwable caught) {
				super.onFailure(caught);
				enviaRequisicaoOtimizacaoOnFailure();
			}

			@Override
			public void onSuccess(final Long round) {
				Info.display("Otimização","A requisição de otimização foi enviada com sucesso!");
				
				service.registraRequisicaoDeOtimizacao(parametroDTO,round,new AbstractAsyncCallbackWithDefaultOnFailure<Void>("Erro ao tentar registrar no BD a requisição de otimização.",i18nGateway) {
					@Override
					public void onSuccess(Void result) {
						Info.display("Otimização","A requisição de otimização foi registrada na base de dados com sucesso!");
					}
				});
				
				checkSolver(round,parametroDTO,cenarioDTO);
				
				enviaRequisicaoOtimizacaoOnSuccess();
			}
		});
	}
	
	private void checkSolver(final Long round, final ParametroDTO parametroDTO, final CenarioDTO cenarioDTO) {
		final Timer t = new Timer() {
			@Override
			public void run() {
				final FutureResult<Boolean> futureBoolean = new FutureResult<Boolean>();
				service.isOptimizing(round,futureBoolean);
				
				FutureSynchronizer synch = new FutureSynchronizer(futureBoolean);
				synch.addCallback(new AbstractAsyncCallbackWithDefaultOnFailure<Boolean>("Erro ao verificar se a requisição de otimização está ou não em andamento",i18nGateway) {
					@Override
					public void onSuccess(Boolean result) {
						if (futureBoolean.result()) {
							checkSolver(round,parametroDTO,cenarioDTO);
						} else {
							Info.display("Otimização","Otimização finalizada!");
							atualizaSaida(round,cenarioDTO,parametroDTO);
							otimizacaoFinalizada();
						}
					}
				});
			}
		};

		t.schedule( 5 * 1000 );
	}
	
	private void atualizaSaida(final Long round, final CenarioDTO cenarioDTO, final ParametroDTO parametroDTO) {
		service.removeRequisicaoDeOtimizacao(parametroDTO,round,new AbstractAsyncCallbackWithDefaultOnFailure<Void>("Erro ao remover registro de requisição de otimização.",i18nGateway) {
			@Override
			public void onSuccess(Void result) {
				Info.display("Otimização","O registro da requisição de otimização foi removido com sucesso da base de dados!");
			}
		});
		
		final FutureResult<Map<String,List<String>>> futureBoolean = new FutureResult<Map<String,List<String>>>();
		service.saveContent(cenarioDTO,round,futureBoolean);
		FutureSynchronizer synch = new FutureSynchronizer(futureBoolean);
		synch.addCallback(new AbstractAsyncCallbackWithDefaultOnFailure<Boolean>("Erro ao ler a saída do solver",i18nGateway) {
			@Override
			public void onSuccess(Boolean result) {
				Map<String,List<String>> ret = futureBoolean.result();

				if (ret.get("warning").isEmpty() && ret.get("error").isEmpty()) {
					MessageBox.alert("Otimização","Saída salva com sucesso",null);
				}
				else {
					Presenter presenter = new OtimizarMessagesPresenter(ret.get("warning"),ret.get("error"),new OtimizarMessagesView());
					presenter.go(null);
				}
			}
		});
	}
}