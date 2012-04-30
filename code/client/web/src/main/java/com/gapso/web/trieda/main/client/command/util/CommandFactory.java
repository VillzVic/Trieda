package com.gapso.web.trieda.main.client.command.util;

import com.gapso.web.trieda.main.client.command.CarregarSolucaoCommand;
import com.gapso.web.trieda.main.client.command.ConsultarRequisicoesOtimizacaoCommand;
import com.gapso.web.trieda.main.client.command.ICommand;
import com.gapso.web.trieda.main.client.mvp.view.gateways.GenericViewGatewayImp;
import com.gapso.web.trieda.main.client.mvp.view.gateways.ICarregarSolucaoViewGateway;
import com.gapso.web.trieda.main.client.mvp.view.gateways.IGenericViewGateway;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;

public class CommandFactory {
	
	static private IGenericViewGateway genericViewGateway;
	static private IGenericViewGateway getGenericViewGateway() {
		if (genericViewGateway == null) {
			genericViewGateway = new GenericViewGatewayImp();
		}
		return genericViewGateway;
	}
	
	static public ICommand createCarregarSolucaoCommand(CenarioDTO cenarioDTO, ICarregarSolucaoViewGateway roundGateway, ITriedaI18nGateway i18nGateway) {
		return new CarregarSolucaoCommand(cenarioDTO,roundGateway,i18nGateway,getGenericViewGateway());
	}
	
	static public ICommand createCarregarSolucaoCommand(RequisicaoOtimizacaoDTO requisicaoDTO, ITriedaI18nGateway i18nGateway) {
		return new CarregarSolucaoCommand(requisicaoDTO,i18nGateway,getGenericViewGateway());
	}
	
	static public ICommand createConsultarRequisicoesOtimizacaoCommand(boolean showEmpty) {
		return new ConsultarRequisicoesOtimizacaoCommand(showEmpty,getGenericViewGateway());
	}
}