package com.gapso.web.trieda.shared.services;

import java.util.List;
import java.util.Map;

import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ErrorsWarningsInputSolverDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath("otimizar")
public interface OtimizarService extends RemoteService {
	
	/**
	 * A partir dos parâmetros de otimização, verifica a consistência dos dados de entrada antes de enviá-los numa requisição
	 * de otimização.
	 * @param parametroDTO parâmetros da requisição de otimização
	 * @return o status do procedimento de verificação da consistência dos dados de entrada, isto é, se os dados estão válidos ou não e,
	 * em caso negativo, quais são os alertas e os erros existentes.
	 * @throws Exception
	 */
	ErrorsWarningsInputSolverDTO checkInputDataBeforeRequestOptimization(ParametroDTO parametroDTO) throws Exception;
	
	/**
	 * Registra no banco de dados a requisição de otimização. Este método é utilizado logo após o envio dos dados de entrada
	 * para o solver.
	 * @param parametroDTO parâmetros da requisição de otimização
	 * @param round identificador da requisição de otimização
	 * @return requisição registrada
	 * @throws TriedaException
	 */
	RequisicaoOtimizacaoDTO registraRequisicaoDeOtimizacao(ParametroDTO parametroDTO, Long round) throws TriedaException;
	
	/**
	 * Remove do banco de dados o registro da requisição de otimização. Este método é utilizado em duas ocasiões:
	 *    - quando é detectada, automaticamente pelo Trieda, o final de uma requisição de otimização.
	 *    - quando é detectada, por consulta do usuário, o final de uma requisição de otimização.
	 * @param requisicaoDTO requisição de otimização a ser removida
	 * @throws TriedaException
	 */
	void removeRequisicaoDeOtimizacao(RequisicaoOtimizacaoDTO requisicaoDTO) throws TriedaException;
	
	/**
	 * 
	 * @param requisicoesASeremRemovidas
	 * @throws TriedaException
	 */
	void removeRequisicoesDeOtimizacao(List<RequisicaoOtimizacaoDTO> requisicoesASeremRemovidas) throws TriedaException;
	
	/**
	 * 
	 * @param cenarioDTO
	 * @return
	 */
	ParametroDTO getParametrosDaRequisicaoDeOtimizacao(CenarioDTO cenarioDTO);
	
	/**
	 * 
	 * @param parametroDTO
	 * @return
	 */
	ParDTO<Long,ParametroDTO> enviaRequisicaoDeOtimizacao(ParametroDTO parametroDTO) throws TriedaException;
	
	/**
	 * 
	 * @return
	 * @throws TriedaException
	 */
	List<RequisicaoOtimizacaoDTO> consultaRequisicoesDeOtimizacao() throws TriedaException;
	
	/**
	 * 
	 * @param round
	 * @return
	 * @throws TriedaException
	 */
	boolean cancelaRequisicaoDeOtimizacao(Long round) throws TriedaException;
	
	Boolean isOptimizing( Long round );
	Map< String, List< String > > saveContent( CenarioDTO cenarioDTO, Long round );
}