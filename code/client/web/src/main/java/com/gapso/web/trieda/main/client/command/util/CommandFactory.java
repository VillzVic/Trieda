package com.gapso.web.trieda.main.client.command.util;

import com.gapso.web.trieda.main.client.command.CarregarSolucaoCommand;
import com.gapso.web.trieda.main.client.command.ICommand;
import com.gapso.web.trieda.main.client.mvp.view.gateways.ICarregarSolucaoViewGateway;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;

public class CommandFactory {
	
	static public ICommand createCarregarSolucaoCommand(CenarioDTO cenarioDTO, ICarregarSolucaoViewGateway roundGateway, ITriedaI18nGateway i18nGateway) {
		return new CarregarSolucaoCommand(cenarioDTO,roundGateway,i18nGateway);
	}
	
	static public ICommand createCarregarSolucaoCommand(RequisicaoOtimizacaoDTO requisicaoDTO, ITriedaI18nGateway i18nGateway) {
		return new CarregarSolucaoCommand(requisicaoDTO,i18nGateway);
	}
}