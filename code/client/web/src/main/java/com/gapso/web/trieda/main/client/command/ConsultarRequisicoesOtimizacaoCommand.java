package com.gapso.web.trieda.main.client.command;

import com.gapso.web.trieda.main.client.mvp.view.gateways.IGenericViewGateway;

public class ConsultarRequisicoesOtimizacaoCommand implements ICommand {
	
	private boolean showEmpty;
	private IGenericViewGateway genericViewGateway;
	
	public ConsultarRequisicoesOtimizacaoCommand(boolean showEmpty, IGenericViewGateway genericViewGateway) {
		this.showEmpty = showEmpty;
		this.genericViewGateway = genericViewGateway;
	}

	@Override
	public void execute() {
		genericViewGateway.showRequisicoesOtimizacaoView(showEmpty);
	}
}