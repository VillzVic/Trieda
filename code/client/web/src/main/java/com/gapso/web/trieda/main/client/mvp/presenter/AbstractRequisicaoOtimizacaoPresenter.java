package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.gapso.web.trieda.main.client.mvp.view.OtimizarMessagesView;
import com.gapso.web.trieda.main.client.mvp.view.RequisicoesOtimizacaoView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.EstatisticasInputSolverXMLDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO.StatusRequisicaoOtimizacao;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.OtimizarServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.google.gwt.user.client.Timer;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public abstract class AbstractRequisicaoOtimizacaoPresenter implements Presenter {
	
	private final OtimizarServiceAsync service;
	private ITriedaI18nGateway i18nGateway;
	private RequisicaoOtimizacaoDTO requisicaoOtimizacaoRegistrada;
	
	protected AbstractRequisicaoOtimizacaoPresenter(ITriedaI18nGateway i18nGateway) {
		this.i18nGateway = i18nGateway;
		this.service = Services.otimizar();
	}
	
	abstract protected void enviaRequisicaoOtimizacaoOnFailure();
	abstract protected void enviaRequisicaoOtimizacaoOnSuccess();
	abstract protected void otimizacaoFinalizada();
	

	protected void enviaRequisicaoDeOtimizacao(final ParametroDTO parametroDTO, final CenarioDTO cenarioDTO, final AcompanhamentoPanelView av) {
		service.enviaRequisicaoDeOtimizacao(parametroDTO,new AbstractAsyncCallbackWithDefaultOnFailure<TrioDTO<Long,ParametroDTO,EstatisticasInputSolverXMLDTO>>("Não foi possível gerar a grade de horários.",i18nGateway) {
			@Override
			public void onFailure(Throwable caught) {
				super.onFailure(caught);
				enviaRequisicaoOtimizacaoOnFailure();
			}

			@Override
			public void onSuccess(final TrioDTO<Long,ParametroDTO,EstatisticasInputSolverXMLDTO> dto) {
				Info.display("Otimização","A requisição de otimização foi enviada com sucesso!");
				
				Long round = dto.getPrimeiro();
				ParametroDTO novoParametroDTO = dto.getSegundo();
				EstatisticasInputSolverXMLDTO estatisticasDTO = dto.getTerceiro();
				
				final RequisicoesOtimizacaoView reqOtmView = new RequisicoesOtimizacaoView(true,new RequisicoesOtimizacaoPresenter());
				
				service.registraRequisicaoDeOtimizacao(novoParametroDTO,round,estatisticasDTO,new AbstractAsyncCallbackWithDefaultOnFailure<RequisicaoOtimizacaoDTO>("Erro ao tentar registrar no BD a requisição de otimização.",i18nGateway) {
					@Override
					public void onSuccess(RequisicaoOtimizacaoDTO result) {
						requisicaoOtimizacaoRegistrada = result;
						Info.display("Otimização","A requisição de otimização foi registrada na base de dados com sucesso!");
						reqOtmView.show();//CommandFactory.createConsultarRequisicoesOtimizacaoCommand(true).execute();
					}
				});
				
				av.hide();
				
				checkSolver(round,cenarioDTO,reqOtmView,null);
				
				enviaRequisicaoOtimizacaoOnSuccess();
			}
		});
	}
	
	private void checkSolver(final Long round, final CenarioDTO cenarioDTO, final RequisicoesOtimizacaoView reqOtmView, final Integer statusReqOtmAnterior) {
		final Timer t = new Timer() {
			@Override
			public void run() {
				service.getStatusRequisicaoDeOtimizacao(round,new AbstractAsyncCallbackWithDefaultOnFailure<Integer>("Erro ao verificar se a requisição de otimização está ou não em andamento",i18nGateway) {
					@Override
					public void onSuccess(Integer statusReqOtm) {
						boolean requisicaoFinalizada = (statusReqOtm != StatusRequisicaoOtimizacao.AGUARDANDO.ordinal()) && (statusReqOtm != StatusRequisicaoOtimizacao.EXECUTANDO.ordinal());
						if (!requisicaoFinalizada) {
							if ((statusReqOtmAnterior == null) || (!statusReqOtmAnterior.equals(statusReqOtm))) {
								reqOtmView.updateRequisicaoOtimizacaoGrid();
							}
							checkSolver(round,cenarioDTO,reqOtmView,statusReqOtm);
						} else {
							if (statusReqOtm == StatusRequisicaoOtimizacao.FINALIZADA_COM_RESULTADO.ordinal())
							{
								MessageBox msgBox = MessageBox.info("Otimização","A requisição de otimização de ID {"+round+"} foi finalizada. Na sequência, o resultado gerado pela mesma será escrito na base de dados do sistema.",null);
								atualizaSaida(round,cenarioDTO,msgBox,reqOtmView);
							}
							else if (statusReqOtm == StatusRequisicaoOtimizacao.FINALIZADA_SEM_RESULTADO.ordinal())
							{
								reqOtmView.updateRequisicaoOtimizacaoGrid();
								MessageBox.alert("Otimização","A requisição de otimização de ID {"+round+"} foi cancelada ou finalizada com erro.",null);
							}
							otimizacaoFinalizada();
						}
					}
				});
				
				//final FutureResult<Boolean> futureBoolean = new FutureResult<Boolean>();
				//service.isOptimizing(round,futureBoolean);
//				FutureSynchronizer synch = new FutureSynchronizer(futureValue);
//				synch.addCallback(new AbstractAsyncCallbackWithDefaultOnFailure<Boolean>("Erro ao verificar se a requisição de otimização está ou não em andamento",i18nGateway) {
//					@Override
//					public void onSuccess(Boolean result) {
//						if (futureBoolean.result()) {
//							checkSolver(round,cenarioDTO);
//						} else {
//							MessageBox.info("Otimização","A requisição de otimização de ID {"+round+"} foi finalizada. Na sequência, o resultado gerado pela mesma será escrito na base de dados do sistema.",null);							
//							atualizaSaida(round,cenarioDTO);
//							otimizacaoFinalizada();
//						}
//					}
//				});
			}
		};

		t.schedule( 5 * 1000 );
	}
	
	private void atualizaSaida(final Long round, final CenarioDTO cenarioDTO, final MessageBox msgBoxAnterior, final RequisicoesOtimizacaoView reqOtmView) {		
		/*service.removeRequisicaoDeOtimizacao(requisicaoOtimizacaoRegistrada,new AbstractAsyncCallbackWithDefaultOnFailure<Void>("Erro ao remover registro de requisição de otimização.",i18nGateway) {
			@Override
			public void onSuccess(Void result) {
				requisicaoOtimizacaoRegistrada = null;
				Info.display("Otimização","O registro da requisição de otimização foi removido com sucesso da base de dados!");
			}
		});*/
		
		AcompanhamentoPanelView av = new AcompanhamentoPanelView();
		new AcompanhamentoPanelPresenter("chaveOtimizacao", av);
		final AcompanhamentoPanelView avf = av;
		
		final FutureResult<Map<String,List<String>>> futureBoolean = new FutureResult<Map<String,List<String>>>();
		service.saveContent(cenarioDTO,round,futureBoolean);
		FutureSynchronizer synch = new FutureSynchronizer(futureBoolean);
		synch.addCallback(new AbstractAsyncCallbackWithDefaultOnFailure<Boolean>("Erro ao ler a saída do solver",i18nGateway) {
			@Override
			public void onSuccess(Boolean result) {
				msgBoxAnterior.close(); // apenas para fechar a mensagem anterior antes de mostrar a próxima
				avf.hide();
				reqOtmView.updateRequisicaoOtimizacaoGrid();
				
				Map<String,List<String>> ret = futureBoolean.result();
				if (ret.get("warning").isEmpty() && ret.get("error").isEmpty()) {
					MessageBox.info("Otimização","O resultado gerado pela requisição de otimização de ID {"+round+"} foi registrado na base de dados com sucesso!",null);
				}
				else {
					Presenter presenter = new OtimizarMessagesPresenter(ret.get("warning"),ret.get("error"),new OtimizarMessagesView());
					presenter.go(null);
				}
			}
		});
	}
}