package com.gapso.web.trieda.shared.services;

import java.util.List;
import java.util.Map;

import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ErrorsWarningsInputSolverDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface OtimizarServiceAsync {
	
	/**
	 * @see com.gapso.web.trieda.shared.services.OtimizarService#checkInputDataBeforeRequestOptimization(com.gapso.web.trieda.shared.dtos.ParametroDTO)
	 */
	void checkInputDataBeforeRequestOptimization(ParametroDTO parametroDTO, AsyncCallback<ErrorsWarningsInputSolverDTO> callback) throws Exception;
	
	/**
	 * @see com.gapso.web.trieda.shared.services.OtimizarService#registraRequisicaoDeOtimizacao(com.gapso.web.trieda.shared.dtos.ParametroDTO, java.lang.Long)
	 */
	void registraRequisicaoDeOtimizacao(ParametroDTO parametroDTO, Long round, AsyncCallback<Void> callback);
	
	/**
	 * @see com.gapso.web.trieda.shared.services.OtimizarService#removeRequisicaoDeOtimizacao(com.gapso.web.trieda.shared.dtos.ParametroDTO, java.lang.Long)
	 */
	void removeRequisicaoDeOtimizacao(ParametroDTO parametroDTO, Long round, AsyncCallback<Void> callback);
	
	void removeRequisicoesDeOtimizacao(List<RequisicaoOtimizacaoDTO> requisicoesASeremRemovidas, AsyncCallback<Void> callback);
	
	/**
	 * @see com.gapso.web.trieda.shared.services.OtimizarService#getParametrosDaRequisicaoDeOtimizacao(com.gapso.web.trieda.shared.dtos.CenarioDTO)
	 */
	void getParametrosDaRequisicaoDeOtimizacao(CenarioDTO cenarioDTO, AsyncCallback<ParametroDTO> callback);
	
	void enviaRequisicaoDeOtimizacao(ParametroDTO parametroDTO, AsyncCallback<Long> callback);
	
	/** 
	 * @see com.gapso.web.trieda.shared.services.OtimizarService#consultaRequisicoesDeOtimizacao()
	 */
	void consultaRequisicoesDeOtimizacao(AsyncCallback<List<RequisicaoOtimizacaoDTO>> callback);
	
	void isOptimizing( Long round, AsyncCallback< Boolean > callback );
	void saveContent( CenarioDTO cenarioDTO, Long round,
		AsyncCallback< Map< String, List< String > > > callback );
}